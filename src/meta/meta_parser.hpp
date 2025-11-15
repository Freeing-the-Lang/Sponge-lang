#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "meta_ir.hpp"

namespace sponge {

class MetaParser {
public:
    // rule: "Expr": "Term ((+|-) Term)*"
    std::unordered_map<std::string, std::string> rules;

    void addRule(const std::string& head, const std::string& pattern);

    std::shared_ptr<IRNode> parse(const std::string& src);

private:
    std::string input;
    size_t pos;

    char peek();
    char advance();
    double parseNumber();
    std::shared_ptr<IRNode> parseExpr();
    std::shared_ptr<IRNode> parseTerm();
    std::shared_ptr<IRNode> parseFactor();
};

} // namespace sponge
