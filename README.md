A test library to support TDD in C++ projects

### Overview

I am excited to announce the first release of the CPP-Test-Library, a comprehensive testing framework designed for C++ projects. This release includes core functionalities to help developers create, manage, and run unit tests and test suites efficiently. The library is built to support exception handling, setup, and teardown mechanisms for robust testing processes.

### Key Features

- **Test Management**: 
  - Easily define and manage tests using `Test` and `TestSuite` classes.
  - Support for automatic registration of tests and test suites.

- **Exception Handling**:
  - Specialized classes (`ConfirmException`, `BoolConfirmException`, `ActualConfirmException`, and `MissingException`) for handling various exceptions during test execution.
  - `TestEx` template class for creating tests that expect specific exceptions.

- **Setup and Teardown**:
  - `SetupAndTeardown` template class to manage setup and teardown routines.
  - `TestSuiteSetupAndTearDown` template class for managing setup and teardown of test suites.

- **Assertions and Confirmations**:
  - Inline functions for confirming test outcomes (`confirm`) supporting various data types.
  - Macros (`CONFIRM_TRUE`, `CONFIRM_FALSE`, `CONFIRM`) for convenient assertions in tests.

- **Macro Definitions**:
  - Macros for defining tests (`TEST`, `TEST_EX`, `TEST_SUITE`, `TEST_SUITE_EX`) with easy syntax.
  - Automatic generation of unique class names for tests to avoid naming conflicts.

### How to Use

1. **Download and Extract**:
   - Download the `CPP-Test-Library-1.0.0.tar.gz` from the [Releases](https://github.com/sromic1990/CPP-Test-Library/releases) page.
   - Extract the tarball to a known location.

2. **Integrate with Your Project**:
   - Add the include directory to your project's include path.
   - Link your project with the static library.

Example CMake configuration:

```cmake
cmake_minimum_required(VERSION 3.10)
project(NewProject)

# Path to the extracted release
set(CPP_TEST_LIBRARY_PATH "/path/to/extracted/CPP-Test-Library-1.0.0")

include_directories(${CPP_TEST_LIBRARY_PATH}/include)
link_directories(${CPP_TEST_LIBRARY_PATH}/lib)

add_executable(NewProject main.cpp)

target_link_libraries(NewProject CPPTestLibrary)
```

### Example Usage

To use the CPP-Test-Library in your project, you can define and run tests using the provided macros and functions. Here is an example:

```cpp
#include "Test.h"

TEST("ExampleTest") {
    CONFIRM_TRUE(true);
}

int main() {
    std::ostream &output = std::cout;
    return SouravTDD::runTests(output);
}
```