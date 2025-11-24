#!/usr/bin/env python3
import re, os, yaml

ROOT = os.path.dirname(os.path.dirname(__file__))  # project root
META_DIR = os.path.join(ROOT, "src", "meta")

schema_file = os.path.join(META_DIR, "meta_schema.hpp")
ir_file = os.path.join(META_DIR, "meta_ir.cpp")
loader_file = os.path.join(META_DIR, "meta_absorb_loader.hpp")


def extract_ir_nodes():
    if not os.path.exists(ir_file): 
        return []
    txt = open(ir_file).read()
    return sorted(set(re.findall(r"IR_([A-Za-z0-9_]+)", txt)))


def extract_absorb_rules():
    if not os.path.exists(loader_file): 
        return []
    txt = open(loader_file).read()
    return sorted(set(re.findall(r'absorb\("([^"]+)"\)', txt)))


def extract_types():
    if not os.path.exists(schema_file): 
        return []
    txt = open(schema_file).read()
    types = re.findall(r"(struct|class)\s+([A-Za-z0-9_]+)", txt)
    return sorted(set([t[1] for t in types]))


def generate_meta_file(output):
    ir_nodes = extract_ir_nodes()
    absorb_rules = extract_absorb_rules()
    types = extract_types()

    meta = {
        "meta_version": 1,
        "language": "sponge",
        "types": types,
        "ir_nodes": ir_nodes,
        "absorb_rules": absorb_rules,
    }

    with open(output, "w") as f:
        yaml.dump(meta, f, sort_keys=False)

    print(f"[OK] Generated META → {output}")


if __name__ == "__main__":
    os.makedirs("packs", exist_ok=True)
    generate_meta_file("packs/runtime.meta")
