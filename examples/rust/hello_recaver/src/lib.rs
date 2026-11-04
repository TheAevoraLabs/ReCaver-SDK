// ReCaver SDK Example: Native Rust Mod
// Credit: Quantum Creeper / Aevora Labs

#[no_mangle]
pub extern "C" fn recaver_mod_init() {
    println!("[HelloReCaver] Native Rust mod loaded successfully into ReCaver!");
}

#[no_mangle]
pub extern "C" fn recaver_mod_shutdown() {
    println!("[HelloReCaver] Native Rust mod is shutting down.");
}
