#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
using namespace llvm;

struct MyTracePass : public PassInfoMixin<MyTracePass> {
    Type *voidType;
    Type *int8PtrTy;

    bool isFuncLogger(StringRef name) {
        return name == "funcLogUses";
    }

    bool insertUsersLog(Module &M, Function &F, IRBuilder<> &builder) {
        // Prepare funcLogUses function
        ArrayRef<Type *> LogUsesParamTypes = {int8PtrTy, int8PtrTy};
        FunctionType *LogUsesFuncType =
            FunctionType::get(voidType, LogUsesParamTypes, false);
        FunctionCallee funcLogUsesFunc =
            M.getOrInsertFunction("funcLogUses", LogUsesFuncType);

        bool Inserted = false;
        for (auto &BB : F) {
            std::vector<Instruction*> instr;
            for (auto &I : BB) {
                instr.push_back(&I);
            }

            for(auto *I : instr) {
                if(isa<PHINode>(I)) {
                    continue;
                }
                auto insertPlace = ++BasicBlock::iterator(I);
                for(auto &U : I->uses()) {
                    User *user = U.getUser();
                    if (Instruction *userInstr = dyn_cast<Instruction>(user)) {
                        builder.SetInsertPoint(&BB, insertPlace);
    
                        Value *IName = builder.CreateGlobalStringPtr(I->getOpcodeName());
                        Value *UName = builder.CreateGlobalStringPtr(userInstr->getOpcodeName());
                        Value *args[] = {IName, UName};
                        auto *insertInstr = builder.CreateCall(funcLogUsesFunc, args);
                        // insertInstr->addFnAttr(Attribute::NoInline);
                        insertPlace = ++BasicBlock::iterator(insertInstr);
                    }
                }
                Inserted = true;
            }
        }
        return Inserted;
    }

    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
        outs() << "[Module] " << M.getName() << '\n';

        LLVMContext &Ctx = M.getContext();
        IRBuilder<> builder(Ctx);
        voidType = Type::getVoidTy(Ctx);
        int8PtrTy = Type::getInt8Ty(Ctx)->getPointerTo();

        for (auto &F : M) {
            if (isFuncLogger(F.getName()) || F.isDeclaration()) {
                continue;
            }
            if (insertUsersLog(M, F,builder)) {
                outs() << "insertUsersLog done\n";
            }

            bool verif = verifyFunction(F, &outs());
            outs() << "[VERIFICATION] " << (verif ? "FAIL\n\n" : "OK\n\n");
        }
        outs() << '\n';
        return PreservedAnalyses::none();
    };
};

PassPluginLibraryInfo getPassPluginInfo() {
    const auto callback = [](PassBuilder &PB) {
        // PB.registerOptimizerLastEPCallback([](ModulePassManager &MPM, auto) {
        PB.registerPipelineStartEPCallback([](ModulePassManager &MPM, auto) {
            MPM.addPass(MyTracePass{});
            return true;
        });
    };

    return {LLVM_PLUGIN_API_VERSION, "MyPlugin", "0.0.1", callback};
};

/* When a plugin is loaded by the driver, it will call this entry point to
obtain information about this plugin and about how to register its passes.
*/
extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return getPassPluginInfo();
}