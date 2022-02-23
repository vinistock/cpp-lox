cc_library(
    name = "vm",
    srcs = ["vm.cc", "token.cc", "scanner.cc", "parser.cc"],
    hdrs = ["vm.h", "token.h", "scanner.h", "expr.h", "ast_printer.h", "parser.h"],
    visibility = ["//:__pkg__", "//test:__pkg__"],
    deps = [
        "//literals:literals"
    ] 
)
