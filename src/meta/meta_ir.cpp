#include "meta_ir.hpp"
#include "meta_engine.hpp"

namespace sponge {

std::shared_ptr<IRNode> IRBuilder::literal(double v) {
    auto n = std::make_shared<IRNode>();
    n->op = "literal";
    n->literal = {v};
    return n;
}

std::shared_ptr<IRNode> IRBuilder::binary(const std::string& op,
            std::shared_ptr<IRNode> L,
            std::shared_ptr<IRNode> R)
{
    auto n = std::make_shared<IRNode>();
    n->op = op;
    n->children = {L, R};
    return n;
}

} // namespace sponge
