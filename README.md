[![Project Incubating](https://docs.outscale.com/fr/userguide/_images/Project-Incubating-blue.svg)](https://docs.outscale.com/en/userguide/Open-Source-Projects.html)

# Outscale SDK for C

Welcome to Outscale SDK for C.

## Dependency

json-c and libcurl (7.75 and after)

## How to use the SDK?

Simply grab the C source and add it to your project, you need to include json-c and curl (version > 7.75) to your code.

See [examples](examples/) folder to jump straight into the code!
Here's an explication of the Makefile rule compiling example 1:

```
${CC} examples/example1.c ./osc_sdk.c -I./ `pkg-config --cflags json-c` -lcurl -ljson-c -o example1 -Wall -Wno-unused-function -g
```
- `-I` specify include path (where to find `osc_sdk.h`). If osc_sdk.h was in the same directory than `example0.c` it wouldn't be needed.
- `-lcurl -ljson-c` specify curl and json-c libraries.
- `-Wno-unused-function` because osc-sdk.c is compiled with the example, and not all functions are used
- `pkg-config --cflags json-c` to find json-c cflags
- `-Wall` for more warning
- `-g` for debug symbole

`example0.c` is similar, but use an archive, so it can compile in ansi C (`-std=c89`)

## Notes

You need libcurl to a version superior to 7.75 (again it's important).

json-c have deprecated ANSI C in favor to C99 and C11, if you want to continue using C89 you might stick to current or old versions.

**Interface is not stable yet, please be aware that some breaking change can still exist until 1.x.**

SDK itself is generated from Outscale's [COGNAC](https://github.com/outscale/COGNAC) using [osc-api](https://github.com/outscale/osc-api) description.

# Contributing

Check [contributing documentation](CONTRIBUTING.md).

# License

> Copyright Outscale SAS
>
> BSD-3-Clause

This project is compliant with [REUSE](https://reuse.software/).
