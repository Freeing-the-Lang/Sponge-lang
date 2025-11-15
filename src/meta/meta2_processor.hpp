#pragma once
#include "meta_schema.hpp"
#include "meta_absorb_loader.hpp"

namespace sponge {

class Meta2Processor {
public:
    MetaSchema schema;

    MetaAbsorbLoader::LangPack parseMetaWithSchema(
        const std::string& schemaPath,
        const std::string& metaPath);
};

} // namespace sponge
