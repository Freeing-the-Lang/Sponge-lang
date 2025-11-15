#pragma once
#include <memory>
#include <string>
#include <vector>

namespace sponge {

struct IRNode {
    std::string op;               // "+", "literal", "call", etc.
    std::vector<double> literal;  // numeric literal
    std::vector<std::shared_ptr<IRNode>> children;
};

class IRBuilder {
public:
    std::shared_ptr<IRNode> literal(double v);
    std::shared_ptr<IRNode> binary(const std::string& op,
            std::shared_ptr<IRNode> L,
            std::shared_ptr<IRNode> R);
};

} // namespace sponge
