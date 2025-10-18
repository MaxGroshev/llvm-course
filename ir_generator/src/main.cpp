#include "ir_reader.hpp"
#include "ir_generator.hpp"

int main(int argc, char **argv) {
    irWrapper ir;
    ir.initModule();
    ir.genMainAbility();

    return 0;
}