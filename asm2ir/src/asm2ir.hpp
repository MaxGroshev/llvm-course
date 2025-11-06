
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

#include <fstream>
#include <filesystem>
#include <iostream>
#include <unordered_map>

#include "graphic/public_interface.h"

#include "utils/utils.hpp"

using namespace llvm;

class asm2ir {
private:
    static constexpr int REG_FILE_SIZE = 64;

public:
    asm2ir(int argc, char **argv) : builder(context)
    {
        module = std::unique_ptr<Module>(new Module {"top", context});

        initSrcPath(argc, argv);
        initBasicTypes();
        initRegFile();
        initMainFunc();
    }

private:
    void initSrcPath(int argc, char **argv)
    {
        if (argc != 2) {
            outs() << "[ERROR] Need 1 argument: file with assembler code\n";
        }
        asmSrcPath = argv[1];
    }
    void initBasicTypes()
    {
        voidType = Type::getVoidTy(context);
        int32Type = Type::getInt32Ty(context);
        int64Type = Type::getInt64Ty(context);
    }

    void initRegFile()
    {
        regFileType = ArrayType::get(builder.getInt64Ty(), REG_FILE_SIZE);
        regFile = new GlobalVariable(*module, regFileType, false, GlobalValue::PrivateLinkage, 0,
                                     "regFile");
        regFile->setInitializer(ConstantAggregateZero::get(regFileType));
    }

    void initMainFunc()
    {
        auto funcType = FunctionType::get(voidType, false);
        mainFunc = Function::Create(funcType, Function::ExternalLinkage, "main", module.get());
    }

    void openSrcInput()
    {
        input.open(asmSrcPath);
        if (!input.is_open()) {
            outs() << "[ERROR] Can't open " << asmSrcPath << " " << __PRETTY_FUNCTION__ << '\n';
            throw("Can not open file");
        }
    }

public:
    int initBB()
    {
        openSrcInput();
        std::string name;
        std::string arg;

        outs() << "\n#[FILE]:\nBBs:";
        while (input >> name) {
            if (!name.compare("ALLOCA") || !name.compare("SREM") || !name.compare("GETELEMPTRi") ||
                !name.compare("GETELEMPTR") || !name.compare("ADDi") || !name.compare("CMP_EQ") ||
                !name.compare("CMP_SGT") || !name.compare("CMP_SLT") || !name.compare("BR_COND") ||
                !name.compare("AND") || !name.compare("SHL") || !name.compare("PUT_PIXEL") ||
                !name.compare("SIM_MAX") || !name.compare("SIM_MIN") || !name.compare("SUB") ||
                !name.compare("ST_BT_OFFSET")) {
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
            if (!name.compare("GETELEMPTR_2DEMi") || !name.compare("GETELEMPTR_2DEM") ||
                !name.compare("ST") || !name.compare("LD") || !name.compare("STi")) {
                input >> arg >> arg >> arg >> arg >> arg >> arg;
                continue;
            }
            if (!name.compare("SEXT") || !name.compare("MOV") || !name.compare("MOVi") ||
                !name.compare("ZEXT") || !name.compare("TRUNC") || !name.compare("SIM_ABS")) {
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

        input.close();  // Bad make RAII

        return 0;
    }

    void parseInstr()
    {
        openSrcInput();
        std::string name;
        std::string arg;

        while (input >> name) {
            dispathAndHandleInstruction(input, name, arg);
        }

        input.close();  // Bad make RAII
    }

    void verify()
    {
        outs() << "\n#[LLVM IR]:\n";
        module->print(outs(), nullptr);
        outs() << '\n';
        bool verif = verifyFunction(*mainFunc, &outs());
        outs() << "[VERIFICATION] " << (verif ? "FAIL\n\n" : "OK\n\n");
    }

    int runCode()
    {
        outs() << "\n#[Running code]\n";
        InitializeNativeTarget();
        InitializeNativeTargetAsmPrinter();

        ExecutionEngine *ee = EngineBuilder(std::unique_ptr<Module>(module.get())).create();
        ee->InstallLazyFunctionCreator([](const std::string &fnName) -> void * {
            auto convName = converToABIIndependentName(fnName);
            if (convName == "simFlush") {
                return reinterpret_cast<void *>(simFlush);
            }
            if (convName == "simPutPixel") {
                return reinterpret_cast<void *>(simPutPixel);
            }
            if (convName == "simRand") {
                return reinterpret_cast<void *>(simRand);
            }
            if (convName == "simAbs") {
                return reinterpret_cast<void *>(simAbs);
            }
            if (convName == "simMax") {
                return reinterpret_cast<void *>(simMax);
            }
            if (convName == "simMin") {
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
    void dispathAndHandleInstruction(std::ifstream &input, std::string &name, std::string &arg);

    void handlePutPixel(std::ifstream &input, std::string &name, std::string &arg);
    void handleSimRand(std::ifstream &input, std::string &name, std::string &arg);
    void handleSimMax(std::ifstream &input, std::string &name, std::string &arg);
    void handleSimMin(std::ifstream &input, std::string &name, std::string &arg);
    void handleSimAbs(std::ifstream &input, std::string &name, std::string &arg);
    void handleSimFlush(std::ifstream &input, std::string &name, std::string &arg);
    void handleExit(std::ifstream &input, std::string &name, std::string &arg);

    void handleBrCond(std::ifstream &input, std::string &name, std::string &arg);
    void handleSub(std::ifstream &input, std::string &name, std::string &arg);
    void handleAddi(std::ifstream &input, std::string &name, std::string &arg);
    void handleAlloca(std::ifstream &input, std::string &name, std::string &arg);
    void handleSremi(std::ifstream &input, std::string &name, std::string &arg);
    void handleSext(std::ifstream &input, std::string &name, std::string &arg);
    void handleStBtOffset(std::ifstream &input, std::string &name, std::string &arg);
    void handleSt(std::ifstream &input, std::string &name, std::string &arg);
    void handleSti(std::ifstream &input, std::string &name, std::string &arg);
    void handleZext(std::ifstream &input, std::string &name, std::string &arg);
    void handleCmpEq(std::ifstream &input, std::string &name, std::string &arg);
    void handleCmpSlt(std::ifstream &input, std::string &name, std::string &arg);
    void handleCmpSgt(std::ifstream &input, std::string &name, std::string &arg);
    void handleTrunc(std::ifstream &input, std::string &name, std::string &arg);
    void handleBr(std::ifstream &input, std::string &name, std::string &arg);
    void handleAndi(std::ifstream &input, std::string &name, std::string &arg);
    void handleSelect(std::ifstream &input, std::string &name, std::string &arg);
    void handleLd(std::ifstream &input, std::string &name, std::string &arg);
    void handleMov(std::ifstream &input, std::string &name, std::string &arg);
    void handleMovi(std::ifstream &input, std::string &name, std::string &arg);
    void handleShli(std::ifstream &input, std::string &name, std::string &arg);

    std::filesystem::path asmSrcPath;
    std::ifstream input;

    std::unordered_map<std::string, BasicBlock *> BBMap;
    LLVMContext context;
    std::unique_ptr<Module> module;
    IRBuilder<> builder;

    GlobalVariable *regFile;
    Function *mainFunc;

    Type *voidType;
    Type *int32Type;
    Type *int64Type;
    ArrayType *regFileType;
};