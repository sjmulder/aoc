intcode
=======
**interp** [**-v**] *code* [*input*]

AoC intcode interpreter. **-v** causes instructions, inputs and outputs
to be printed. *code* and *input* must be files or `-` for standard
input. Standard input is also used if *input* is omitted.

**intas** [**-v**] [**-o** *output*] [*input*]

AoC intcode assembler. See below for syntax. If *input* is not given or
`-`, standard input is used. If *output* is not given or `-`, standard
output is used. **-v** shows a dump between assembly stages.

Assembler synax
----------------
The base format is:

*instruction* *argument*, *argument*, ...

The following instructions are supported:

 - **add** *op1*, *op2*, *dst* - add two numbers.
 - **mul** *op1*, *op2*, *dst* - multiply two numbers.
 - **in** *dst* - read a number (IO).
 - **out** *src* - output a number (IO).
 - **jt** *op*, *target* - jump to *target* if *op* is nonzero.
 - **jf** *op*, *target* - jump to *target if *op* is zero.
 - **lt** *op1*, *op2*, *dst* - write 1 to *dst* if *op1* is less than
   *op2* or 0 otherwise.
 - **eq** *op1*, *op2*, *dst* - write 1 to *dst* if *op1* and *op* are
   equal, or 0 otherwise.
 - **hlt** - stop execution

Arguments can be given in these forms:

 - `out 0` - **immediate**: output the value 0.
 - `out foo` - **immediate (label)**: output the location of *foo*.
 - `out [0]` - **indirect**: output the value of the memory location 0.
 - `out [label]` - **indirect (label)**: output the value at the
   labeled location.

Note that destination arguments must always be indirect:

            add [1], 2, [3]  # good
            add [1], 2, 3    # bad

Labels are used to name specific memory locations (including arguments,
making it easy to write self-modifying code) like so:

            eq [0], [1], [are_same]
            jt are_same:0 [done]
            # ...
    done:   hlt

Multiple statements can be placed on one line separated by a `;`.
Comments start with `#` and run through the end of the line.

Limitations
-----------
Largely untested. Error messages are terrible. Hard limits abound. A
notable missing feature is raw data, e.g. for variables:

            add [var1], [var2], [var1]
    var1:   1
    var2:   2

Example
-------
Given `fizzbuzz.s`;

    loop:
            add 1, [n], [n]
            eq n:0, 101, [is_done]; jt is_done:0, done
    
            eq next_fibu:15, [n], [is_fibu]; jt is_fibu:0, fibu
            eq next_fizz: 3, [n], [is_fizz]; jt is_fizz:0, fizz
            eq next_buzz: 5, [n], [is_buzz]; jt is_buzz:0, buzz
            out [n]
            jt 1, loop
    
    fibu:
            out 88888
            add 15, [next_fibu], [next_fibu]
            add  3, [next_fizz], [next_fizz]
            add  5, [next_buzz], [next_buzz]
            jt 1, loop
    
    fizz:
            out 33333
            add 3, [next_fizz], [next_fizz]
            jt 1, loop
    
    buzz:
            out 55555
            add 5, [next_buzz], [next_buzz]
            jt 1, loop
    
    done:
            hlt

Assemble:

    $ intas -o as/fizzbuzz as/fizzbuzz.s
    $ cat as/fizzbuzz
    101,1,5,5,1108,0,101,9,1105,0,72,108,15,5,16,1105,0,37,108,3,5,23,
    105,0,54,108,5,5,30,1105,0,63,4,5,1105,1,0,104,88888,101,15,12,12,
    101,3,19,19,101,5,26,26,1105,1,0,104,33333,101,3,19,19,1105,1,0,
    104,55555,101,5,26,26,1105,1,0,99

Run:

    $ interp as/fizzbuzz
    1
    2
    33333
    4
    55555
    33333
    7
    [...]
    94
    55555
    33333
    97
    98
    33333
    55555

Installation
------------
Should work on Linux, macOS, BSD, etc.

    make

Author
------
Sijmen J. Mulder (<ik@sjmulder.nl>)
