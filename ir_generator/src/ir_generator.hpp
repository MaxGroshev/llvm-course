#pragma once

#include <iostream>
#include <memory>
#include <system_error>
#include <filesystem>

#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"

#include "ir_reader.hpp"

class irGenerator {
    public:
    void generate(irWrapper& ir) {
        genTripleInfo(ir);
        // outs() << ir.mod->getTargetTriple();
        for (auto &G : ir.mod->globals()) {
            outs() << "[Global Variable] " << G.getName() << '\n';
        }
        for (auto &F : *ir.mod) {
            outs() << "[Function] " << F. << '\n';
        }
    }

    private:
    void genTripleInfo(irWrapper& ir) {
        outs() << "source_filename = \"" << ir.mod->getSourceFileName() << "\"\n";
        outs() << "target datalayout = \"" << ir.mod->getTargetTriple() << "\"\n";
        outs() << "target triple = \"" << ir.mod->getDataLayoutStr() << "\"\n";
    }
};