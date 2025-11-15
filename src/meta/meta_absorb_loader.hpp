#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>

namespace sponge {

class SpongeMetaEngine;

/**
 * 언어 스펙(.meta) 파일을 파싱해서 SpongeMetaEngine 이
 * 흡수할 수 있는 LangPack 형식으로 변환하는 로더.
 *
 * meta.meta 스키마에 맞는 키만 인식하도록 설계되어 있어
 * 확장 가능한 메타 흡수팩 구조를 지원한다.
 */
class MetaAbsorbLoader {
public:

    /**
     * 하나의 언어 정의를 담는 구조체:
     *
     * language: "rust" 등
     * tokens:   number → "[0-9]+"
     * precedence:  + → 10, * → 20
     * evalRules:   "+" → "a + b" 와 같은 문자열 기반 평가식
     * bytecode:    "+" → "ADD"
     */
    struct LangPack {
        std::string name;

        std::unordered_map<std::string, std::string> tokens;      // tokenName → regex
        std::unordered_map<std::string, int> precedence;          // op → precedence
        std::unordered_map<std::string, std::string> evalRules;   // op → "a + b"
        std::unordered_map<std::string, std::string> irRules;     // IR tag mapping
        std::unordered_map<std::string, std::string> bytecode;    // op → opcode
    };

    /**
     * .meta 파일을 읽어서 LangPack 구조로 파싱한다.
     * meta.meta 스키마는 Meta2Processor 단계에서 보정됨.
     */
    LangPack loadFromFile(const std::string& path);

    /**
     * LangPack을 SpongeMetaEngine에 실제로 적용한다.
     * absorb()를 호출해서 토큰/우선순위/평가규칙/바이트코드 규칙을 반영.
     */
    void mount(SpongeMetaEngine& eng, const LangPack& pack);
};

} // namespace sponge
