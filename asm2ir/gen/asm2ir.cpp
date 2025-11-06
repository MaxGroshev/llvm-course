#include "./asm2ir.hpp"
#include <cassert>

void asm2ir::handlePutPixel(std::ifstream &input, std::string &name,
                            std::string &arg) {
    ArrayRef<Type *> simPutPixelParamTypes = {int32Type, int32Type, int32Type};
    FunctionType *simPutPixelType =
        FunctionType::get(voidType, simPutPixelParamTypes, false);
    FunctionCallee simPutPixelFunc =
        module->getOrInsertFunction("simPutPixel", simPutPixelType);

    input >> arg;
    outs() << "\tPUT_PIXEL " << arg;
    Value *arg1 = builder.CreateLoad(
        int32Type, builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                              std::stoi(arg.substr(1))));
    input >> arg;
    outs() << " " << arg;
    Value *arg2 = builder.CreateLoad(
        int32Type, builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                              std::stoi(arg.substr(1))));
    input >> arg;
    outs() << " " << arg << '\n';
    Value *arg3 = builder.CreateLoad(
        int32Type, builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                              std::stoi(arg.substr(1))));
    Value *args[] = {arg1, arg2, arg3};
    builder.CreateCall(simPutPixelFunc, args);
}

void asm2ir::handleSimMax(std::ifstream &input, std::string &name,
                          std::string &arg) {
    ArrayRef<Type *> simMaxParamTypes = {int32Type, int32Type};
    FunctionType *simMaxType =
        FunctionType::get(int32Type, simMaxParamTypes, false);
    FunctionCallee simMaxFunc =
        module->getOrInsertFunction("simMax", simMaxType);

    input >> arg;
    outs() << "\tsimMax " << arg;
    Value *res = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                            std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " " << arg;
    Value *arg1 = builder.CreateLoad(
        int32Type, builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                              std::stoi(arg.substr(1))));
    input >> arg;
    outs() << " " << arg;
    Value *arg2 = builder.CreateLoad(
        int32Type, builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                              std::stoi(arg.substr(1))));

    Value *args[] = {arg1, arg2};
    auto call = builder.CreateCall(simMaxFunc, args);
    builder.CreateStore(call, res);
}

void asm2ir::handleSimMin(std::ifstream &input, std::string &name,
                          std::string &arg) {
    ArrayRef<Type *> simMinParamTypes = {int32Type, int32Type};
    FunctionType *simMinType =
        FunctionType::get(int32Type, simMinParamTypes, false);
    FunctionCallee simMinFunc =
        module->getOrInsertFunction("simMin", simMinType);

    input >> arg;
    outs() << "\tsimMin " << arg;
    Value *res = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                            std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " " << arg;
    Value *arg1 = builder.CreateLoad(
        int32Type, builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                              std::stoi(arg.substr(1))));
    input >> arg;
    outs() << " " << arg;
    Value *arg2 = builder.CreateLoad(
        int32Type, builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                              std::stoi(arg.substr(1))));

    Value *args[] = {arg1, arg2};
    auto call = builder.CreateCall(simMinFunc, args);
    builder.CreateStore(call, res);
}

void asm2ir::handleSimAbs(std::ifstream &input, std::string &name,
                          std::string &arg) {
    ArrayRef<Type *> simAbsParamTypes = {int32Type};
    FunctionType *simAbsType =
        FunctionType::get(int32Type, simAbsParamTypes, false);
    FunctionCallee simAbsFunc =
        module->getOrInsertFunction("simAbs", simAbsType);

    input >> arg;
    outs() << "\tsimAbs " << arg;
    Value *res = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                            std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " " << arg;
    Value *arg1 = builder.CreateLoad(
        int32Type, builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                              std::stoi(arg.substr(1))));

    Value *args[] = {arg1};
    auto call = builder.CreateCall(simAbsFunc, args);
    builder.CreateStore(call, res);
}

