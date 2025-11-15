#include "meta_parser.hpp"
#include <cctype>
#include <stdexcept>

namespace sponge {

void MetaParser::addRule(const std::string& head, const std::string& pattern) {
    rules[head] = pattern;
}

char MetaParser::peek() { return input[pos]; }
char MetaParser::advance() { return input[pos++]; }

double MetaParser::parseNumber() {
    size_t start = pos;
    while (isdigit(peek())) advance();
    return std::stod(input.substr(start, pos - start));
}

std::shared_ptr<IRNode> MetaParser::parseFactor() {
    if (isdigit(peek()))
        return IRBuilder().literal(parseNumber());
    throw std::runtime_error("factor parse error");
}

std::shared_ptr<IRNode> MetaParser::parseTerm() {
    auto n = parseFactor();
    while (peek() == '*' || peek() == '/') {
        char op = advance();
        auto r = parseFactor();
        n = IRBuilder().binary(std::string(1,op), n, r);
    }
    return n;
}

std::shared_ptr<IRNode> MetaParser::parseExpr() {
    auto n = parseTerm();
    while (peek() == '+' || peek() == '-') {
        char op = advance();
        auto r = parseTerm();
        n = IRBuilder().binary(std::string(1,op), n, r);
    }
    return n;
}

std::shared_ptr<IRNode> MetaParser::parse(const std::string& src) {
    input = src;
    pos = 0;
    return parseExpr();
}

} // namespace sponge
