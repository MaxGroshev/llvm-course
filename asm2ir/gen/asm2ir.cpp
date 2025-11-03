#include "./asm2ir.hpp"

void asm2ir::handlePutPixel(std::ifstream& input , std::string& name, std::string& arg) {
    ArrayRef<Type *> simPutPixelParamTypes = {int32Type, int32Type, int32Type};
    FunctionType *simPutPixelType =
        FunctionType::get(voidType, simPutPixelParamTypes, false);
    FunctionCallee simPutPixelFunc =
        module->getOrInsertFunction("simPutPixel", simPutPixelType);
    
    input >> arg;
    outs() << "\tPUT_PIXEL " << arg;
    Value *arg1 = builder.CreateLoad(
        int32Type,
        builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                std::stoi(arg.substr(1))));
    input >> arg;
    outs() << " " << arg;
    Value *arg2 = builder.CreateLoad(
        int32Type,
        builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                std::stoi(arg.substr(1))));
    input >> arg;
    outs() << " " << arg << '\n';
    Value *arg3 = builder.CreateLoad(
        int32Type,
        builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                std::stoi(arg.substr(1))));
    Value *args[] = {arg1, arg2, arg3};
    builder.CreateCall(simPutPixelFunc, args);
}

void asm2ir::handleBrCond(std::ifstream& input , std::string& name, std::string& arg) {
    input >> arg;
    outs() << "\tif (" << arg;
    // reg1
    Value *reg_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                            std::stoi(arg.substr(1)));
    Value *reg_i1 = builder.CreateTrunc(
        builder.CreateLoad(int32Type, reg_p), builder.getInt1Ty());
    input >> arg;
    outs() << ") then BB:" << arg;
    input >> name;
    outs() << " else BB:" << name << '\n';
    outs() << "BB " << name << '\n';
    builder.CreateCondBr(reg_i1, BBMap[arg], BBMap[name]);
    builder.SetInsertPoint(BBMap[name]);
}