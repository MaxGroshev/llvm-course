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

        regFileType = ArrayType::get(int32Type, REG_FILE_SIZE);
        regFile = new GlobalVariable(*module, regFileType, false,
                                     GlobalValue::PrivateLinkage, 0, "regFile");
        regFile->setInitializer(ConstantAggregateZero::get(regFileType));

        ArrayType *innerArrayType =
            ArrayType::get(Type::getInt32Ty(context), 512);
        ArrayType *outerArrayType = ArrayType::get(innerArrayType, 256);
        // arrayPtrStorage = ArrayType::get(int32Type, REG_FILE_SIZE);
        arrayPtrStorage = new GlobalVariable(*module, outerArrayType, false,
                                             GlobalValue::PrivateLinkage, 0,
                                             "arrayPtrStorage");
        arrayPtrStorage->setInitializer(
            ConstantAggregateZero::get(regFileType));

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
            if (!name.compare("ALLOCA_2DEM") || !name.compare("SREM") ||
                !name.compare("GETELEMPTRi") || !name.compare("GETELEMPTR") ||
                !name.compare("ADDi") || !name.compare("CMP_EQ") ||
                !name.compare("CMP_SGT") || !name.compare("CMP_SLT") ||
                !name.compare("BR_COND") || !name.compare("AND") ||
                !name.compare("SHL") || !name.compare("PUT_PIXEL") ||
                !name.compare("SIM_MAX") || !name.compare("SIM_MIN") ||
                !name.compare("SUB") || !name.compare("ST_BT_OFFSET")) {
                input >> arg >> arg >> arg;
                continue;
            }
            if (!name.compare("SIM_RAND") || !name.compare("BR")) {
                input >> arg;
                continue;
            }
            if (!name.compare("SELECT")) {
                input >> arg >> arg >> arg >> arg;
                continue;
            }
            if (!name.compare("GETELEMPTR_2DEMi") ||
                !name.compare("GETELEMPTR_2DEM") || !name.compare("ST") ||
                !name.compare("LD") || !name.compare("STi")) {
                input >> arg >> arg >> arg >> arg >> arg >> arg;
                continue;
            }
            if (!name.compare("SEXT") || !name.compare("MOV") ||
                !name.compare("MOVi") || !name.compare("ZEXT") ||
                !name.compare("TRUNC") ||
                !name.compare("SIM_ABS")) {
                input >> arg >> arg;
                continue;
            }
            if (!name.compare("EXIT") || !name.compare("SIM_FLUSH")) {
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

        std::ifstream input;
        input.open(argv[1]);

        while (input >> name) {
            std::cout << std::endl;
            if (!name.compare("EXIT")) {
                handleExit(input, name, arg);
                continue;
            }
            if (!name.compare("PUT_PIXEL")) {
                handlePutPixel(input, name, arg);
                continue;
            }
            if (!name.compare("SIM_FLUSH")) {
                handleSimFlush(input, name, arg);
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
                handleSimAbs(input, name, arg);
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
            if (!name.compare("AND")) {
                handleAndi(input, name, arg);
                continue;
            }
            if (!name.compare("SREM")) {
                handleSremi(input, name, arg);
                continue;
            }
            if (!name.compare("SEXT")) {
                handleSext(input, name, arg);
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
            if (!name.compare("ST_BT_OFFSET")) {
                handleStBtOffset(input, name, arg);
                continue;
            }
            if (!name.compare("GETELEMPTR")) {
                handleGetelemptr(input, name, arg);
                continue;
            }
            if (!name.compare("GETELEMPTRi")) {
                handleGetelemptri(input, name, arg);
                continue;
            }
            if (!name.compare("ST")) {
                handleSt(input, name, arg);
                continue;
            }
            if (!name.compare("STi")) {
                handleSti(input, name, arg);
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
            if (fnName == "_simRand") {
                return reinterpret_cast<void *>(simRand);
            }
            if (fnName == "_memset") {
                return reinterpret_cast<void *>(llvm::Intrinsic::memset);
            }
            if (fnName == "_simAbs") {
                return reinterpret_cast<void *>(simAbs);
            }
            if (fnName == "_simMax") {
                return reinterpret_cast<void *>(simMax);
            }
            if (fnName == "_simMin") {
                return reinterpret_cast<void *>(simMin);
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
    void handleSimFlush(std::ifstream &input, std::string &name,
                        std::string &arg);
    void handleExit(std::ifstream &input, std::string &name, std::string &arg);

    void handleBrCond(std::ifstream &input, std::string &name,
                      std::string &arg);
    void handleSub(std::ifstream &input, std::string &name, std::string &arg);
    void handleAddi(std::ifstream &input, std::string &name, std::string &arg);
    void handleAlloca(std::ifstream &input, std::string &name,
                      std::string &arg);
    void handleSremi(std::ifstream &input, std::string &name, std::string &arg);
    void handleSext(std::ifstream &input, std::string &name, std::string &arg);
    void handleStBtOffset(std::ifstream &input, std::string &name,
                          std::string &arg);
    void handleGetelemptr(std::ifstream &input, std::string &name,
                          std::string &arg);
    void handleGetelemptri(std::ifstream &input, std::string &name,
                           std::string &arg);
    void handleSt(std::ifstream &input, std::string &name, std::string &arg);
    void handleSti(std::ifstream &input, std::string &name, std::string &arg);
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
    GlobalVariable *arrayPtrStorage;

    Type *voidType;
    Type *int32Type;
    FunctionType *funcType;
    ArrayType *regFileType;

    Function *mainFunc;

    std::unordered_map<std::string, BasicBlock *> BBMap;
};