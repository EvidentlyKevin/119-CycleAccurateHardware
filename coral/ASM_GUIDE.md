# Assembly / Machine Code from Python Guide

Explanations and code given by [ChatGPT](https://chatgpt.com/share/67b7799d-6084-8000-9228-9de680dadccc)

## View Python Bytecode Instructions

Python compiles code into an intermediate format called bytecode, which you can inspect using the dis module.

```python
import dis

def add(a, b):
    return a + b

dis.dis(add)
```

When this is run, the output will be (printed to the terminal):

```
  2           0 LOAD_FAST                0 (a)
              2 LOAD_FAST                1 (b)
              4 BINARY_ADD
              6 RETURN_VALUE
```

## Convert Python to a Compiled Binary

If you want to convert Python into a standalone machine code executable, you can use Cython or PyInstaller.

### Using Cython to Compile Python to C and Machine Code

1. Install Cython:

```bash
pip install cython
```

2. Write a simple Python script, add.py:

```python
def add(int a, int b):
    return a + b
```

3. Compile to a shared library:

```bash
cython --embed -o add.c add.py
gcc -shared -o add.so add.c -I/usr/include/python3.7 -lpython3.7
```

This generates a binary file that can run directly on the Cortex-A53 processor.

### Viewing the Contents of .o (Object) and .so (Shared Library) Files

Now that your add.o (object file) and add.so (shared library) were successfully created, you can inspect their contents using different tools.

#### 1. View Symbols and Function Names

Use nm to List Symbols in Object/Shared Files

```bash
nm -C add.o
nm -C add.so
```

Example Output:

```
0000000000001120 T PyInit_add
0000000000001140 t __pyx_module_is_main_add
0000000000001160 T add_function
```

#### 2. Disassemble Machine Code

Use objdump to Disassemble

```bash
objdump -d add.o | less
objdump -d add.so | less
```

Example Output:

```
0000000000001120 <PyInit_add>:
    1120: d2800000   mov x0, #0
    1124: 94000000   bl  1124 <some_function>
    1128: d65f03c0   ret
```

#### 3. View Raw Machine Code in Hex

Use xxd to See Hex Dump

```bash
xxd add.o | less
xxd add.so | less
```

Example Output:

```
00000000: 7f45 4c46 0201 0100 0000 0000 0000 0000  .ELF............
00000010: 0200 3e00 0100 0000 0000 0000 0000 0000  ..>.............
```

## Errors

### Fixing Python.h: No such file or directory Error in Cython Compilation

The error occurs because Python development headers are missing on your system. Here’s how you can fix it on Mendel Linux.

#### 1. Install Python Development Headers

You need the python3-dev package (or its equivalent for Mendel Linux).

##### Step 1: Try Installing python3-dev

```bash
sudo apt-get update
sudo apt-get install python3-dev
```

##### Step 2: Verify Installation

```bash
find /usr/include -name "Python.h"
```

Should have an output similar to "/usr/include/python3.7/Python.h"

#### 2. Fix GCC Include Path

If Python.h exists but gcc still can't find it, manually specify the path.

```bash
gcc -shared -o add.so add.c -I/usr/include/python3.7 -lpython3.7
gcc -shared -o add.so add.c -I/home/mendel/envs/instruction/include/python3.7m -lpython3.7m
```

### Fixing cannot find -lpython3.7 Error in GCC Linking

The error means that GCC cannot find the Python 3.7 shared library (libpython3.7.so) during the linking stage.

#### Step 1: Check If libpython3.7.so Exists

```bash
find /usr/lib -name "libpython3.7.so" 2>/dev/null
find /usr/local/lib -name "libpython3.7.so" 2>/dev/null
find /home/mendel/envs/instruction -name "libpython3.7.so" 2>/dev/null
```

If libpython3.7.so exists, note the directory (e.g., /usr/lib/python3.7/config-3.7-arm-linux-gnueabihf/).

#### Step 2: Install python3.7-dev (If Missing)

```bash
sudo apt-get update
sudo apt-get install python3.7-dev
sudo apt-get install libpython3.7
```

#### Step 3: Manually Specify the Library Path

```bash
gcc -shared -o add.so add.c -I/usr/include/python3.7 -L/usr/lib/python3.7/config-3.7-arm-linux-gnueabihf/ -lpython3.7
gcc -shared -o add.so add.c -I/home/mendel/envs/instruction/include/python3.7 -L/home/mendel/envs/instruction/lib -lpython3.7
```

#### Step 5: Check LD_LIBRARY_PATH

Make sure the linker can find libpython3.7.so:

```bash
export LD_LIBRARY_PATH=/usr/lib/python3.7/config-3.7-arm-linux-gnueabihf/:$LD_LIBRARY_PATH
echo 'export LD_LIBRARY_PATH=/usr/lib/python3.7/config-3.7-arm-linux-gnueabihf/:$LD_LIBRARY_PATH' >> ~/.bashrc
source ~/.bashrc
```

### Fixing cannot find -lpython3.7 Error on Mendel Linux

#### Fix 1: Specify Library Path in GCC

```bash
gcc -shared -o add.so add.c -I/usr/include/python3.7 -L/usr/lib/python3.7/config-3.7m-aarch64-linux-gnu/ -lpython3.7
```

#### Fix 2: Create a Symlink to /usr/lib/

GCC is probably searching /usr/lib/ instead of /usr/lib/python3.7/config-3.7m-aarch64-linux-gnu/. To fix this, create a symbolic link:

```bash
sudo ln -s /usr/lib/python3.7/config-3.7m-aarch64-linux-gnu/libpython3.7.so /usr/lib/libpython3.7.so
gcc -shared -o add.so add.c -I/usr/include/python3.7 -L/usr/lib/ -lpython3.7
```

#### Fix 3: Update Library Path with LD_LIBRARY_PATH

```bash
export LD_LIBRARY_PATH=/usr/lib/python3.7/config-3.7m-aarch64-linux-gnu/:$LD_LIBRARY_PATH
echo 'export LD_LIBRARY_PATH=/usr/lib/python3.7/config-3.7m-aarch64-linux-gnu/:$LD_LIBRARY_PATH' >> ~/.bashrc
source ~/.bashrc
gcc -shared -o add.so add.c -I/usr/include/python3.7 -L/usr/lib/python3.7/config-3.7m-aarch64-linux-gnu/ -lpython3.7
```

### Fixing relocation R_AARCH64_ADR_PREL_PG_HI21 and recompile with -fPIC Error

The error occurs because the compiled object file (add.o) is not position-independent code (PIC), which is required when creating shared libraries.

#### Fix 1: Recompile with -fPIC

To fix this, recompile add.c with the -fPIC flag:

```bash
gcc -fPIC -c add.c -I/usr/include/python3.7
gcc -shared -o add.so add.o -L/usr/lib/python3.7/config-3.7m-aarch64-linux-gnu/ -lpython3.7
```

#### Fix 2: Recompile from Cython with -fPIC

If add.c was generated using Cython, recompile it from scratch with the correct flags:

```bash
cython --embed -o add.c add.py
gcc -fPIC -c add.c -I/usr/include/python3.7
gcc -shared -o add.so add.o -L/usr/lib/python3.7/config-3.7m-aarch64-linux-gnu/ -lpython3.7
```
