#pragma once

#include <filesystem>
#include <iostream>
#include <memory>
#include <system_error>

#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"

#include "ir_reader.hpp"

class irGenerator {
  public:
    void generate(irWrapper &ir) {
        genTripleInfo(ir);
        // outs() << ir.mod->getTargetTriple();
        for (auto &G : ir.mod->globals()) {
            outs() << "[Global Variable] " << G.getName() << '\n';
        }
        for (auto &F : *ir.mod) {
            genFunction(F);
        }
    }

  private:
    void genTripleInfo(irWrapper &ir) {
        outs() << "source_filename = \"" << ir.mod->getSourceFileName()
               << "\"\n";
        outs() << "target datalayout = \"" << ir.mod->getTargetTriple()
               << "\"\n";
        outs() << "target triple = \"" << ir.mod->getDataLayoutStr() << "\"\n";
    }
    void genFunction(Function &F) {
        outs() << "[Function] " << F.getName() << '\n';
        F.print(llvm::outs());
    }
};