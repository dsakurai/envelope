
Use vcpkg to install dependencies.
An easy way is to open the project in CLion and activate the vcpkg plugin.
Then, build the program and run it.

Basically, something like
```
$ ./envelope "[[[0, 0], [1,0]], [[1,1], [2,1]]]"
[[[0.0,0.0],[1.0,0.0]],[[1.0,1.0],[2.0,1.0]]]
```

Run it like
```
$ ./envelope
```
to see the help on the input format.