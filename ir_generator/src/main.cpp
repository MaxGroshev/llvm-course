#include "ir_generator.hpp"

int main(int argc, char **argv) {
    if (argc < 2) {
        llvm::errs() << "ERROR: Not defined IR output\n";
        return 1;
    }
    
    irWrapper ir;
    ir.initTriple();
    ir.genMainAbility();
    ir.dumpIR(argv[1]);

    return 0;
}