#include "./asm2ir.hpp"

int main(int argc, char **argv) {
    if (argc < 2) {
        llvm::errs() << "ERROR: Not defined IR output\n";
        return 1;
    }
    
    asm2ir ir;
    ir.initBB(argc, argv);
    ir.parseInstr(argc, argv);
    ir.verify();
    ir.runCode();

    return 0;
}