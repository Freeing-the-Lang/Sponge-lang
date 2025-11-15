#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>
#include <memory>

namespace sponge {

// 전방 선언
struct IRNode;
class MetaParser;
class IRBuilder;

/**
 * SpongeMetaEngine
 * ----------------------------
 * Sponge-lang 메타 시스템의 중추 엔진.
 *
 * - absorb(language, tokens, precedence, evalOps)
 *      언어 스펙(meta pack)을 현재 엔진에 흡수
 *
 * - run(src)
 *      흡수된 문법으로 파싱 → IR → 평가 → 결과 반환
 *
 * - toGo(src), toRust(src)
 *      IR을 다른 언어 코드로 변환하는 트랜스파일러 훅
 *
 * 이 엔진은 '언어를 먹는 언어'의 핵심 구조를 담당한다.
 */
class SpongeMetaEngine {
public:
    SpongeMetaEngine();

    // -------------------------
    // 언어 흡수
    // -------------------------
    void absorb(
        const std::string& langName,
        const std::unordered_map<std::string, std::string>& tokenRules,
        const std::unordered_map<std::string, int>& precedenceRules,
        const std::unordered_map<std::string, std::function<double(double,double)>>& evalRules
    );

    // -------------------------
    // 실행
    // -------------------------
    double run(const std::string& src);

    // -------------------------
    // IR → Go 변환 (예시)
    // -------------------------
    std::string toGo(const std::string& src);

    // -------------------------
    // 언어 "흡수" 고급 버전 (메타팩: tokens + precedence + evalOps)
    // IR 및 Bytecode 매핑도 향후 여기로 확장 가능
    // -------------------------
    void absorbMetaPack(
        const std::string& langName,
        const std::unordered_map<std::string, std::string>& tokenRules,
        const std::unordered_map<std::string, int>& precedenceRules,
        const std::unordered_map<std::string, std::function<double(double,double)>>& evalRules,
        const std::unordered_map<std::string, std::string>& irRules,
        const std::unordered_map<std::string, std::string>& bytecodeRules
    );

private:
    // 현재 활성 언어 이름
    std::string currentLanguage;

    // 규칙들
    std::unordered_map<std::string,std::string> tokenMap;
    std::unordered_map<std::string,int> precedenceMap;
    std::unordered_map<std::string,std::function<double(double,double)>> evalMap;

    // optional - IR/Bytecode 규칙
    std::unordered_map<std::string,std::string> irMap;
    std::unordered_map<std::string,std::string> bcMap;

    // 내부 파서/IR 빌더 포인터
    std::unique_ptr<MetaParser> parser;
    std::unique_ptr<IRBuilder> irbuilder;

    // 평가용 내부 함수
    double evaluateIR(const std::shared_ptr<IRNode>& node);
};

} // namespace sponge
