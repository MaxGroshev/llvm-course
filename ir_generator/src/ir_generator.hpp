#pragma once

#include <iostream>
#include <memory>
#include <system_error>

#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Intrinsics.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/BasicBlock.h>
#include "llvm/Support/raw_ostream.h"

using namespace llvm; 

class irWrapper {
    public:
        void initTriple() {
            mod = std::make_unique<Module>("main_ability.c", context);
            // mod->setTargetTriple("arm64-apple-macosx15.0.0");
            // mod->setDataLayout("e-m:o-i64:64-i128:128-n32:64-S128-Fn32");
        }
        void genBuiltinIntrinsics() {
            Function* smaxFunc = Intrinsic::getOrInsertDeclaration(mod.get(), Intrinsic::smax, {Type::getInt32Ty(context)});
            smaxFunc->setName("llvm.smax.i32");

            if (!mod->getFunction("llvm.lifetime.start.p0")) {
                FunctionType* lifetimeStartType = FunctionType::get(
                    Type::getVoidTy(context),
                    {Type::getInt64Ty(context), PointerType::get(Type::getInt8Ty(context), 0)},
                    false
                );
                Function* lifetimeStartFunc = Function::Create(
                    lifetimeStartType,
                    Function::ExternalLinkage,
                    "llvm.lifetime.start.p0",
                    mod.get()
                );
                lifetimeStartFunc->addParamAttr(0, Attribute::ImmArg);
            }
            
            if (!mod->getFunction("llvm.memset.p0.i64")) {
                FunctionType* memsetType = FunctionType::get(
                    Type::getVoidTy(context),
                    {PointerType::get(Type::getInt8Ty(context), 0), 
                    Type::getInt8Ty(context),
                    Type::getInt64Ty(context),
                    Type::getInt1Ty(context)},
                    false
                );
                Function::Create(memsetType, Function::ExternalLinkage, "llvm.memset.p0.i64", mod.get());
            }
        }
        void genSDLDeclarartions() {
            LLVMContext& context = mod->getContext();
            
            if (!mod->getFunction("simRand")) {
                FunctionType* simRandType = FunctionType::get(
                    Type::getInt32Ty(context), false);
                Function::Create(simRandType, Function::ExternalLinkage, "simRand", mod.get());
            }
            
            if (!mod->getFunction("simAbs")) {
                FunctionType* simAbsType = FunctionType::get(
                    Type::getInt32Ty(context), 
                    {Type::getInt32Ty(context)}, 
                    false);
                Function::Create(simAbsType, Function::ExternalLinkage, "simAbs", mod.get());
            }
            
            if (!mod->getFunction("simPutPixel")) {
                FunctionType* putPixelType = FunctionType::get(
                    Type::getVoidTy(context),
                    {Type::getInt32Ty(context), Type::getInt32Ty(context), Type::getInt32Ty(context)},
                    false);
                Function::Create(putPixelType, Function::ExternalLinkage, "simPutPixel", mod.get());
            }

            if (!mod->getFunction("simMin")) {
                FunctionType* minType = FunctionType::get(
                    Type::getInt32Ty(context),
                    {Type::getInt32Ty(context), Type::getInt32Ty(context)},
                    false);
                Function::Create(minType, Function::ExternalLinkage, "simMin", mod.get());
            }
            
            if (!mod->getFunction("simMax")) {
                FunctionType* maxType = FunctionType::get(
                    Type::getInt32Ty(context),
                    {Type::getInt32Ty(context), Type::getInt32Ty(context)},
                    false);
                Function::Create(maxType, Function::ExternalLinkage, "simMax", mod.get());
            }
            
            if (!mod->getFunction("simFlush")) {
                FunctionType* flushType = FunctionType::get(
                    Type::getVoidTy(context), false);
                Function::Create(flushType, Function::ExternalLinkage, "simFlush", mod.get());
            }
        }
        void gen_getBarrierCenter() {
           FunctionType* funcType = FunctionType::get(
                Type::getInt32Ty(context),
                false
            );
            
            Function* func = Function::Create(
                funcType,
                Function::ExternalLinkage,
                "getBarrierCenter",
                mod.get()
            );
            
            func->addFnAttr(Attribute::get(context, "range", "i32 -245, 246"));
            
            BasicBlock* entryBlock = BasicBlock::Create(context, "", func);
            IRBuilder<> builder(entryBlock);
            
            Function* simRandFunc = mod->getFunction("simRand");

            CallInst* call = builder.CreateCall(simRandFunc, {});
            call->setTailCall(true);
            
            Value* modulus = ConstantInt::get(Type::getInt32Ty(context), 246);
            Value* remainder = builder.CreateSRem(call, modulus);
            
            builder.CreateRet(remainder);
        }

