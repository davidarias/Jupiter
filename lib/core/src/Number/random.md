# Random number generator

This method generates random real numbers in thr range (0, 1)

it is intended to be used as a *'class'* method like this:

```smalltalk
Number random
```
but it will work as a *'instance'* method like this:

```smalltalk
3 random
```

Of course this last form doesn't make much sense, since the *self*
argument is ignored