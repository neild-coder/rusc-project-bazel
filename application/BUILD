load("@rules_rust//rust:defs.bzl", "rust_static_library", "rust_common")

rust_static_library(
    name = "rust-application",
    srcs = [
        "src/lib.rs",
        "src/rust_application.rs",
        "src/abstraction/gpio.rs",
        "src/abstraction/misc.rs",
    ],
    crate_root = "src/lib.rs",
    deps =[
        "@crate_index//:serde",
        "@crate_index//:serde_json"
    ]
)

cc_library(
    name = "rust-binder",
    srcs = [
        "application-src/rust_binder.c", "generated-output/librust_application.a"
    ],
    hdrs = [
        "application-src/rust_binder.h",
    ]
)

cc_binary(
    name = "application",
    srcs = [
        "application-src/main.c"
    ],
    deps = [
        ":rust-binder",
    ],
)