        void gen_generateNewBarrier() {
            FunctionType* funcType = FunctionType::get(
                Type::getInt32Ty(context),
                {
                    Type::getInt32Ty(context),
                    Type::getInt32Ty(context), 
                    PointerType::get(Type::getInt32Ty(context), 0)
                },
                false
            );
            
            Function* func = Function::Create(
                funcType,
                Function::ExternalLinkage,
                "generateNewBarrier",
                mod.get()
            );
            
            func->addFnAttr(Attribute::get(context, "range", "i32 -245, 246"));
            // func->addFnAttr(Attribute::LocalUnnamedAddr);
            
            func->addParamAttr(0, Attribute::NoUndef); 
            func->addParamAttr(1, Attribute::NoUndef); 
            // func->addParamAttr(2, Attribute::NoCapture); // nocapture %2
            func->addParamAttr(2, Attribute::WriteOnly);
            
            BasicBlock* entryBlock = BasicBlock::Create(context, "", func);
            BasicBlock* loopHeader = BasicBlock::Create(context, "12", func);
            BasicBlock* returnBlock = BasicBlock::Create(context, "15", func);
            BasicBlock* loopBody = BasicBlock::Create(context, "16", func);
            BasicBlock* thenBlock = BasicBlock::Create(context, "21", func);
            BasicBlock* loopLatch = BasicBlock::Create(context, "24", func);
            
            IRBuilder<> builder(entryBlock);
            
            auto argIt = func->arg_begin();
            Value* arg0 = &*argIt++; 
            Value* arg1 = &*argIt++;   
            Value* arg2 = &*argIt;   
            
            Value* zext0 = builder.CreateZExt(arg0, Type::getInt64Ty(context), "4");
            
            Function* simRand = mod->getFunction("simRand");
            CallInst* randCall = builder.CreateCall(simRand, {}, "5");
            randCall->setTailCall(true);
            
            Value* srem = builder.CreateSRem(randCall, ConstantInt::get(Type::getInt32Ty(context), 246), "6");
            
            Value* sext0 = builder.CreateSExt(arg0, Type::getInt64Ty(context), "7");
            
            Value* gep1 = builder.CreateGEP(Type::getInt32Ty(context), arg2, sext0, "8");
            
            Value* gep2 = builder.CreateGEP(Type::getInt8Ty(context), gep1, ConstantInt::get(Type::getInt64Ty(context), -8), "9");
            
            Function* smax = mod->getFunction("llvm.smax.i32");
            CallInst* smaxCall = builder.CreateCall(smax, {arg1, ConstantInt::get(Type::getInt32Ty(context), 0)}, "10");
            smaxCall->setTailCall(true);
            
            Value* zextSmax = builder.CreateZExt(smaxCall, Type::getInt64Ty(context), "11", true);

            builder.CreateBr(loopHeader);
            
            builder.SetInsertPoint(loopHeader);
            
            PHINode* phi = builder.CreatePHI(Type::getInt64Ty(context), 2, "13");
            phi->addIncoming(ConstantInt::get(Type::getInt64Ty(context), 0), entryBlock);
            
            Value* cmp = builder.CreateICmpEQ(phi, zextSmax, "14");
            
            builder.CreateCondBr(cmp, returnBlock, loopBody);
            
            builder.SetInsertPoint(returnBlock);
            builder.CreateRet(srem);
            
            builder.SetInsertPoint(loopBody);
            
            Value* trunc = builder.CreateTrunc(phi, Type::getInt32Ty(context), "17");
            
            Value* sub = builder.CreateSub(srem, trunc, "18");
            
            Function* simAbs = mod->getFunction("simAbs");
            CallInst* absCall = builder.CreateCall(simAbs, {sub}, "19");
            absCall->setTailCall(true);
            absCall->addParamAttr(0, Attribute::NoUndef);
            
            Value* cmp2 = builder.CreateICmpSGT(absCall, ConstantInt::get(Type::getInt32Ty(context), 35), "20");
            
            builder.CreateCondBr(cmp2, thenBlock, loopLatch);
            
            builder.SetInsertPoint(thenBlock);
            
            Value* mul = builder.CreateNUWMul(phi, zext0, "22");
            
            Value* gep3 = builder.CreateGEP(Type::getInt32Ty(context), gep2, mul, "23");
            
            builder.CreateStore(ConstantInt::get(Type::getInt32Ty(context), 7522568), gep3);
            
            builder.CreateBr(loopLatch);
            
            builder.SetInsertPoint(loopLatch);
            
            Value* add = builder.CreateAdd(phi, ConstantInt::get(Type::getInt64Ty(context), 1), "25", true, true);
            
            BranchInst* branch = builder.CreateBr(loopHeader);
            
            phi->addIncoming(add, loopLatch);
        }

