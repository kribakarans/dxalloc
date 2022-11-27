# Dxalloc: Xtended dynamic memory allocation

Dxalloc provides flawless execution of the system's dynamic memory management routines with inbuilt memory leak detection.

# Features:
1. Detect errors in system's dynamic memory management routines like `malloc`, `calloc` and `realloc`
2. Detect errors in string functions that returns heap memory areas like `strdup` and `asprintf`
3. Track and report memory leakage in dynamic memory management routines and above mentioned string functions
4. Track file open and close events with `dxfopen` and `dxfclose` routines
5. Mitigate dangling pointers and double free errors by nullifying pointers after `free`

| **Dxalloc API** | **Equivalent C library API** |
| ----------------|------------------------------|
| `dxmalloc`      | `malloc`                     |
| `dxcalloc`      | `calloc`                     |
| `dxrealloc`     | `realloc`                    |
| `dxfree`        | `free`                       |
| `dxstrdup`      | `strdup`                     |
| `dxasprintf`    | `asprintf`                   |
| `dxvasprintf`   | `vasprintf`                  |
| `dxfopen`       | `fopen`                      |
| `dxfclose`      | `fclose`                     |

# Start tracking
- Call `init_dxtrace()` in you code (in `main()`) to start tracking the memory leaks
- Report will print at end of the running process

# Usage:
1. Install dependencies `libbacktrace`
2. Build Dxalloc from here and do `make` and `make install`
    - Library files will be save at `/usr/local/lib`
    - Header files will be save at `/usr/local/include`
2. Compile the code with `-g -Wl,-rpath=/usr/local/lib/ -ldxalloc` flags
3. Now the code is ready to debug

**Example:**
```
#include <dxalloc.h>
  
int main()
{
  init_dxtrace();

  int *ptr = dxmalloc(10);
  FILE *fp = dxfopen("/tmp/test.txt", "w+");

  /* returning program without freeing ptr and closng fp */

  return 0;
}
```
**Compilation:**<br>
Compile the code: `gcc file.c -g -Wl,-rpath=/usr/local/lib/ -ldxalloc`

**Report:**
```
HEAP TRACE SUMMARY:
Backtrace of heap-pointer: 0x56157365aea0
  |__  0x7fbf029c2e3a :                   dxfopen (in src/alloc/dxalloc.c:139)
  |__  0x561571a271ba :                      main (in /home/shanmugk/Workspace/dxalloc-glib-dev/file.c:8)
  |__  0x7fbf027c7082 :         __libc_start_main (in ../csu/libc-start.c:308)
  |__  0x561571a270cd :                        ?? (in ./a.out:0)

Backtrace of heap-pointer: 0x56157365b120
  |__  0x7fbf029c275f :                  dxmalloc (in src/alloc/dxalloc.c:36)
  |__  0x561571a271a3 :                      main (in /home/shanmugk/Workspace/dxalloc-glib-dev/file.c:7)
  |__  0x7fbf027c7082 :         __libc_start_main (in ../csu/libc-start.c:308)
  |__  0x561571a270cd :                        ?? (in ./a.out:0)

Heap trace: Memory leak at 2 blocks !!!
```

