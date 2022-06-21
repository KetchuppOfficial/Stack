![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)

# Stack

This project is a task of the programming course by [Ilya Dedinsky (aka ded32)](https://github.com/ded32).

## General information

**Stack** is an implementation of the stack - one of basic data stuctures. My version of the stack has 3 levels of security. Lever zero provides only reporting on errors in log file, canary protection is used on level one and level two checks data consistency by hashing it.

## Build and run

**Stack** is released for Linux only.

**Step 1:** Clone this repository.
```bash
git clone git@github.com:KetchuppOfficial/Stack.git
cd Stack
```

**Step 2:** Clone submodule.
```bash
git submodule init
git submodule update
```

**Step 3:** Build the project.
```bash
make
```

You'll see something like this:

```bash
username@machine:~/Stack$ make
Collecting dependencies for "src/Stack.c"...
Collecting dependencies for "src/Akinator.c"...
Collecting dependencies for "src/main.c"...
Compiling "src/main.c"...
Compiling "src/Akinator.c"...
Compiling "src/Stack.c"...
Collecting dependencies for "src/My_Lib.c"...
Compiling "src/My_Lib.c"...
ar: creating My_Lib.a
Linking project...
```

**Step 4:** Running.
```bash
make run IN=input_file_name
```
The program won't work, if you don't specify **input_file_name**.

## Different type support

**Stack** supports 11 data types. The default one is **int**. If you want to change it, you can do it in **./include/Stack.h**. Security mode can be chosen there as well.

```C
#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define SECURITY_LEVEL 2    // ,---------- Choose security mode here

#define CHAR      0     // char
#define INT       1     // int
#define U_INT     2     // unsigned int
#define L_INT     3     // long int
#define U_L_INT   4     // unsigned long int
#define L_L_INT   5     // long long int
#define U_L_L_INT 6     // unsigned long long int
#define FLOAT     7     // float
#define DOUBLE    8     // double
#define L_DOUBLE  9     // long double
#define PTR       10    // void *

#define STACK_TYPE INT      // <---------- Choose type here
```

## Dump system

You can dump all information about your stack with **Stack_Dump ()** function. Here is an example:

```
*************************************
ALL INFORMATION ABOUT STACK

POINTER ON STACK: 0x5624059444b0
LEFT STACK CANARY: DEDBEDA
DATA:
LEFT DATA CANARY: BADEDA
	data[0] = 1
	data[1] = 2
	data[2] = 3
	data[3] = 4
	data[4] = 5
	data[5] = 6
	data[6] = 7
	data[7] = 8
	data[8] = 9
	data[9] = 0
	data[10] = 0
	data[11] = 0
	data[12] = 0
	data[13] = 0
	data[14] = 0
	data[15] = 0
RIGHT DATA CANARY: BE3BAB
SIZE: 9
CAPACITY: 16
SAVED HASH:   3213657593669471231
CURRENT HASH: 3213657593669471231
RIGHT STACK CANARY: DEDBAD
*************************************
```
