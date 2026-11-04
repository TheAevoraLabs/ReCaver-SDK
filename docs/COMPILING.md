# Compiling Native ReCaver Mods

This guide covers compiling native C++ and Rust shared libraries for ReCaver.

---

## 1. C++ Mod Compilation

### Toolchain Requirements
- Modern C++ compiler with C++14 support (GCC 7+, Clang 6+).
- ReCaver SDK headers (`ReCaver-SDK/include`).

### Linux Desktop (`x86_64`)
```bash
g++ -O2 -fPIC -shared -std=c++14 \
    -I/path/to/ReCaver-SDK/include \
    src/mod_main.cpp \
    -o libmymod.so
```

### Android NDK (`arm64-v8a`)
Using Android NDK Clang (e.g. NDK r25+):
```bash
$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android24-clang++ \
    -O2 -fPIC -shared -std=c++14 \
    -I/path/to/ReCaver-SDK/include \
    src/mod_main.cpp \
    -o libmymod.so
```

---

## 2. Rust Mod Compilation

Rust mods compile as `cdylib` crates.

### `Cargo.toml`
```toml
[package]
name = "my_rust_mod"
version = "0.1.0"
edition = "2021"

[lib]
crate-type = ["cdylib"]

[dependencies]
```

### `src/lib.rs`
```rust
#[no_mangle]
pub extern "C" fn recaver_mod_init() {
    println!("[RustMod] Hello from Rust in ReCaver!");
}

#[no_mangle]
pub extern "C" fn recaver_mod_shutdown() {
    println!("[RustMod] Shutting down Rust module...");
}
```

### Build Command
```bash
cargo build --release
```
Copy `target/release/libmy_rust_mod.so` into your mod's `bin/linux-x86_64/` directory.
