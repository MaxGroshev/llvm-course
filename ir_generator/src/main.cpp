#include "ir_reader.hpp"
#include "ir_generator.hpp"

int main(int argc, char **argv) {
    irWrapper ir;
    ir.getFromFile(argc, argv);
    irGenerator irGen;
    irGen.generate(ir);

    return 0;
}