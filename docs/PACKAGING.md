# ReCaver Mod Packaging Guide

This guide describes how to organize and distribute mods for ReCaver.

---

## 1. Directory Structure

A mod is packaged as a directory inside the ReCaver `mods/` folder:

```text
mods/
└── <mod_id>/
    ├── mod.toml              (Required: metadata manifest)
    ├── scripts/              (Optional: Lua scripts)
    │   └── init.lua          (Default Lua entry point)
    ├── bin/                  (Optional: compiled native shared libraries)
    │   ├── linux-x86_64/
    │   │   └── lib<mod_id>.so
    │   └── arm64-v8a/
    │       └── lib<mod_id>.so
    ├── assets/               (Optional: replacement models, textures, sounds)
    └── data/                 (Created automatically at runtime for save states)
```

---

## 2. Manifest Specification (`mod.toml`)

The manifest is written in TOML:

```toml
[mod]
id = "dark_fantasy_particles"
name = "Dark Fantasy Particle Overhaul"
version = "1.2.0"
author = "Modder"
description = "Replaces magic bolt and slash particle emitters with dark purple ember effects"

[entry]
# Path to Lua script relative to mod root (default: scripts/init.lua)
lua = "scripts/init.lua"

# Path to native library relative to mod root (optional if placed in bin/<platform-arch>/)
native = "bin/linux-x86_64/libparticles.so"
```

---

## 3. Platform Architecture Subdirectories

To ensure cross-platform compatibility when distributing mods, place binaries inside `bin/<platform-arch>/`:

| Subdirectory | Target Architecture | Notes |
|---|---|---|
| `bin/linux-x86_64/` | Linux Desktop 64-bit | Standard desktop Linux distributions |
| `bin/arm64-v8a/` | Android ARM64 64-bit | Modern Android devices |
| `bin/armeabi-v7a/` | Android ARM32 32-bit | Older Android devices |
| `bin/windows-x64/` | Windows Desktop 64-bit | Windows NT 64-bit builds |
