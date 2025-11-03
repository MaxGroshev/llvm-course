#include "../sim.h"

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include <fstream>
#include <iostream>
#include <unordered_map>

using namespace llvm;

class asm2ir {
    private:
        static constexpr int REG_FILE_SIZE = 100;
    public: 
        asm2ir() : builder(context) {
            module = std::unique_ptr<Module>(new Module{"top", context});
            voidType = Type::getVoidTy(context);
            int32Type = Type::getInt32Ty(context);
                //[32 x i32] regFile = {0, 0, 0, 0}
            regFileType = ArrayType::get(int32Type, REG_FILE_SIZE);
            regFile = new GlobalVariable(
                *module, regFileType, false, GlobalValue::PrivateLinkage, 0, "regFile");
            regFile->setInitializer(ConstantAggregateZero::get(regFileType));

            // declare void @main()
            funcType = FunctionType::get(voidType, false);
            mainFunc = Function::Create(funcType, Function::ExternalLinkage, "main", module.get());
        }

        int initBB(int argc, char **argv) {
            if (argc != 2) {
                outs() << "[ERROR] Need 1 argument: file with assembler code\n";
                return 1;
            }
            std::ifstream input;
            input.open(argv[1]);
            if (!input.is_open()) {
                outs() << "[ERROR] Can't open " << argv[1] << '\n';
                return 1;
            }

            std::string name;
            std::string arg;

            outs() << "\n#[FILE]:\nBBs:";
            while (input >> name) {
                if (!name.compare("XOR") || !name.compare("MUL") ||
                    !name.compare("SUBi") || !name.compare("PUT_PIXEL") ||
                    !name.compare("INC_NEi")) {
                    input >> arg >> arg >> arg;
                    continue;
                }
                if (!name.compare("BR_COND")) {
                    input >> arg >> arg;
                    continue;
                }
                if (!name.compare("EXIT") || !name.compare("FLUSH")) {
                    continue;
                }

                outs() << " " << name;
                BBMap[name] = BasicBlock::Create(context, name, mainFunc);
            }
            outs() << '\n';
            input.close();

            return 0;
        }

