#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>

namespace sponge {

struct MetaNode {
    // Node type is dynamically determined by rules
    std::string tag; 
    std::vector<std::shared_ptr<MetaNode>> children;
    std::string value;
};

class MetaGrammar {
public:
    // token → regex-like pattern (simplified)
    std::unordered_map<std::string, std::string> tokenRule;

    // operator precedence (auto-generated)
    std::unordered_map<std::string, int> precedence;

    // parse rule: name → pattern (e.g., "Expr": "Term ((+|-) Term)*")
    std::unordered_map<std::string, std::string> parseRule;

    // Add / Extend grammar
    void addToken(const std::string& name, const std::string& rule);
    void addOperator(const std::string& op, int prec);
    void addParseRule(const std::string& name, const std::string& pattern);
};

class MetaEvaluator {
public:
    // evaluator rule: node.tag → function
    std::unordered_map<std::string, std::function<double(const MetaNode*)>> evalRule;

    void addRule(const std::string& tag,
                 std::function<double(const MetaNode*)> fn);

    double eval(const MetaNode* n) const;
};

class MetaTranspiler {
public:
    // tag → generator
    std::unordered_map<std::string, std::function<std::string(const MetaNode*)>> goRule;
    std::unordered_map<std::string, std::function<std::string(const MetaNode*)>> rustRule;
    std::unordered_map<std::string, std::function<std::string(const MetaNode*)>> cppRule;

    void addGoRule(const std::string& tag,
                   std::function<std::string(const MetaNode*)> fn);

    std::string toGo(const MetaNode* n) const;
    std::string toRust(const MetaNode* n) const;
    std::string toCpp(const MetaNode* n) const;
};

// -----------------------------
// Sponge Meta-Engine
// -----------------------------
class SpongeMetaEngine {
public:
    MetaGrammar grammar;
    MetaEvaluator evaluator;
    MetaTranspiler transpiler;

    // absorb external language fragment
    void absorb(const std::string& langName,
                const std::unordered_map<std::string, std::string>& syntax,
                const std::unordered_map<std::string, int>& prec,
                const std::unordered_map<std::string,
                    std::function<double(double,double)>> ops);

    // parse & evaluate & transpile
    std::shared_ptr<MetaNode> parse(const std::string& src);
    double run(const std::string& src);
    std::string toGo(const std::string& src);
};

} // namespace sponge