void asm2ir::handleSimRand(std::ifstream &input, std::string &name,
                           std::string &arg) {
    FunctionType *simRandType = FunctionType::get(int32Type, false);
    FunctionCallee simRandFunc =
        module->getOrInsertFunction("simRand", simRandType);

    input >> arg;
    outs() << "\tsimRand " << arg;
    Value *res = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                            std::stoi(arg.substr(1)));

    auto call = builder.CreateCall(simRandFunc);
    builder.CreateStore(call, res);
}

void asm2ir::handleSimFlush(std::ifstream &input, std::string &name,
                            std::string &arg) {
    FunctionType *simFlushType = FunctionType::get(voidType, false);
    FunctionCallee simFlushFunc =
        module->getOrInsertFunction("simFlush", simFlushType);

    outs() << "\tFLUSH\n";
    builder.CreateCall(simFlushFunc);
}

void asm2ir::handleExit(std::ifstream &input, std::string &name,
                        std::string &arg) {
    outs() << "\tEXIT\n";
    builder.CreateRetVoid();
    if (input >> name) {
        outs() << "BB " << name << '\n';
        builder.SetInsertPoint(BBMap[name]);
    }
}

void asm2ir::handleBrCond(std::ifstream &input, std::string &name,
                          std::string &arg) {
    input >> arg;
    outs() << "\tif (" << arg;
    // reg1
    Value *reg_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                              std::stoi(arg.substr(1)));
    Value *reg_i1 = builder.CreateTrunc(builder.CreateLoad(int32Type, reg_p),
                                        builder.getInt1Ty());
    input >> arg;
    outs() << ") then BB:" << arg;
    input >> name;
    outs() << " else BB:" << name << '\n';
    // outs() << "BB " << name << '\n';
    builder.CreateCondBr(reg_i1, BBMap[arg], BBMap[name]);
    // builder.SetInsertPoint(BBMap[name]);
}

void asm2ir::handleSub(std::ifstream &input, std::string &name,
                       std::string &arg) {
    input >> arg;
    outs() << "\t" << arg;
    Value *res_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                              std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " = " << arg;
    Value *arg1_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                               std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " - " << arg << '\n';
    Value *arg2 = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                             std::stoi(arg.substr(1)));
    Value *sub_arg1_arg2 =
        builder.CreateSub(builder.CreateLoad(int32Type, arg1_p),
                          builder.CreateLoad(int32Type, arg2));
    builder.CreateStore(sub_arg1_arg2, res_p);
}

void asm2ir::handleAddi(std::ifstream &input, std::string &name,
                        std::string &arg) {
    input >> arg;
    outs() << "\t" << arg;
    // res
    Value *res_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                              std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " = " << arg;
    // arg1
    Value *arg1_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                               std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " + " << arg << '\n';
    // arg2
    Value *arg2 = builder.getInt32(std::stoi(arg));
    Value *add_arg1_arg2 =
        builder.CreateAdd(builder.CreateLoad(int32Type, arg1_p), arg2);
    builder.CreateStore(add_arg1_arg2, res_p);
}

void asm2ir::handleAlloca(std::ifstream &input, std::string &name,
                          std::string &arg) {
    input >> arg;
    outs() << "\tAlloca: " << arg;
    // res
    Value *res = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                              std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " " << arg;
    // arg1
    auto outer = std::stoi(arg);
    input >> arg;
    outs() << " * " << arg << '\n';
    // arg2
    auto inner = std::stoi(arg);
    ArrayType* innerArrayType = ArrayType::get(Type::getInt32Ty(context), inner); 
    ArrayType* outerArrayType = ArrayType::get(innerArrayType,outer);


    auto arrPtr = builder.CreateAlloca(outerArrayType,  nullptr);
    // Value* zeroArray = ConstantAggregateZero::get(outerArrayType);
    // Value* zeroInit = ConstantAggregateZero::get(outerArrayType);
    // builder.CreateStore(zeroInit, arrPtr);
    // arrPtr->setInitializer(
    //         ConstantAggregateZero::get(regFileType));
    builder.CreateStore(builder.CreatePtrToInt(arrPtr, builder.getInt64Ty()), res);
}

