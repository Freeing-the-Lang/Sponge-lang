#include "meta_vm.hpp"
#include "meta_engine.hpp"

#include <stdexcept>

namespace sponge {

double VM::run(const Bytecode& bc) {
    stack.clear();
    size_t di = 0;

    for (size_t i = 0; i < bc.ops.size(); ++i) {
        switch (bc.ops[i]) {
            case OpCode::PUSH:
                stack.push_back(bc.data[di++]);
                break;
            case OpCode::ADD: {
                double b = stack.back(); stack.pop_back();
                double a = stack.back(); stack.pop_back();
                stack.push_back(a + b);
                break;
            }
            case OpCode::SUB: {
                double b = stack.back(); stack.pop_back();
                double a = stack.back(); stack.pop_back();
                stack.push_back(a - b);
                break;
            }
            case OpCode::MUL: {
                double b = stack.back(); stack.pop_back();
                double a = stack.back(); stack.pop_back();
                stack.push_back(a * b);
                break;
            }
            case OpCode::DIV: {
                double b = stack.back(); stack.pop_back();
                double a = stack.back(); stack.pop_back();
                stack.push_back(a / b);
                break;
            }
            case OpCode::HALT:
                return stack.back();
        }
    }
    throw std::runtime_error("VM halted unexpectedly");
}

} // namespace sponge
