# cstring library

A string library in C with C++ standard features for string manipulation, designed to mimic the behavior of C++'s `std::string` (specifically `std::basic_string<char>`).

I don't have a documentation for now but
for reference check:
[cppreference](https://en.cppreference.com/cpp/string/basic_string)

## Quick Start

```c
#include "cstring.h"

int main(void)
{
    // Initialization
    cstring s = cstr_init("Hello");
    
    // Modifiers
    cstr_append(&s, " World");
    cstr_insert(&s, 5, " amazing");
    
    // Access & Info
    printf("String: %s\n", s);
    printf("Length: %zu\n", cstr_length(s));
    
    // Operations
    if (cstr_contains(s, "amazing")) {
        printf("It's amazing!\n");
    }
    
    // Cleanup
    cstr_free(&s);
    return 0;
}
```


## Core API Overview

### Initialization
- `cstr_init()`: Default constructor.
- `cstr_init("literal")`: Copy constructor.
- `cstr_init(count, 'c')`: Create a string with `count` characters of 'c'.
- `cstr_literal(name, "content")`: Create a **stack-allocated**, literal as `const_cstring`.

### Element Access
- `cstr_at(s, index)`: Bounds-checked access(supports `[]` but not recommended).
- `cstr_front(s)`, `cstr_back(s)`: First and last characters.

### Modifiers
- `cstr_append(&s, ...)`: Append strings, chars, or substrings.
- `cstr_insert(&s, pos, ...)`: Insert at a specific position.
- `cstr_erase(&s, pos, count)`: Remove a range of characters.
- `cstr_replace(&s, pos, count, ...)`: Replace a range with new content.
- `cstr_push_back(&s, 'c')`, `cstr_pop_back(&s)`: Stack-like operations.

### Search & Compare
- `cstr_find(s, ...)`: Find first occurrence of a char or string.
- `cstr_contains(s, ...)`: Check for existence.
- `cstr_starts_with(s, ...)` / `cstr_ends_with(s, ...)`: Prefix/Suffix checks.
- `cstr_compare(s, other)`: Lexicographical comparison.


## Build Instructions

This project uses CMake. 

```bash
git clone --recursive https://github.com/lucky017/cstring.git && cd cstring
mkdir build && cd build
cmake ..
make
```

Make sure to include the header and
link the resulting library with your binaries.

> [!NOTE]
> Use `-DBUILD_SHARED=ON` to build a shared library.
> 
> To build tests: `cmake -DBUILD_TESTS=ON ..`

## 8 Things I have to do
* **Search functions**
* **Input & Output**
* **Numeric conversions**
* **Hash support for cstring**
* **String View**
* **UTF-8 support**
* **Support for wchar_t, char16_t and char32_t**
* **Issues of performance and designs**

---
*Note: This library is byte-based and does not natively support multi-byte encodings like UTF-8.*