        void parseInstr(int argc, char **argv) {
            std::string name;
            std::string arg;

            // declare void @simFlush(...)
            FunctionType *simFlushType = FunctionType::get(voidType, false);
            FunctionCallee simFlushFunc =
                module->getOrInsertFunction("simFlush", simFlushType);

            std::ifstream input;
            input.open(argv[1]);

            while (input >> name) {
                if (!name.compare("EXIT")) {
                    outs() << "\tEXIT\n";
                    builder.CreateRetVoid();
                    if (input >> name) {
                        outs() << "BB " << name << '\n';
                        builder.SetInsertPoint(BBMap[name]);
                    }
                    continue;
                }
                if (!name.compare("PUT_PIXEL")) {
                    handlePutPixel(input, name, arg);
                    continue;
                }
                if (!name.compare("FLUSH")) {
                    outs() << "\tFLUSH\n";
                    builder.CreateCall(simFlushFunc);
                    continue;
                }
                if (!name.compare("XOR")) {
                    input >> arg;
                    outs() << "\t" << arg;
                    // res
                    Value *res_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                                            std::stoi(arg.substr(1)));
                    input >> arg;
                    outs() << " = " << arg;
                    // arg1
                    Value *arg1_p = builder.CreateConstGEP2_32(
                        regFileType, regFile, 0, std::stoi(arg.substr(1)));
                    input >> arg;
                    outs() << " ^ " << arg << '\n';
                    // arg2
                    Value *arg2_p = builder.CreateConstGEP2_32(
                        regFileType, regFile, 0, std::stoi(arg.substr(1)));
                    Value *xor_arg1_arg2 =
                        builder.CreateXor(builder.CreateLoad(int32Type, arg1_p),
                                        builder.CreateLoad(int32Type, arg2_p));
                    builder.CreateStore(xor_arg1_arg2, res_p);
                    continue;
                }
                if (!name.compare("MUL")) {
                    input >> arg;
                    outs() << "\t" << arg;
                    // res
                    Value *res_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                                            std::stoi(arg.substr(1)));
                    input >> arg;
                    outs() << " = " << arg;
                    // arg1
                    Value *arg1_p = builder.CreateConstGEP2_32(
                        regFileType, regFile, 0, std::stoi(arg.substr(1)));
                    input >> arg;
                    outs() << " * " << arg << '\n';
                    // arg2
                    Value *arg2_p = builder.CreateConstGEP2_32(
                        regFileType, regFile, 0, std::stoi(arg.substr(1)));
                    Value *mul_arg1_arg2 =
                        builder.CreateMul(builder.CreateLoad(int32Type, arg1_p),
                                        builder.CreateLoad(int32Type, arg2_p));
                    builder.CreateStore(mul_arg1_arg2, res_p);
                    continue;
                }
                if (!name.compare("SUBi")) {
                    input >> arg;
                    outs() << "\t" << arg;
                    // res
                    Value *res_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                                            std::stoi(arg.substr(1)));
                    input >> arg;
                    outs() << " = " << arg;
                    // arg1
                    Value *arg1_p = builder.CreateConstGEP2_32(
                        regFileType, regFile, 0, std::stoi(arg.substr(1)));
                    input >> arg;
                    outs() << " - " << arg << '\n';
                    // arg2
                    Value *arg2 = builder.getInt32(std::stoi(arg));
                    Value *sub_arg1_arg2 =
                        builder.CreateSub(builder.CreateLoad(int32Type, arg1_p), arg2);
                    builder.CreateStore(sub_arg1_arg2, res_p);
                    continue;
                }

                if (!name.compare("INC_NEi")) {
                    input >> arg;
                    outs() << "\t" << arg;
                    // res
                    Value *res_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                                            std::stoi(arg.substr(1)));
                    input >> arg;
                    outs() << " = ++" << arg;
                    // arg1
                    Value *arg1_p = builder.CreateConstGEP2_32(
                        regFileType, regFile, 0, std::stoi(arg.substr(1)));
                    Value *arg1 = builder.CreateAdd(
                        builder.CreateLoad(int32Type, arg1_p), builder.getInt32(1));
                    builder.CreateStore(arg1, arg1_p);
                    input >> arg;
                    outs() << " != " << arg << '\n';
                    // arg2
                    Value *arg2 = builder.getInt32(std::stoi(arg));
                    Value *cond = builder.CreateICmpNE(arg1, arg2);
                    builder.CreateStore(cond, res_p);
                    continue;
                }
                if (!name.compare("BR_COND")) {
                    handleBrCond(input, name, arg);
                    continue;   
                }

                if (builder.GetInsertBlock()) {
                    builder.CreateBr(BBMap[name]);
                    outs() << "\tbranch to " << name << '\n';
                }
                outs() << "BB " << name << '\n';
                builder.SetInsertPoint(BBMap[name]);
            }
        }
    
        void verify() {
            outs() << "\n#[LLVM IR]:\n";
            module->print(outs(), nullptr);
            outs() << '\n';
            bool verif = verifyFunction(*mainFunc, &outs());
            outs() << "[VERIFICATION] " << (verif ? "FAIL\n\n" : "OK\n\n");
        }

        int runCode() {
            outs() << "\n#[Running code]\n";
            InitializeNativeTarget();
            InitializeNativeTargetAsmPrinter();

            ExecutionEngine *ee =
                EngineBuilder(std::unique_ptr<Module>(module.get())).create();
            ee->InstallLazyFunctionCreator([](const std::string &fnName) -> void * {
                std::cout << fnName << std::endl;
                if (fnName == "_simFlush") {
                    return reinterpret_cast<void *>(simFlush);
                }
                if (fnName == "_simPutPixel") {
                    return reinterpret_cast<void *>(simPutPixel);
                }
                return nullptr;
            });

            ee->finalizeObject();

            simInit();

            ArrayRef<GenericValue> noargs;
            ee->runFunction(mainFunc, noargs);
            outs() << "#[Code was run]\n";

            simExit();
            return EXIT_SUCCESS;
        }
    private:
        void handlePutPixel(std::ifstream& input, std::string& name, std::string& arg);
        void handleBrCond(std::ifstream& input , std::string& name, std::string& arg);


        LLVMContext context;
        std::unique_ptr<Module> module;
        IRBuilder<> builder;
        GlobalVariable *regFile;

        Type *voidType;
        Type *int32Type;
        FunctionType *funcType;
        ArrayType *regFileType;
        
        Function *mainFunc; // remove from here
        
        std::unordered_map<std::string, BasicBlock *> BBMap;
};