void asm2ir::handleStBtOffset(std::ifstream &input, std::string &name,
                              std::string &arg) {

    input >> arg;
    outs() << "\tst by offset: " << arg << " ";
    auto val = builder.getInt32(std::stoi(arg));
    input >> arg;
    outs() << " * " << arg;
    Value *src = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                            std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " * " << arg;
    auto off2 = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                           std::stoi(arg.substr(1)));
    
    ArrayType* innerArrayType = ArrayType::get(Type::getInt32Ty(context), 512); 
    ArrayType* outerArrayType = ArrayType::get(innerArrayType,256);

    auto mem = builder.CreateIntToPtr(builder.CreateLoad(builder.getInt64Ty(), src), builder.getPtrTy());
    llvm::Value *gep = builder.CreateGEP(builder.getInt8Ty(), mem,
                                         builder.CreateLoad(int32Type, off2));
    builder.CreateStore(val, gep);
}

void asm2ir::handleSt(std::ifstream &input, std::string &name,
                       std::string &arg) {

    input >> arg;
    outs() << "\tsti: " << arg << " ";
    auto val = builder.CreateLoad(
        int32Type, builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                              std::stoi(arg.substr(1))));
    input >> arg;
    outs() << arg;
    auto outer = std::stoi(arg);
    input >> arg;
    outs() << " * " << arg;
    auto inner = std::stoi(arg);

    ArrayType *innerArrayType =
        ArrayType::get(Type::getInt32Ty(context), inner);
    ArrayType *outerArrayType = ArrayType::get(innerArrayType, outer);

    input >> arg;
    outs() << " * " << arg;
    Value *src = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                            std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " * " << arg;
    auto off2 = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                           std::stoi(arg.substr(1)));

    input >> arg;
    outs() << " * " << arg;
    auto off3 = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                           std::stoi(arg.substr(1)));
                                        
    auto mem = builder.CreateIntToPtr(builder.CreateLoad(builder.getInt64Ty(), src), builder.getPtrTy());
    Value *gep = builder.CreateInBoundsGEP(
        outerArrayType, mem,
        {ConstantInt::get(Type::getInt32Ty(context), 0),
         builder.CreateLoad(int32Type, off2),
         builder.CreateLoad(int32Type, off3)});
    builder.CreateStore(val, gep);
}

void asm2ir::handleSti(std::ifstream &input, std::string &name,
                      std::string &arg) {

    input >> arg;
    outs() << "\tst: " << arg << " ";
    auto val = builder.getInt32(std::stoi(arg));
    input >> arg;
    outs() << arg;
    auto outer = std::stoi(arg);
    input >> arg;
    outs() << " * " << arg;
    auto inner = std::stoi(arg);

    ArrayType *innerArrayType =
        ArrayType::get(Type::getInt32Ty(context), inner);
    ArrayType *outerArrayType = ArrayType::get(innerArrayType, outer);

    input >> arg;
    outs() << " * " << arg;
    Value *src = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                            std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " * " << arg;
    auto off2 = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                           std::stoi(arg.substr(1)));

    input >> arg;
    outs() << " * " << arg;
    auto off3 = ConstantInt::get(Type::getInt32Ty(context), std::stoi(arg));

    auto mem = builder.CreateIntToPtr(builder.CreateLoad(builder.getInt64Ty(), src), builder.getPtrTy());
    Value *gep = builder.CreateInBoundsGEP(
        outerArrayType, mem,
        {ConstantInt::get(Type::getInt32Ty(context), 0),
         builder.CreateLoad(int32Type, off2), off3});
    builder.CreateStore(val, gep);
}

void asm2ir::handleLd(std::ifstream &input, std::string &name,
                      std::string &arg) {

    input >> arg;
    outs() << "\tld: " << arg << " ";
    auto dst = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                          std::stoi(arg.substr(1)));
    input >> arg;
    outs() << arg;
    auto outer = std::stoi(arg);
    input >> arg;
    outs() << " * " << arg;
    auto inner = std::stoi(arg);

    ArrayType *innerArrayType =
        ArrayType::get(Type::getInt32Ty(context), inner);
    ArrayType *outerArrayType = ArrayType::get(innerArrayType, outer);

    input >> arg;
    outs() << " * " << arg;
    Value *src = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                            std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " * " << arg;
    auto off2 = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                           std::stoi(arg.substr(1)));

    input >> arg;
    outs() << " * " << arg;
    auto off3 = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                           std::stoi(arg.substr(1)));

    auto mem = builder.CreateIntToPtr(builder.CreateLoad(builder.getInt64Ty(), src), builder.getPtrTy());
    Value *gep = builder.CreateInBoundsGEP(
        outerArrayType, mem,
        {ConstantInt::get(Type::getInt32Ty(context), 0),
         builder.CreateLoad(int32Type, off2),
         builder.CreateLoad(int32Type, off3)});
    builder.CreateStore(builder.CreateLoad(int32Type, gep), dst);
}

