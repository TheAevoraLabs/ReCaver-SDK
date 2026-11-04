# ReCaver Native Modding Architecture

This document details how ReCaver executes native and script mods, how dynamic symbol resolution works without hooking engines, and how the sandbox boundary operates.

---

## 1. Why Native Hooking (Dobby / GlossHook) Was Replaced

Legacy mobile/desktop mods typically hook compiled game engines using inline assembly trampoline libraries (such as Dobby, GlossHook, Substrate, or MinHook). This approach suffered from severe drawbacks:

1. **Fragility Across Releases**: A minor recompilation changes instruction boundaries and function prologue offsets, causing immediate crash upon launch.
2. **CPU Cache Coherency Issues**: Modifying executable memory pages (`mprotect` / `PAGE_EXECUTE_READWRITE`) causes instruction cache desynchronization on modern ARM CPUs.
3. **ABI Incompatibilities**: Compilers optimize parameter passing and register allocation differently across compiler versions and optimization levels.
4. **Lack of Type Safety**: Mod authors cast raw pointers at arbitrary byte offsets (e.g. `*(float*)(entity + 0x148) = 100.0f`), which breaks if a struct field moves by even 4 bytes.

ReCaver solves this at the architecture level by making the engine itself the host. ReCaver compiles with standard symbol visibility and exports its Itanium C++ mangled symbols directly to the dynamic linker.

---

## 2. ELF Dynamic Symbol Resolution (`.dynsym`)

When compiling ReCaver, the link stage includes `-rdynamic -Wl,--export-dynamic`.

In ELF binaries:
- `.symtab` holds static/local debug symbols (which are stripped in production builds).
- `.dynsym` holds dynamic symbols required for dynamic linking (`dlopen`, `dlsym`).

Stripping a ReCaver binary with `strip` strips `.symtab`, reducing binary size from 52MB to ~7.6MB, but leaves `.dynsym` 100% intact.

When ReCaver calls `dlopen("libmymod.so", RTLD_NOW | RTLD_GLOBAL)`, the dynamic linker automatically binds undefined symbols in `libmymod.so` (like `_ZN5Caver5Scene10LoadFromFileERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE`) to the running game process. No import libraries or static link stubs are needed.

---

## 3. Mod Discovery and Load Lifecycle

When ReCaver starts up, `ModLoader::EnsureInitialized()` scans the `mods/` directory.

For each directory:
1. `mod.toml` is parsed to read `id`, `name`, `version`, `author`, `description`, `entry.lua`, and `entry.native`.
2. ReCaver checks for platform-specific native shared libraries in candidate locations:
   - Explicit path from `mod.toml` (`entry.native`)
   - `bin/<platform-arch>/` (e.g. `bin/linux-x86_64/`, `bin/arm64-v8a/`)
   - `bin/`
   - `libraries/<platform-arch>/`
   - `libraries/`
3. Found libraries are loaded via `dlopen` (or `LoadLibraryA` on Windows).
4. ReCaver looks up the initialization symbol in the library:
   - `recaver_mod_init` (preferred)
   - `caver_mod_init`
   - `mod_init`
   If found, it is executed immediately.
5. If an entry Lua script is present (`scripts/init.lua` by default), ReCaver initializes the sandboxed Lua environment and executes the script.
6. When the game shuts down or reloads mods, ReCaver looks up `recaver_mod_shutdown` (or `caver_mod_shutdown` / `mod_shutdown`), executes it, unregisters all event listeners via `ClearAllListeners()`, and closes library handles via `dlclose`.

---

## 4. Lua Sandbox Boundary

For Lua modding, ReCaver provides a sandboxed environment:
- Unsafe standard libraries (`io`, `os`, `package`, `debug`) are omitted.
- Mods interact with storage via `ModData` APIs.
- Script functions can interact with engine state through high-level `Caver` Lua bindings.
- Hybrid mods can export custom C functions to Lua within `recaver_mod_init`.
