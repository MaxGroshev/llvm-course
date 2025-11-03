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
        regFile = new GlobalVariable(*module, regFileType, false,
                                     GlobalValue::PrivateLinkage, 0, "regFile");
        regFile->setInitializer(ConstantAggregateZero::get(regFileType));

        // declare void @main()
        funcType = FunctionType::get(voidType, false);
        mainFunc = Function::Create(funcType, Function::ExternalLinkage, "main",
                                    module.get());
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
            if (!name.compare("SIM_FLUSH")) {
                outs() << "\tFLUSH\n";
                builder.CreateCall(simFlushFunc);
                continue;
            }
            if (!name.compare("SIM_RAND")) {
                handleSimRand(input, name, arg);
                continue;
            }
            if (!name.compare("SIM_MAX")) {
                handleSimMax(input, name, arg);
                continue;
            }
            if (!name.compare("SIM_MIN")) {
                handleSimMin(input, name, arg);
                continue;
            }
            if (!name.compare("SIM_ABS")) {
                handleSimMin(input, name, arg);
                continue;
            }

            if (!name.compare("SUB")) {
                handleSub(input, name, arg);
                continue;
            }
            if (!name.compare("ADDi")) {
                handleAddi(input, name, arg);
                continue;
            }
            if (!name.compare("BR_COND")) {
                handleBrCond(input, name, arg);
                continue;
            }
            if (!name.compare("BR")) {
                handleBr(input, name, arg);
                continue;
            }
            if (!name.compare("ANDi")) {
                handleAndi(input, name, arg);
                continue;
            }
            if (!name.compare("MOV")) {
                handleMov(input, name, arg);
                continue;
            }
            if (!name.compare("MOVi")) {
                handleMovi(input, name, arg);
                continue;
            }
            if (!name.compare("SREM")) {
                handleSremi(input, name, arg);
                continue;
            }
            if (!name.compare("SEXT")) {
                handleSremi(input, name, arg);
                continue;
            }
            if (!name.compare("ZEXT")) {
                handleZext(input, name, arg);
                continue;
            }
            if (!name.compare("TRUNC")) {
                handleTrunc(input, name, arg);
                continue;
            }
            if (!name.compare("GETELEMPTR_2DEM")) {
                handleGetelemptr(input, name, arg);
                continue;
            }
            if (!name.compare("ST")) {
                handleSt(input, name, arg);
                continue;
            }
            if (!name.compare("LD")) {
                handleLd(input, name, arg);
                continue;
            }
            if (!name.compare("SELECT")) {
                handleSelect(input, name, arg);
                continue;
            }
            if (!name.compare("CMP_EQ")) {
                handleCmpEq(input, name, arg);
                continue;
            }
            if (!name.compare("CMP_SGT")) {
                handleCmpSgt(input, name, arg);
                continue;
            }
            if (!name.compare("CMP_SLT")) {
                handleCmpSlt(input, name, arg);
                continue;
            }
            if (!name.compare("ALLOCA_2DEM")) {
                handleAlloca(input, name, arg);
                continue;
            }
            if (!name.compare("SHL")) {
                handleShli(input, name, arg);
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
    void handlePutPixel(std::ifstream &input, std::string &name,
                        std::string &arg);
    void handleSimRand(std::ifstream &input, std::string &name,
                       std::string &arg);
    void handleSimMax(std::ifstream &input, std::string &name,
                      std::string &arg);
    void handleSimMin(std::ifstream &input, std::string &name,
                      std::string &arg);
    void handleSimAbs(std::ifstream &input, std::string &name,
                      std::string &arg);

    void handleBrCond(std::ifstream &input, std::string &name,
                      std::string &arg);
    void handleSub(std::ifstream &input, std::string &name, std::string &arg);
    void handleAddi(std::ifstream &input, std::string &name, std::string &arg);
    void handleAlloca(std::ifstream &input, std::string &name,
                      std::string &arg);
    void handleSremi(std::ifstream &input, std::string &name, std::string &arg);
    void handleSext(std::ifstream &input, std::string &name, std::string &arg);
    void handleGetelemptr(std::ifstream &input, std::string &name,
                          std::string &arg);
    void handleSt(std::ifstream &input, std::string &name, std::string &arg);
    void handleZext(std::ifstream &input, std::string &name, std::string &arg);
    void handleCmpEq(std::ifstream &input, std::string &name, std::string &arg);
    void handleCmpSlt(std::ifstream &input, std::string &name,
                      std::string &arg);
    void handleCmpSgt(std::ifstream &input, std::string &name,
                      std::string &arg);
    void handleTrunc(std::ifstream &input, std::string &name, std::string &arg);
    void handleBr(std::ifstream &input, std::string &name, std::string &arg);
    void handleAndi(std::ifstream &input, std::string &name, std::string &arg);
    void handleSelect(std::ifstream &input, std::string &name,
                      std::string &arg);
    void handleLd(std::ifstream &input, std::string &name, std::string &arg);
    void handleMov(std::ifstream &input, std::string &name, std::string &arg);
    void handleMovi(std::ifstream &input, std::string &name, std::string &arg);
    void handleShli(std::ifstream &input, std::string &name, std::string &arg);

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