void asm2ir::handleSremi(std::ifstream &input, std::string &name,
                         std::string &arg) {
    input >> arg;
    outs() << "\t" << arg;
    // res
    Value *res_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                              std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " = " << arg;
    // arg1
    Value *arg1_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                               std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " srem " << arg << '\n';
    // arg2
    Value *arg2 = builder.getInt32(std::stoi(arg));
    Value *srem_arg1_arg2 =
        builder.CreateSRem(builder.CreateLoad(int32Type, arg1_p), arg2);
    builder.CreateStore(srem_arg1_arg2, res_p);
}

void asm2ir::handleSext(std::ifstream &input, std::string &name,
                        std::string &arg) {
    input >> arg;
    outs() << "\tsext: " << arg;
    // res
    Value *res = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                            std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " = " << arg;
    // arg1
    Value *arg1 = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                             std::stoi(arg.substr(1)));
    auto int64Type = Type::getInt64Ty(context);

    Value *sextRes =
        builder.CreateSExt(builder.CreateLoad(int32Type, arg1), int64Type);
    builder.CreateStore(sextRes, res);
}

void asm2ir::handleZext(std::ifstream &input, std::string &name,
                        std::string &arg) {
    input >> arg;
    outs() << "\t" << arg;
    // res
    Value *res = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                            std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " = " << arg;
    // arg1
    Value *arg1 = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                             std::stoi(arg.substr(1)));
    auto int64Type = Type::getInt64Ty(context);

    Value *sextRes =
        builder.CreateZExt(builder.CreateLoad(int32Type, arg1), int64Type);
    builder.CreateStore(sextRes, res);
}

void asm2ir::handleCmpEq(std::ifstream &input, std::string &name,
                         std::string &arg) {
    input >> arg;
    outs() << "\t" << arg;
    // res
    Value *res = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                            std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " = " << arg;
    // arg1
    Value *arg1 = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                             std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " cmp " << arg << '\n';
    // arg2
    if (arg.find("x") == std::string::npos) {
        Value *arg2 = builder.getInt32(std::stoi(arg));
        Value *cmp_arg1_arg2 =
            builder.CreateICmpEQ(builder.CreateLoad(int32Type, arg1), arg2);
        builder.CreateStore(cmp_arg1_arg2, res);
        return;
    }
    Value *arg2 = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                             std::stoi(arg.substr(1)));
    Value *cmp_arg1_arg2 =
        builder.CreateICmpEQ(builder.CreateLoad(int32Type, arg1),
                             builder.CreateLoad(int32Type, arg2));

    builder.CreateStore(cmp_arg1_arg2, res);
}

void asm2ir::handleCmpSlt(std::ifstream &input, std::string &name,
                          std::string &arg) {
    input >> arg;
    outs() << "\t" << arg;
    // res
    Value *res = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                            std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " = " << arg;
    // arg1
    Value *arg1 = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                             std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " cmp " << arg << '\n';

    Value *arg2 = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                             std::stoi(arg.substr(1)));
    Value *cmp_arg1_arg2 =
        builder.CreateICmpSLT(builder.CreateLoad(int32Type, arg1),
                              builder.CreateLoad(int32Type, arg2));
    builder.CreateStore(cmp_arg1_arg2, res);
}

void asm2ir::handleCmpSgt(std::ifstream &input, std::string &name,
                          std::string &arg) {
    input >> arg;
    outs() << "\t" << arg;
    // res
    Value *res = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                            std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " = " << arg;
    // arg1
    Value *arg1 = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                             std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " cmp " << arg << '\n';
    // arg2
    Value *arg2 = builder.getInt32(std::stoi(arg));
    Value *cmp_arg1_arg2 =
        builder.CreateICmpSGT(builder.CreateLoad(int32Type, arg1), arg2);
    builder.CreateStore(cmp_arg1_arg2, res);
}