        void gen_app() {
                        
            // Создаем тип функции: i32()
            FunctionType* funcType = FunctionType::get(
                Type::getInt32Ty(context),
                // false,
                false
            );
            
            // Создаем функцию app
            Function* func = Function::Create(
                funcType,
                Function::ExternalLinkage,
                "app",
                mod.get()
            );
            
            // Добавляем атрибуты функции
            // func->addFnAttr(Attribute::NoUndef);
            // func->addFnAttr(Attribute::LocalUnnamedAddr);
            
            BasicBlock* block0  = BasicBlock::Create(context, "", func);
            BasicBlock* block6  = BasicBlock::Create(context, "6", func);
            BasicBlock* block10 = BasicBlock::Create(context, "10", func);
            BasicBlock* block13 = BasicBlock::Create(context, "13", func);
            BasicBlock* block16 = BasicBlock::Create(context, "16", func);
            BasicBlock* block19 = BasicBlock::Create(context, "19", func);
            BasicBlock* block22 = BasicBlock::Create(context, "22", func);
            BasicBlock* block24 = BasicBlock::Create(context, "24", func);
            BasicBlock* block31 = BasicBlock::Create(context, "31", func);
            BasicBlock* block33 = BasicBlock::Create(context, "33", func);
            BasicBlock* block37 = BasicBlock::Create(context, "37", func);
            BasicBlock* block41 = BasicBlock::Create(context, "41", func);
            BasicBlock* block46 = BasicBlock::Create(context, "46", func);
            BasicBlock* block49 = BasicBlock::Create(context, "49", func);
            BasicBlock* block52 = BasicBlock::Create(context, "52", func);
            
            IRBuilder<> builder(block0);
            
            ArrayType* innerArrayType = ArrayType::get(Type::getInt32Ty(context), 512);
            ArrayType* outerArrayType = ArrayType::get(innerArrayType, 256);
            Value* alloca = builder.CreateAlloca(outerArrayType, nullptr, "1");
            // alloca->setAlignment(Align(4));
            
            // call void @llvm.lifetime.start.p0(i64 524288, ptr nonnull %1) #7
            Function* lifetimeStart = mod->getFunction("llvm.lifetime.start.p0");
            Value* bitcast = builder.CreateBitCast(alloca, PointerType::get(Type::getInt8Ty(context), 0));
            CallInst* lifetimeCall = builder.CreateCall(lifetimeStart, {
                ConstantInt::get(Type::getInt64Ty(context), 524288),
                bitcast
            });
            
            // call void @llvm.memset.p0.i64(ptr noundef nonnull align 4 dereferenceable(524288) %1, i8 0, i64 524288, i1 false)
            Function* memset = mod->getFunction("llvm.memset.p0.i64");
            CallInst* memsetCall = builder.CreateCall(memset, {
                bitcast,
                ConstantInt::get(Type::getInt8Ty(context), 0),
                ConstantInt::get(Type::getInt64Ty(context), 524288),
                ConstantInt::get(Type::getInt1Ty(context), false)
            });
            
            Function* simRand = mod->getFunction("simRand");
            CallInst* randCall = builder.CreateCall(simRand, {}, "2");
            randCall->setTailCall(true);
            
            Value* srem = builder.CreateSRem(randCall, ConstantInt::get(Type::getInt32Ty(context), 246), "3");
            
            Value* sext = builder.CreateSExt(srem, Type::getInt64Ty(context), "4");
            
            Value* gep = builder.CreateInBoundsGEP(outerArrayType, alloca, {
                ConstantInt::get(Type::getInt64Ty(context), 0),
                sext,
                ConstantInt::get(Type::getInt64Ty(context), 255)
            }, "5");
            
            builder.CreateStore(ConstantInt::get(Type::getInt32Ty(context), 16763144), gep);
            
            builder.CreateBr(block6);
            
            builder.SetInsertPoint(block6);
            PHINode* phi7 = builder.CreatePHI(Type::getInt32Ty(context), 2, "7");
            PHINode* phi8 = builder.CreatePHI(Type::getInt32Ty(context), 2, "8");
            
            Value* zext = builder.CreateZExt(phi7, Type::getInt64Ty(context), "9");
            
            builder.CreateBr(block10);
            
            builder.SetInsertPoint(block10);
            PHINode* phi11 = builder.CreatePHI(Type::getInt64Ty(context), 2, "11");
            
            Value* cmp12 = builder.CreateICmpEQ(phi11, ConstantInt::get(Type::getInt64Ty(context), 256), "12");
            
            builder.CreateCondBr(cmp12, block16, block13);
            
            builder.SetInsertPoint(block13);
            
            Value* cmp14 = builder.CreateICmpEQ(phi11, zext, "14");
            
            Value* trunc15 = builder.CreateTrunc(phi11, Type::getInt32Ty(context), "15", true, true);
            
            builder.CreateBr(block19);
            
            builder.SetInsertPoint(block16);
            
            Value* and17 = builder.CreateAnd(phi8, ConstantInt::get(Type::getInt32Ty(context), 127), "17");
            
            Value* cmp18 = builder.CreateICmpEQ(and17, ConstantInt::get(Type::getInt32Ty(context), 0), "18");
            
            builder.CreateCondBr(cmp18, block37, block52);
            
            builder.SetInsertPoint(block19);
            PHINode* phi20 = builder.CreatePHI(Type::getInt64Ty(context), 2, "20");
            
            Value* cmp21 = builder.CreateICmpEQ(phi20, ConstantInt::get(Type::getInt64Ty(context), 511), "21");
            
            builder.CreateCondBr(cmp21, block22, block24);
            
            builder.SetInsertPoint(block22);
            
            Value* add23 = builder.CreateAdd(phi11, ConstantInt::get(Type::getInt64Ty(context), 1), "23", true, true);
            
            builder.CreateBr(block10);
            
            builder.SetInsertPoint(block24);
            
            Value* gep25 = builder.CreateInBoundsGEP(outerArrayType, alloca, {
                ConstantInt::get(Type::getInt64Ty(context), 0),
                phi11,
                phi20
            }, "25");
            Value* load26 = builder.CreateLoad(Type::getInt32Ty(context), gep25, "26");          
            Value* trunc27 = builder.CreateTrunc(phi20, Type::getInt32Ty(context), "27");            

            Value* add28 = builder.CreateAdd(trunc27, ConstantInt::get(Type::getInt32Ty(context), -1), "28");
            
            // tail call void @simPutPixel(i32 noundef %28, i32 noundef %15, i32 noundef %26) #6
            Function* simPutPixel = mod->getFunction("simPutPixel");
            CallInst* putPixelCall = builder.CreateCall(simPutPixel, {add28, trunc15, load26});
            putPixelCall->setTailCall(true);
            putPixelCall->addParamAttr(0, Attribute::NoUndef);
            putPixelCall->addParamAttr(1, Attribute::NoUndef);
            putPixelCall->addParamAttr(2, Attribute::NoUndef);
            
            Value* cmp29 = builder.CreateICmpEQ(phi20, ConstantInt::get(Type::getInt64Ty(context), 447), "29");            
            Value* select30 = builder.CreateSelect(cmp29, cmp14, ConstantInt::get(Type::getInt1Ty(context), false), "30");            
            builder.CreateCondBr(select30, block31, block33);
            
            builder.SetInsertPoint(block31);
            
            PHINode* phi32 = builder.CreatePHI(Type::getInt64Ty(context), 2, "32");
            
            builder.CreateBr(block19);
            
            builder.SetInsertPoint(block33);
            
            Value* add34 = builder.CreateAdd(phi20, ConstantInt::get(Type::getInt64Ty(context), 1), "34", true, true);
            
            Value* gep35 = builder.CreateInBoundsGEP(outerArrayType, alloca, {
                ConstantInt::get(Type::getInt64Ty(context), 0),
                phi11,
                add34
            }, "35");
            
            Value* load36 = builder.CreateLoad(Type::getInt32Ty(context), gep35, "36");
            builder.CreateStore(load36, gep25);
            
            builder.CreateBr(block31);
            
            builder.SetInsertPoint(block37);
            
            Function* generateNewBarrier = mod->getFunction("generateNewBarrier");
            CallInst* barrierCall = builder.CreateCall(generateNewBarrier, {
                ConstantInt::get(Type::getInt32Ty(context), 512),
                ConstantInt::get(Type::getInt32Ty(context), 256),
                alloca
            }, "38");
            barrierCall->addParamAttr(0, Attribute::NoUndef);
            barrierCall->addParamAttr(1, Attribute::NoUndef);
            barrierCall->addParamAttr(2, Attribute::NoUndef);
            
            Function* simMin = mod->getFunction("simMin");
            CallInst* minCall = builder.CreateCall(simMin, {phi7, barrierCall}, "39");
            minCall->setTailCall(true);
            minCall->addParamAttr(0, Attribute::NoUndef);
            minCall->addParamAttr(1, Attribute::NoUndef);
            
            Value* sext40 = builder.CreateSExt(minCall, Type::getInt64Ty(context), "40");
            
            builder.CreateBr(block41);
            
            builder.SetInsertPoint(block41);
            PHINode* phi42 = builder.CreatePHI(Type::getInt64Ty(context), 0, "42");
            
            Function* simMax = mod->getFunction("simMax");
            CallInst* maxCall = builder.CreateCall(simMax, {phi7, barrierCall}, "43");
            maxCall->setTailCall(true);
            maxCall->addParamAttr(0, Attribute::NoUndef);
            maxCall->addParamAttr(1, Attribute::NoUndef);
            
            Value* sext44 = builder.CreateSExt(maxCall, Type::getInt64Ty(context), "44");
            
            Value* cmp45 = builder.CreateICmpSLT(phi42, sext44, "45");
            
            builder.CreateCondBr(cmp45, block49, block46);
            
            builder.SetInsertPoint(block46);
            
            Value* sext47 = builder.CreateSExt(barrierCall, Type::getInt64Ty(context), "47");
            
            // %48 = getelementptr inbounds [256 x [512 x i32]], ptr %1, i64 0, i64 %47, i64 447
            Value* gep48 = builder.CreateInBoundsGEP(outerArrayType, alloca, {
                ConstantInt::get(Type::getInt64Ty(context), 0),
                sext47,
                ConstantInt::get(Type::getInt64Ty(context), 447)
            }, "48");
            
            builder.CreateStore(ConstantInt::get(Type::getInt32Ty(context), 16763144), gep48);
            
            builder.CreateBr(block52);
            
            builder.SetInsertPoint(block49);
            
            Value* gep50 = builder.CreateInBoundsGEP(outerArrayType, alloca, {
                ConstantInt::get(Type::getInt64Ty(context), 0),
                phi42,
                ConstantInt::get(Type::getInt64Ty(context), 447)
            }, "50");
            
            builder.CreateStore(ConstantInt::get(Type::getInt32Ty(context), 16763144), gep50);
            
            Value* add51 = builder.CreateNSWAdd(phi42, ConstantInt::get(Type::getInt64Ty(context), 1), "51");
            
            builder.CreateBr(block41);
            
            builder.SetInsertPoint(block52);
            
            PHINode* phi53 = builder.CreatePHI(Type::getInt32Ty(context), 2, "53");
            
            Function* simFlush = mod->getFunction("simFlush");
            CallInst* flushCall = builder.CreateCall(simFlush, {});
            flushCall->setTailCall(true);
            
            Value* add54 = builder.CreateAdd(phi8, ConstantInt::get(Type::getInt32Ty(context), 1), "54", true, true);
            
            builder.CreateBr(block6);
            
            phi7->addIncoming(srem, block0);
            phi7->addIncoming(phi53, block52);
            
            phi8->addIncoming(ConstantInt::get(Type::getInt32Ty(context), 0), block0);
            phi8->addIncoming(add54, block52);
            
            phi11->addIncoming(ConstantInt::get(Type::getInt64Ty(context), 0), block6);
            phi11->addIncoming(add23, block22);
            
            phi20->addIncoming(ConstantInt::get(Type::getInt64Ty(context), 1), block13);
            phi20->addIncoming(phi32, block31);
            
            phi32->addIncoming(ConstantInt::get(Type::getInt64Ty(context), 448), block24);
            phi32->addIncoming(add34, block33);
            
            phi42->addIncoming(sext40, block37);
            phi42->addIncoming(add51, block49);
            
            phi53->addIncoming(barrierCall, block46);
            phi53->addIncoming(phi7, block16);
    
        }

        void dumpIR(const char* fileName) {
            std::error_code ec;
            llvm::raw_fd_ostream file(fileName, ec);
            
            if (!ec) {
                mod->print(file, nullptr);
            } else {
                llvm::errs() << "Error opening file: " << ec.message() << "\n";
            }
            // mod->print(llvm::outs(), nullptr);
        }
        void genMainAbility() {
            genBuiltinIntrinsics();
            genSDLDeclarartions();
            gen_getBarrierCenter();
            gen_generateNewBarrier();
            gen_app();
        }
    
        SMDiagnostic errDiag;
        LLVMContext context;
        std::unique_ptr<Module> mod;
};