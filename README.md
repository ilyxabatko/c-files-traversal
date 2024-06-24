# System traversal script in C

This is a simple traversal script in C to look for JSON files that contain 64 len arrays and starts with an integer. My idea was to look for my abandoned Solana keypairs.

## Build and run

**Make sure you have `json-c` library installed so that the script can work with json objects.**
**Also, you can update the first argument in the `traverse_directory("/", output_file);` call to set a certain path to start with**
Build and run with Makefile (from the root of the project):

```
make && ./file_traversal
```

Build and run with a compiler:

```
gcc -Wall [PATH_TO_JSON-C_LIB_INCLUDE] -o file_traversal file_traversal.c [PATH_TO_JSON-C_LIB] -ljson-c && ./file_traversal
```

You can get a `json-c` path info with the `pkg-config` command (make sure it's installed on your machine) and then adjust your build parameters (refer to my Makefile example):

```
pkg-config --cflags --libs json-c
```

_p.s. ofc this script is not optimized, duplicates have to be removed, files reading and content storing can also be improved (I don't know how yet)._