void asm2ir::handleTrunc(std::ifstream &input, std::string &name,
                         std::string &arg) {
    input >> arg;
    outs() << "\tTrunc: " << arg << " ";
    // res
    Value *res = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                            std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " = " << arg;
    // arg1
    Value *arg1 = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                             std::stoi(arg.substr(1)));

    Value *truncRes =
        builder.CreateTrunc(builder.CreateLoad(int32Type, arg1), int32Type);
    builder.CreateStore(truncRes, res);
}

void asm2ir::handleBr(std::ifstream &input, std::string &name,
                      std::string &arg) {
    input >> name;
    outs() << "\tBr: " << name << " ";
    builder.CreateBr(BBMap[name]);
}

void asm2ir::handleAndi(std::ifstream &input, std::string &name,
                        std::string &arg) {
    input >> arg;
    outs() << "\t" << arg;
    // res
    Value *res_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                              std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " = " << arg;
    // arg1
    Value *arg1_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                               std::stoi(arg.substr(1)));

    input >> arg;
    outs() << " & " << arg << '\n';
    // arg2
    Value *arg2 = builder.getInt32(std::stoi(arg));
    Value *and_arg1_arg2 =
        builder.CreateAnd(builder.CreateLoad(int32Type, arg1_p), arg2);
    builder.CreateStore(and_arg1_arg2, res_p);
}

void asm2ir::handleSelect(std::ifstream &input, std::string &name,
                          std::string &arg) {
    input >> arg;
    outs() << "\tSelect:" << arg;
    // res
    Value *res_p = builder.CreateConstInBoundsGEP2_32(regFileType, regFile, 0,
                                                      std::stoi(arg.substr(1)));

    input >> arg;
    outs() << " = " << arg;
    // arg1
    Value *arg1 = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                             std::stoi(arg.substr(1)));
    Value *arg1_i1 = builder.CreateTrunc(builder.CreateLoad(int32Type, arg1),
                                         builder.getInt1Ty());
    input >> arg;
    outs() << " select " << arg << ' ';

    Value *arg2 = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                             std::stoi(arg.substr(1)));
    Value *arg2_i1 = builder.CreateTrunc(builder.CreateLoad(int32Type, arg2),
                                         builder.getInt1Ty());
    input >> arg;
    outs() << arg;
    Value *arg3 = builder.getInt1(std::stoi(arg));
    Value *select_arg1_arg2_arg3 = builder.CreateSelect(arg1_i1, arg2_i1, arg3);
    builder.CreateStore(select_arg1_arg2_arg3, res_p);
}

void asm2ir::handleMov(std::ifstream &input, std::string &name,
                       std::string &arg) {
    input >> arg;
    outs() << "\t" << arg;
    // res
    Value *res_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                              std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " <- " << arg;
    // arg1
    Value *from = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                             std::stoi(arg.substr(1)));

    builder.CreateStore(builder.CreateLoad(int32Type, from), res_p);
}

void asm2ir::handleMovi(std::ifstream &input, std::string &name,
                        std::string &arg) {
    input >> arg;
    outs() << "\t" << arg;
    // res
    Value *res_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                              std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " <- " << arg;
    // arg1
    Value *val = builder.getInt32(std::stoi(arg));

    builder.CreateStore(val, res_p);
}

void asm2ir::handleShli(std::ifstream &input, std::string &name,
                        std::string &arg) {
    input >> arg;
    outs() << "\t" << arg;
    // res
    Value *res_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                              std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " = " << arg;
    // arg1
    Value *arg1_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                               std::stoi(arg.substr(1)));
    input >> arg;
    outs() << " shl " << arg << '\n';
    // arg2
    Value *arg2 = builder.getInt32(std::stoi(arg));
    Value *shl_arg1_arg2 =
        builder.CreateShl(builder.CreateLoad(int32Type, arg1_p), arg2);
    builder.CreateStore(shl_arg1_arg2, res_p);
}