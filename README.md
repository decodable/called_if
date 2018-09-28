# called_if
To find out whether a function is called in conditional.

We have some functions, which have special requirements, that they cannot be called in conditional.
For example, _bar_ can be called or not depends on the value of variable _cond_. So we need to find out such functions as _bar_.
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

## Docker
Compiled and tested in the docker [clang_dev](https://hub.docker.com/r/codible/clang_dev/).

