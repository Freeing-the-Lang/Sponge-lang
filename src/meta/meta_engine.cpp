#include "meta_engine.hpp"
#include "meta_parser.hpp"
#include "meta_ir.hpp"
#include <stdexcept>
#include <sstream>

namespace sponge {

// ------------------------------------------------------
// 생성자
// ------------------------------------------------------
SpongeMetaEngine::SpongeMetaEngine()
{
    parser = std::make_unique<MetaParser>();
    irbuilder = std::make_unique<IRBuilder>();
}



// ------------------------------------------------------
// 언어 흡수 (기본 absorb)
// ------------------------------------------------------
void SpongeMetaEngine::absorb(
    const std::string& langName,
    const std::unordered_map<std::string, std::string>& tokenRules,
    const std::unordered_map<std::string, int>& precedenceRules,
    const std::unordered_map<std::string, std::function<double(double,double)>>& evalRules
){
    currentLanguage = langName;

    tokenMap = tokenRules;
    precedenceMap = precedenceRules;
    evalMap = evalRules;

    // MetaParser에 규칙을 전달
    parser->rules.clear();
    for (auto& kv : precedenceMap) {
        parser->rules[kv.first] = std::to_string(kv.second);
    }
}



// ------------------------------------------------------
// 확장 absorb (IR/Bytecode까지 흡수)
// ------------------------------------------------------
void SpongeMetaEngine::absorbMetaPack(
    const std::string& langName,
    const std::unordered_map<std::string, std::string>& tokenRules,
    const std::unordered_map<std::string, int>& precedenceRules,
    const std::unordered_map<std::string, std::function<double(double,double)>>& evalRules,
    const std::unordered_map<std::string, std::string>& irRules,
    const std::unordered_map<std::string, std::string>& bytecodeRules
){
    absorb(langName, tokenRules, precedenceRules, evalRules);

    irMap = irRules;
    bcMap = bytecodeRules;
}



// ------------------------------------------------------
// IR 평가기
// ------------------------------------------------------
double SpongeMetaEngine::evaluateIR(const std::shared_ptr<IRNode>& node)
{
    if (!node) throw std::runtime_error("IRNode null");

    // literal
    if (node->op == "literal") {
        return node->literal[0];
    }

    // binary
    if (node->children.size() == 2) {
        auto L = evaluateIR(node->children[0]);
        auto R = evaluateIR(node->children[1]);

        // op에 해당하는 evalRules ("a+b") 적용
        auto it = evalMap.find(node->op);
        if (it != evalMap.end()) {
            return it->second(L, R);
        }

        throw std::runtime_error("Unknown operator: " + node->op);
    }

    throw std::runtime_error("Invalid IR node structure");
}



// ------------------------------------------------------
// run() – 소스 파싱 → IR → 평가
// ------------------------------------------------------
double SpongeMetaEngine::run(const std::string& src)
{
    if (!parser)
        throw std::runtime_error("Parser not initialized");

    // parse → IR
    auto ir = parser->parse(src);

    // evaluate IR
    return evaluateIR(ir);
}



// ------------------------------------------------------
// toGo() – 간단한 IR → Go 코드 변환기
// ------------------------------------------------------
std::string SpongeMetaEngine::toGo(const std::string& src)
{
    auto ir = parser->parse(src);

    std::function<std::string(const std::shared_ptr<IRNode>&)> emit;
    emit = [&](const std::shared_ptr<IRNode>& n)->std::string {

        if (n->op == "literal") {
            return std::to_string(n->literal[0]);
        }

        if (n->children.size() == 2) {
            std::string L = emit(n->children[0]);
            std::string R = emit(n->children[1]);
            return "(" + L + " " + n->op + " " + R + ")";
        }

        return "0";
    };

    std::stringstream out;
    out << "package main\n\n"
        << "import \"fmt\"\n\n"
        << "func main() {\n"
        << "    fmt.Println(" << emit(ir) << ")\n"
        << "}\n";

    return out.str();
}

} // namespace sponge
