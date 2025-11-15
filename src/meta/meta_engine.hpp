#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>
#include <memory>

// ★ 반드시 필요한 include (중요)
#include "meta_parser.hpp"
#include "meta_ir.hpp"

namespace sponge {

class SpongeMetaEngine {
public:
    SpongeMetaEngine();

    void absorb(
        const std::string& langName,
        const std::unordered_map<std::string, std::string>& tokenRules,
        const std::unordered_map<std::string, int>& precedenceRules,
        const std::unordered_map<std::string, std::function<double(double,double)>>& evalRules
    );

    void absorbMetaPack(
        const std::string& langName,
        const std::unordered_map<std::string, std::string>& tokenRules,
        const std::unordered_map<std::string, int>& precedenceRules,
        const std::unordered_map<std::string, std::function<double(double,double)>>& evalRules,
        const std::unordered_map<std::string, std::string>& irRules,
        const std::unordered_map<std::string, std::string>& bytecodeRules
    );

    double run(const std::string& src);

    std::string toGo(const std::string& src);

private:
    std::string currentLanguage;

    std::unordered_map<std::string,std::string> tokenMap;
    std::unordered_map<std::string,int> precedenceMap;
    std::unordered_map<std::string,std::function<double(double,double)>> evalMap;

    std::unordered_map<std::string,std::string> irMap;
    std::unordered_map<std::string,std::string> bcMap;

    // 이제 완전한 타입이므로 unique_ptr OK
    std::unique_ptr<MetaParser> parser;
    std::unique_ptr<IRBuilder> irbuilder;

    double evaluateIR(const std::shared_ptr<IRNode>& node);
};

} // namespace sponge
