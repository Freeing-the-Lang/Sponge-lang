#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <cstdint>

namespace sponge {

enum class OpCode : uint8_t {
    PUSH,      // push literal
    ADD, SUB, MUL, DIV,
    HALT
};

struct Bytecode {
    std::vector<OpCode> ops;
    std::vector<double> data;
};

class VM {
public:
    double run(const Bytecode& bc);

private:
    std::vector<double> stack;
};

} // namespace sponge
