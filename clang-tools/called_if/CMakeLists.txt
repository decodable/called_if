set(LLVM_LINK_COMPONENTS support)

    add_clang_executable(
        called_if called_if.cpp match_func_decl.cpp match_if_stmt.cpp main.cpp)

        target_link_libraries(
            called_if PRIVATE clangTooling clangBasic clangASTMatchers)
