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
