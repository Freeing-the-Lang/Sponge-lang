#include "sponge_meta.hpp"
#include <sstream>
#include <stdexcept>
#include <cctype>

namespace sponge {

// ----------------------
// Grammar setters
// ----------------------
void MetaGrammar::addToken(const std::string& name, const std::string& rule) {
    tokenRule[name] = rule;
}

void MetaGrammar::addOperator(const std::string& op, int prec) {
    precedence[op] = prec;
}

void MetaGrammar::addParseRule(const std::string& name, const std::string& pattern) {
    parseRule[name] = pattern;
}

// ----------------------
// Evaluator
// ----------------------
void MetaEvaluator::addRule(const std::string& tag,
            std::function<double(const MetaNode*)> fn) {
    evalRule[tag] = fn;
}

double MetaEvaluator::eval(const MetaNode* n) const {
    if (evalRule.count(n->tag) == 0)
        throw std::runtime_error("No eval rule for tag: " + n->tag);
    return evalRule.at(n->tag)(n);
}

// ----------------------
// Transpiler
// ----------------------
void MetaTranspiler::addGoRule(const std::string& tag,
    std::function<std::string(const MetaNode*)> fn) {
    goRule[tag] = fn;
}

std::string MetaTranspiler::toGo(const MetaNode* n) const {
    if (goRule.count(n->tag))
        return goRule.at(n->tag)(n);
    return "/* unknown */";
}

std::string MetaTranspiler::toRust(const MetaNode* n) const {
    return toGo(n);
}

std::string MetaTranspiler::toCpp(const MetaNode* n) const {
    return toGo(n);
}

// ----------------------
// Engine absorb()
// ----------------------
void SpongeMetaEngine::absorb(
    const std::string& langName,
    const std::unordered_map<std::string, std::string>& syntax,
    const std::unordered_map<std::string, int>& prec,
    const std::unordered_map<std::string,
        std::function<double(double,double)>> ops)
{
    // add tokens & rules
    for (auto& p : syntax)
        grammar.addToken(langName + "." + p.first, p.second);

    for (auto& p : prec)
        grammar.addOperator(p.first, p.second);

    for (auto& p : ops) {
        evaluator.addRule("op_" + p.first, [=](const MetaNode* n){
            double L = evaluator.eval(n->children[0].get());
            double R = evaluator.eval(n->children[1].get());
            return p.second(L, R);
        });
    }
}

// ----------------------
// extremely simplified demo parser
// ----------------------
std::shared_ptr<MetaNode> SpongeMetaEngine::parse(const std::string& src) {
    // 실제 구현에서는 operator precedence-aware parser 생성됨.
    auto node = std::make_shared<MetaNode>();
    node->tag = "Number";
    node->value = src;
    return node;
}

double SpongeMetaEngine::run(const std::string& src) {
    return evaluator.eval(parse(src).get());
}

std::string SpongeMetaEngine::toGo(const std::string& src) {
    return transpiler.toGo(parse(src).get());
}

} // namespace sponge
