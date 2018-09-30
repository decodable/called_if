# called_if
To find out whether a function is called in conditional.

We have some functions, which have special requirements, that they cannot be called in conditional.
For example, _bar_ is called depends on the value of variable _cond_. So we need to find out such functions like _bar_.
```
void foo(bool cond) {
  if (cond) {
    bar();
  }
}
```

## libclang
One implementation uses libclang, which is the C Interface to Clang.

To build:
```
git clone git@github.com:decodable/called_if.git
cd called_if/lib-clang
mkdir build
cd build
cmake ..
make
```
## LibTooling
```
LibTooling is a library to support writing standalone tools based on Clang.
```
To build:
```
git clone git@github.com:decodable/called_if.git
cd called_if/clang-tools/called_if
docker build .
```
## Docker
Compiled and tested in the docker [clang_dev](https://hub.docker.com/r/codible/clang_dev/).

## References
1. [Clang 8 - Choosing the Right Interface for Your Application](https://clang.llvm.org/docs/Tooling.html)
2. [Baby steps with `libclang`: Walking an abstract syntax tree](http://bastian.rieck.ru/blog/posts/2015/baby_steps_libclang_ast/)
3. [How to write RecursiveASTVisitor based ASTFrontendActions.](https://clang.llvm.org/docs/RAVFrontendAction.html)

