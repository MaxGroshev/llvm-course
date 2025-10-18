#pragma once

#include <iostream>
#include <memory>
#include <system_error>

#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
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
        void initModule() {
            mod = std::make_unique<Module>("main_ability.c", context);
            mod->setTargetTriple("arm64-apple-macosx15.0.0");
            mod->setDataLayout("e-m:o-i64:64-i128:128-n32:64-S128-Fn32");
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
                // lifetimeStartFunc->addParamAttr(1, Attribute::Noca);
            }
            
            // llvm.memset.p0.i64
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
            
            // simMax - функция для нахождения максимума
            if (!mod->getFunction("simMax")) {
                FunctionType* maxType = FunctionType::get(
                    Type::getInt32Ty(context),
                    {Type::getInt32Ty(context), Type::getInt32Ty(context)},
                    false);
                Function::Create(maxType, Function::ExternalLinkage, "simMax", mod.get());
            }
            
            // simFlush - функция для обновления экрана
            if (!mod->getFunction("simFlush")) {
                FunctionType* flushType = FunctionType::get(
                    Type::getVoidTy(context), false);
                Function::Create(flushType, Function::ExternalLinkage, "simFlush", mod.get());
            }
        }
        void gen_getBarrierCenter() { //TODO remove (...) before @simRand()
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
            
    // tail call void @simFlush() #6
    // Function* simFlush = mod->getFunction("simFlush");
    // CallInst* flushCall = builder.CreateCall(simFlush, {});
    // flushCall->setTailCall(true);

            CallInst* call = builder.CreateCall(simRandFunc, {});
            call->setTailCall(true);
            
            Value* modulus = ConstantInt::get(Type::getInt32Ty(context), 246);
            Value* remainder = builder.CreateSRem(call, modulus);
            
            builder.CreateRet(remainder);
        }

        void gen_generateNewBarrier() {
            FunctionType* funcType = FunctionType::get(
                Type::getInt32Ty(context), // возвращаемый тип
                {
                    Type::getInt32Ty(context), // %0 - i32
                    Type::getInt32Ty(context), // %1 - i32  
                    PointerType::get(Type::getInt32Ty(context), 0) // %2 - ptr
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
            
            func->addParamAttr(0, Attribute::NoUndef);  // noundef %0
            func->addParamAttr(1, Attribute::NoUndef);  // noundef %1
            // func->addParamAttr(2, Attribute::NoCapture); // nocapture %2
            func->addParamAttr(2, Attribute::WriteOnly); // writeonly %2
            
            BasicBlock* entryBlock = BasicBlock::Create(context, "", func);
            BasicBlock* loopHeader = BasicBlock::Create(context, "12", func);
            BasicBlock* returnBlock = BasicBlock::Create(context, "15", func);
            BasicBlock* loopBody = BasicBlock::Create(context, "16", func);
            BasicBlock* thenBlock = BasicBlock::Create(context, "21", func);
            BasicBlock* loopLatch = BasicBlock::Create(context, "24", func);
            
            IRBuilder<> builder(entryBlock);
            
            auto argIt = func->arg_begin();
            Value* arg0 = &*argIt++; // %0
            Value* arg1 = &*argIt++; // %1  
            Value* arg2 = &*argIt;   // %2
            
            // 8. Генерируем код для entry блока (%3)
            // %4 = zext i32 %0 to i64
            Value* zext0 = builder.CreateZExt(arg0, Type::getInt64Ty(context), "4");
            
            // %5 = tail call i32 @simRand()
            Function* simRand = mod->getFunction("simRand");
            CallInst* randCall = builder.CreateCall(simRand, {});
            randCall->setTailCall(true);
            
            // %6 = srem i32 %5, 246
            Value* srem = builder.CreateSRem(randCall, ConstantInt::get(Type::getInt32Ty(context), 246), "6");
            
            // %7 = sext i32 %0 to i64
            Value* sext0 = builder.CreateSExt(arg0, Type::getInt64Ty(context), "7");
            
            // %8 = getelementptr i32, ptr %2, i64 %7
            Value* gep1 = builder.CreateGEP(Type::getInt32Ty(context), arg2, sext0, "8");
            
            // %9 = getelementptr i8, ptr %8, i64 -8
            Value* gep2 = builder.CreateGEP(Type::getInt8Ty(context), gep1, ConstantInt::get(Type::getInt64Ty(context), -8), "9");
            
            // %10 = tail call i32 @llvm.smax.i32(i32 %1, i32 0)
            Function* smax = mod->getFunction("llvm.smax.i32");
            CallInst* smaxCall = builder.CreateCall(smax, {arg1, ConstantInt::get(Type::getInt32Ty(context), 0)}, "10");
            smaxCall->setTailCall(true);
            
            // %11 = zext nneg i32 %10 to i64
            Value* zextSmax = builder.CreateZExt(smaxCall, Type::getInt64Ty(context), "11");
            
            // br label %12
            builder.CreateBr(loopHeader);
            
            // 9. Генерируем код для loop header (%12)
            builder.SetInsertPoint(loopHeader);
            
            // phi узел для индукционной переменной
            PHINode* phi = builder.CreatePHI(Type::getInt64Ty(context), 2, "13");
            phi->addIncoming(ConstantInt::get(Type::getInt64Ty(context), 0), entryBlock);
            
            // %14 = icmp eq i64 %13, %11
            Value* cmp = builder.CreateICmpEQ(phi, zextSmax, "14");
            
            // br i1 %14, label %15, label %16
            builder.CreateCondBr(cmp, returnBlock, loopBody);
            
            // 10. Генерируем код для return block (%15)
            builder.SetInsertPoint(returnBlock);
            builder.CreateRet(srem); // ret i32 %6
            
            // 11. Генерируем код для loop body (%16)
            builder.SetInsertPoint(loopBody);
            
            // %17 = trunc i64 %13 to i32
            Value* trunc = builder.CreateTrunc(phi, Type::getInt32Ty(context), "17");
            
            // %18 = sub i32 %6, %17
            Value* sub = builder.CreateSub(srem, trunc, "18");
            
            // %19 = tail call i32 @simAbs(i32 noundef %18)
            Function* simAbs = mod->getFunction("simAbs");
            CallInst* absCall = builder.CreateCall(simAbs, {sub}, "19");
            absCall->setTailCall(true);
            absCall->addParamAttr(0, Attribute::NoUndef);
            
            // %20 = icmp sgt i32 %19, 35
            Value* cmp2 = builder.CreateICmpSGT(absCall, ConstantInt::get(Type::getInt32Ty(context), 35), "20");
            
            // br i1 %20, label %21, label %24
            builder.CreateCondBr(cmp2, thenBlock, loopLatch);
            
            // 12. Генерируем код для then block (%21)
            builder.SetInsertPoint(thenBlock);
            
            // %22 = mul nuw nsw i64 %13, %4
            Value* mul = builder.CreateNUWMul(phi, zext0, "22");
            
            // %23 = getelementptr i32, ptr %9, i64 %22
            Value* gep3 = builder.CreateGEP(Type::getInt32Ty(context), gep2, mul, "23");
            
            // store i32 7522568, ptr %23, align 4, !tbaa !6
            builder.CreateStore(ConstantInt::get(Type::getInt32Ty(context), 7522568), gep3);
            
            // br label %24
            builder.CreateBr(loopLatch);
            
            // 13. Генерируем код для loop latch (%24)
            builder.SetInsertPoint(loopLatch);
            
            // %25 = add nuw nsw i64 %13, 1
            Value* add = builder.CreateNUWAdd(phi, ConstantInt::get(Type::getInt64Ty(context), 1), "25");
            
            // br label %12, !llvm.loop !10
            BranchInst* branch = builder.CreateBr(loopHeader);
            
            // Завершаем phi узел
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
            func->addFnAttr(Attribute::NoUndef);
            // func->addFnAttr(Attribute::LocalUnnamedAddr);
            
            // Создаем базовые блоки
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
            
            // Блок 0
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
            
            // %2 = tail call i32 @simRand() #6
            Function* simRand = mod->getFunction("simRand");
            CallInst* randCall = builder.CreateCall(simRand, {});
            randCall->setTailCall(true);
            
            // %3 = srem i32 %2, 246
            Value* srem = builder.CreateSRem(randCall, ConstantInt::get(Type::getInt32Ty(context), 246), "3");
            
            // %4 = sext i32 %3 to i64
            Value* sext = builder.CreateSExt(srem, Type::getInt64Ty(context), "4");
            
            // %5 = getelementptr inbounds [256 x [512 x i32]], ptr %1, i64 0, i64 %4, i64 255
            Value* gep = builder.CreateInBoundsGEP(outerArrayType, alloca, {
                ConstantInt::get(Type::getInt64Ty(context), 0),
                sext,
                ConstantInt::get(Type::getInt64Ty(context), 255)
            }, "5");
            
            // store i32 16763144, ptr %5, align 4, !tbaa !6
            builder.CreateStore(ConstantInt::get(Type::getInt32Ty(context), 16763144), gep);
            
            // br label %6
            builder.CreateBr(block6);
            
            // Блок 6
            builder.SetInsertPoint(block6);
            PHINode* phi7 = builder.CreatePHI(Type::getInt32Ty(context), 2, "7");
            PHINode* phi8 = builder.CreatePHI(Type::getInt32Ty(context), 2, "8");
            
            // %9 = zext i32 %7 to i64
            Value* zext = builder.CreateZExt(phi7, Type::getInt64Ty(context), "9");
            
            // br label %10
            builder.CreateBr(block10);
            
            // Блок 10
            builder.SetInsertPoint(block10);
            PHINode* phi11 = builder.CreatePHI(Type::getInt64Ty(context), 2, "11");
            
            // %12 = icmp eq i64 %11, 256
            Value* cmp12 = builder.CreateICmpEQ(phi11, ConstantInt::get(Type::getInt64Ty(context), 256), "12");
            
            // br i1 %12, label %16, label %13
            builder.CreateCondBr(cmp12, block16, block13);
            
            // Блок 13
            builder.SetInsertPoint(block13);
            
            // %14 = icmp eq i64 %11, %9
            Value* cmp14 = builder.CreateICmpEQ(phi11, zext, "14");
            
            // %15 = trunc nuw nsw i64 %11 to i32
            Value* trunc15 = builder.CreateTrunc(phi11, Type::getInt32Ty(context), "15");
            
            // br label %19
            builder.CreateBr(block19);
            
            // Блок 16
            builder.SetInsertPoint(block16);
            
            // %17 = and i32 %8, 127
            Value* and17 = builder.CreateAnd(phi8, ConstantInt::get(Type::getInt32Ty(context), 127), "17");
            
            // %18 = icmp eq i32 %17, 0
            Value* cmp18 = builder.CreateICmpEQ(and17, ConstantInt::get(Type::getInt32Ty(context), 0), "18");
            
            // br i1 %18, label %37, label %52
            builder.CreateCondBr(cmp18, block37, block52);
            
            // Блок 19
            builder.SetInsertPoint(block19);
            PHINode* phi20 = builder.CreatePHI(Type::getInt64Ty(context), 2, "20");
            
            // %21 = icmp eq i64 %20, 511
            Value* cmp21 = builder.CreateICmpEQ(phi20, ConstantInt::get(Type::getInt64Ty(context), 511), "21");
            
            // br i1 %21, label %22, label %24
            builder.CreateCondBr(cmp21, block22, block24);
            
            // Блок 22
            builder.SetInsertPoint(block22);
            
            // %23 = add nuw nsw i64 %11, 1
            Value* add23 = builder.CreateNUWAdd(phi11, ConstantInt::get(Type::getInt64Ty(context), 1), "23");
            
            // br label %10, !llvm.loop !12
            builder.CreateBr(block10);
            
            // Блок 24
            builder.SetInsertPoint(block24);
            
            // %25 = getelementptr inbounds [256 x [512 x i32]], ptr %1, i64 0, i64 %11, i64 %20
            Value* gep25 = builder.CreateInBoundsGEP(outerArrayType, alloca, {
                ConstantInt::get(Type::getInt64Ty(context), 0),
                phi11,
                phi20
            }, "25");
            
            // %26 = load i32, ptr %25, align 4, !tbaa !6
            Value* load26 = builder.CreateLoad(Type::getInt32Ty(context), gep25, "26");
            
            // %27 = trunc i64 %20 to i32
            Value* trunc27 = builder.CreateTrunc(phi20, Type::getInt32Ty(context), "27");
            
            // %28 = add i32 %27, -1
            Value* add28 = builder.CreateAdd(trunc27, ConstantInt::get(Type::getInt32Ty(context), -1), "28");
            
            // tail call void @simPutPixel(i32 noundef %28, i32 noundef %15, i32 noundef %26) #6
            Function* simPutPixel = mod->getFunction("simPutPixel");
            CallInst* putPixelCall = builder.CreateCall(simPutPixel, {add28, trunc15, load26});
            putPixelCall->setTailCall(true);
            putPixelCall->addParamAttr(0, Attribute::NoUndef);
            putPixelCall->addParamAttr(1, Attribute::NoUndef);
            putPixelCall->addParamAttr(2, Attribute::NoUndef);
            
            // %29 = icmp eq i64 %20, 447
            Value* cmp29 = builder.CreateICmpEQ(phi20, ConstantInt::get(Type::getInt64Ty(context), 447), "29");
            
            // %30 = select i1 %29, i1 %14, i1 false
            Value* select30 = builder.CreateSelect(cmp29, cmp14, ConstantInt::get(Type::getInt1Ty(context), false), "30");
            
            // br i1 %30, label %31, label %33
            builder.CreateCondBr(select30, block31, block33);
            
            // Блок 31
            builder.SetInsertPoint(block31);
            
            // %32 = phi i64 [ %34, %33 ], [ 448, %24 ]
            PHINode* phi32 = builder.CreatePHI(Type::getInt64Ty(context), 2, "32");
            
            // br label %19, !llvm.loop !13
            builder.CreateBr(block19);
            
            // Блок 33
            builder.SetInsertPoint(block33);
            
            // %34 = add nuw nsw i64 %20, 1
            Value* add34 = builder.CreateNUWAdd(phi20, ConstantInt::get(Type::getInt64Ty(context), 1), "34");
            
            // %35 = getelementptr inbounds [256 x [512 x i32]], ptr %1, i64 0, i64 %11, i64 %34
            Value* gep35 = builder.CreateInBoundsGEP(outerArrayType, alloca, {
                ConstantInt::get(Type::getInt64Ty(context), 0),
                phi11,
                add34
            }, "35");
            
            // %36 = load i32, ptr %35, align 4, !tbaa !6
            Value* load36 = builder.CreateLoad(Type::getInt32Ty(context), gep35, "36");
            
            // store i32 %36, ptr %25, align 4, !tbaa !6
            builder.CreateStore(load36, gep25);
            
            // br label %31
            builder.CreateBr(block31);
            
            // Блок 37
            builder.SetInsertPoint(block37);
            
            // %38 = call i32 @generateNewBarrier(i32 noundef 512, i32 noundef 256, ptr noundef nonnull %1) #8
            Function* generateNewBarrier = mod->getFunction("generateNewBarrier");
            CallInst* barrierCall = builder.CreateCall(generateNewBarrier, {
                ConstantInt::get(Type::getInt32Ty(context), 512),
                ConstantInt::get(Type::getInt32Ty(context), 256),
                alloca
            }, "38");
            barrierCall->addParamAttr(0, Attribute::NoUndef);
            barrierCall->addParamAttr(1, Attribute::NoUndef);
            barrierCall->addParamAttr(2, Attribute::NoUndef);
            
            // %39 = tail call i32 @simMin(i32 noundef %7, i32 noundef %38) #6
            Function* simMin = mod->getFunction("simMin");
            CallInst* minCall = builder.CreateCall(simMin, {phi7, barrierCall}, "39");
            minCall->setTailCall(true);
            minCall->addParamAttr(0, Attribute::NoUndef);
            minCall->addParamAttr(1, Attribute::NoUndef);
            
            // %40 = sext i32 %39 to i64
            Value* sext40 = builder.CreateSExt(minCall, Type::getInt64Ty(context), "40");
            
            // br label %41
            builder.CreateBr(block41);
            
            // Блок 41
            builder.SetInsertPoint(block41);
            PHINode* phi42 = builder.CreatePHI(Type::getInt64Ty(context), 2, "42");
            
            // %43 = tail call i32 @simMax(i32 noundef %7, i32 noundef %38) #6
            Function* simMax = mod->getFunction("simMax");
            CallInst* maxCall = builder.CreateCall(simMax, {phi7, barrierCall}, "43");
            maxCall->setTailCall(true);
            maxCall->addParamAttr(0, Attribute::NoUndef);
            maxCall->addParamAttr(1, Attribute::NoUndef);
            
            // %44 = sext i32 %43 to i64
            Value* sext44 = builder.CreateSExt(maxCall, Type::getInt64Ty(context), "44");
            
            // %45 = icmp slt i64 %42, %44
            Value* cmp45 = builder.CreateICmpSLT(phi42, sext44, "45");
            
            // br i1 %45, label %49, label %46
            builder.CreateCondBr(cmp45, block49, block46);
            
            // Блок 46
            builder.SetInsertPoint(block46);
            
            // %47 = sext i32 %38 to i64
            Value* sext47 = builder.CreateSExt(barrierCall, Type::getInt64Ty(context), "47");
            
            // %48 = getelementptr inbounds [256 x [512 x i32]], ptr %1, i64 0, i64 %47, i64 447
            Value* gep48 = builder.CreateInBoundsGEP(outerArrayType, alloca, {
                ConstantInt::get(Type::getInt64Ty(context), 0),
                sext47,
                ConstantInt::get(Type::getInt64Ty(context), 447)
            }, "48");
            
            // store i32 16763144, ptr %48, align 4, !tbaa !6
            builder.CreateStore(ConstantInt::get(Type::getInt32Ty(context), 16763144), gep48);
            
            // br label %52
            builder.CreateBr(block52);
            
            // Блок 49
            builder.SetInsertPoint(block49);
            
            // %50 = getelementptr inbounds [256 x [512 x i32]], ptr %1, i64 0, i64 %42, i64 447
            Value* gep50 = builder.CreateInBoundsGEP(outerArrayType, alloca, {
                ConstantInt::get(Type::getInt64Ty(context), 0),
                phi42,
                ConstantInt::get(Type::getInt64Ty(context), 447)
            }, "50");
            
            // store i32 16763144, ptr %50, align 4, !tbaa !6
            builder.CreateStore(ConstantInt::get(Type::getInt32Ty(context), 16763144), gep50);
            
            // %51 = add nsw i64 %42, 1
            Value* add51 = builder.CreateNSWAdd(phi42, ConstantInt::get(Type::getInt64Ty(context), 1), "51");
            
            // br label %41, !llvm.loop !14
            builder.CreateBr(block41);
            
            // Блок 52
            builder.SetInsertPoint(block52);
            
            // %53 = phi i32 [ %38, %46 ], [ %7, %16 ]
            PHINode* phi53 = builder.CreatePHI(Type::getInt32Ty(context), 2, "53");
            
            // tail call void @simFlush() #6
            Function* simFlush = mod->getFunction("simFlush");
            CallInst* flushCall = builder.CreateCall(simFlush, {});
            flushCall->setTailCall(true);
            
            // %54 = add nuw nsw i32 %8, 1
            Value* add54 = builder.CreateNUWAdd(phi8, ConstantInt::get(Type::getInt32Ty(context), 1), "54");
            
            // br label %6
            builder.CreateBr(block6);
            
            // Завершаем PHI-узлы
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
        void genMainAbility() {
            genBuiltinIntrinsics();
            genSDLDeclarartions();
            gen_getBarrierCenter();
            gen_generateNewBarrier();
            gen_app();
        }

        ~irWrapper() {
            mod->print(llvm::outs(), nullptr);
        }
    
        SMDiagnostic errDiag;
        LLVMContext context;
        std::unique_ptr<Module> mod;
};