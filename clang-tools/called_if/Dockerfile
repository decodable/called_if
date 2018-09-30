FROM codible / clang_dev : latest

                               ARG LLVM_SRC_ROOT =
    / usr / src / clang - llvm ARG TOOL_NAME =
        called_if

                WORKDIR $LLVM_SRC_ROOT WORKDIR llvm /
            tools / clang RUN echo "add_subdirectory(${TOOL_NAME})" >>
        tools / extra /
            CMakeLists.txt

                WORKDIR tools /
            extra / $TOOL_NAME COPY *./

            WORKDIR $LLVM_SRC_ROOT WORKDIR build RUN ninja

                CMD["bash"]
