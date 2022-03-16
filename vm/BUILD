cc_library(
    name = "vm",
    srcs = ["vm.cc", "token.cc", "scanner.cc", "parser.cc", "interpreter.cc", "environment.cc"],
    hdrs = ["vm.h", "token.h", "scanner.h", "expr.h", "ast_printer.h", "parser.h", "interpreter.h", "stmt.h", "environment.h", "errors.h"],
    visibility = ["//:__pkg__", "//test:__pkg__"],
    deps = [
        "//literals:literals"
    ] 
)
