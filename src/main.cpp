#include "meta/meta_absorb_loader.hpp"
#include "meta/meta2_processor.hpp"
#include "meta/meta_schema.hpp"
#include "meta_engine.hpp"

using namespace sponge;

int main() {
    SpongeMetaEngine eng;
    Meta2Processor processor;
    MetaAbsorbLoader loader;

    auto pack = processor.parseMetaWithSchema(
        "packs/meta.meta",
        "packs/rust.meta"
    );

    loader.mount(eng, pack);

    std::cout << eng.run("3 + 5 * 2") << "\n";
}
