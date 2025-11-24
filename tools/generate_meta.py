#!/usr/bin/env python3
import re, os, yaml

ROOT = os.path.dirname(os.path.dirname(__file__))  # project root
META_DIR = os.path.join(ROOT, "src", "meta")

schema_file = os.path.join(META_DIR, "meta_schema.hpp")
ir_file = os.path.join(META_DIR, "meta_ir.cpp")
loader_file = os.path.join(META_DIR, "meta_absorb_loader.hpp")


def read_file(path):
    if not os.path.exists(path):
        return ""
    # Windows-safe file reading
    return open(path, encoding="utf-8", errors="ignore").read()


def extract_ir_nodes():
    txt = read_file(ir_file)
    return sorted(set(re.findall(r"IR_([A-Za-z0-9_]+)", txt)))


def extract_absorb_rules():
    txt = read_file(loader_file)
    return sorted(set(re.findall(r'absorb\("([^"]+)"\)', txt)))


def extract_types():
    txt = read_file(schema_file)
    types = re.findall(r"(struct|class)\s+([A-Za-z0-9_]+)", txt)
    return sorted(set([t[1] for t in types]))


def generate_meta_file(output):
    meta = {
        "meta_version": 1,
        "language": "sponge",
        "types": extract_types(),
        "ir_nodes": extract_ir_nodes(),
        "absorb_rules": extract_absorb_rules()
    }

    with open(output, "w", encoding="utf-8") as f:
        yaml.dump(meta, f, sort_keys=False)

    # ASCII only (Windows-safe)
    print("[OK] Generated META:", output)


if __name__ == "__main__":
    os.makedirs("packs", exist_ok=True)
    generate_meta_file("packs/runtime.meta")
