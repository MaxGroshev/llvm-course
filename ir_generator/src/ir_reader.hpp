#pragma once

#include <iostream>
#include <memory>
#include <system_error>

#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"

using namespace llvm; //FIXME

class irWrapper {
    public:
    void getFromFile(int argc, char **argv) {
        if (argc < 2) {
            errs() << "Expected at least one argument - IR file name\n";
            throw("Incorrect arg num");
        }
        mod = parseIRFile(argv[1], errDiag, context);
        
        if (!mod) {
            errDiag.print(argv[0], errs());
            throw("Can not init module");
        }
    }
    
    SMDiagnostic errDiag;
    LLVMContext context;
    std::unique_ptr<Module> mod;
};