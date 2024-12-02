Advent of Code
==============

[![Build Status](https://dev.azure.com/sjmulder/aoc/_apis/build/status/aoc?branchName=master)](https://dev.azure.com/sjmulder/aoc/_build/latest?definitionId=6&branchName=master)

[Advent of Code](https://www.adventofcode.com) solutions by myself (Sijmen).

Building
--------
There's a top-level Makefile to build and/or run all solutions. It
requires a BSD make. For Debian/Ubuntu/etc:

    sudo apt install bmake

Then from the top level:

    bmake        # build
    bmake run

Note that not all languages and years are integrated into the make tree.
There are a bunch of project files for various tools scattered around,
such as Visual C++ 6 projects, Makefiles targeting Turbo C and the odd
Rust project.

Build targets
-------------
It's also possible to build/run just a single year or year/language.
Often those have additional targets, e.g. `vis`:

    cd 2023/c
    bmake run
    bmake bench  # benchmark with /usr/bin/time
    bmake vis    # generate some visualisations with ffmpeg or dot

To run specific days or with other input files:

    cd 2023/c
    bmake run DAYS=day12 INPUT=../data/12-sample.txt
    bmake run DAYS=day15 INPUT=/path/to/my/inputs.txt
    bmake run DAYS="day12 day15" INPUT.day12=... INPUT.day15=...

Inputs
------
Without input overrides, the Makefiles expect inputs at
`inputs/YYYY/DD-input.txt` from the repository root, e.g.
`inputs/2023/15-input.txt`. Apart from some samples, no inputs are
provided in the repo as per Advent of Code guidelines. The `inputs`
submodule referenced in this repository is private.

Build options
-------------
Since there are solutions in various languages this requires a selection
of different build tools, runtimes, etc. The following flags can be
passed to `make` to *disable* languages/tools that are enabled by
default:

 * `WITH_DOTNET=0` for 'modern' .NET with `dotnet`
 * `WITH_MONO=0` for .NET 4 with Mono (`mcs`)
 * `WITH_CPP17=0` for code using C++17 features
 * `WITH_DOT=0` for visualisations using `dot` form Graphviz
 * `WITH_GOLF=0` for the code golf solutions (requiring old C standards)

The following flags *enable* languages that are disabled by default:

 * `WITH_BASIC=1` for `cbmbasic`

For example:

    bmake run WITH_DOTNET=0 WITH_BASIC=1

Standard build variables are mostly honored, e.g. `CC` and `CFLAGS`.

Score card
----------
**2024**

                         1---------2-----
                1234567890123456789012345
    C           **
    JavaScript	*

**2023**

                         1---------2-----
                1234567890123456789012345
    C           ************************
    C (DOS)     ** *  * *
    C++                      *
    C#          *
    Python      ****          *         *
    Kotlin      ****
    manually                       2

**2022**

                         1---------2-----
                1234567890123456789012345
    C           *************************
    C#          ***
    AWK         ** *
    Rust        *

**2021**

                         1---------2-----
                1234567890123456789012345
    C           ***********************1*
    C++         ***
    C#          **** ***    **
    C64 BASIC   **              *
    AWK          *                     1
    Excel       **                     1
    manually                          22
    armv6 asm   **

**2020**

                         1---------2-----
                1234567890123456789012345
    C           *************************
    C#           ** **   1
    JavaScript                        *
    C64 BASIC   *
    awk          *1        **1
    sh          *

**2015**

                         1---------2-----
                1234567890123456789012345
    C           ******************
    C#                  *
    awk                *
    sh
