# Concept

The purpose is to provide simple cryptographic algorithms in C++.

See [task board](https://app.gitkraken.com/glo/board/X1VWeSQxbQAR7JTn) for future updates and features.

# Install

## Requirements

Binaries:

- A C++20 compiler (ex: g++-10)
- CMake 3.26 or later

Libraries:

- [core](https://github.com/arapelle/core) 0.13 or later
- [Google Test](https://github.com/google/googletest) 1.10 or later (only for testing)

## Clone

```
git clone https://github.com/arapelle/seri --recurse-submodules
```

## Quick Install

There is a cmake script at the root of the project which builds the library in *Release* mode and install it (default options are used).

```
cd /path/to/seri
cmake -P cmake_quick_install.cmake
```

Use the following to quickly install a different mode.

```
cmake -DCMAKE_BUILD_TYPE=Debug -P cmake_quick_install.cmake
```

## Uninstall

There is a uninstall cmake script created during installation. You can use it to uninstall properly this library.

```
cd /path/to/installed-seri/
cmake -P cmake_uninstall.cmake
```

# How to use

## Example - Serialize a std::string

```c++
#include <iostream>
#include <seri/binary/io.hpp>

int main()
{
    std::string value("Hello! How are you?");
    std::string stream_value("?");
    std::cout << "       value: '" << value << "'" << std::endl;
    std::cout << "stream_value: '" << stream_value << "'" << std::endl;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    std::cout << "stream_value: '" << stream_value << "'" << std::endl;

    std::cout << "EXIT_SUCCESS" << std::endl;
    return EXIT_SUCCESS;
}
```

## Example - Serialize a polymorphic object

```c++
#include <seri/polymorphism.hpp>
#include <seri/binary/io.hpp>
#include <vector>
#include <memory>
#include <iostream>

class Abstract
{
public:
    virtual ~Abstract() {}
    virtual void print() const = 0;
    virtual std::ostream& write_binary(std::ostream& stream) const = 0;
    virtual std::istream& read_binary(std::istream& stream) = 0;
};

class Cube : public Abstract
{
public:
    Cube(unsigned size = 0) : size(size) {}
    virtual ~Cube() = default;
    virtual void print() const override { std::cout << "Cube_" << size << std::endl; }

    virtual std::ostream& write_binary(std::ostream& stream) const override
    {
        return seri::write_binary(stream, size);
    }

    virtual std::istream& read_binary(std::istream& stream) override
    {
        return seri::read_binary(stream, size);
    }

    unsigned size;
};

SERI_DEFINE_SERIALIZABLE_TYPE_ID(Cube, "45c03a9c-b94b-4168-bb23-dcf52562b9eb");
SERI_REGISTER_INHERITANCE_RELATION(Abstract, Cube)

int main()
{
    std::vector<std::unique_ptr<Abstract>> things;
    things.push_back(std::make_unique<Cube>(2));
    things.push_back(std::make_unique<Cube>(45));
    std::vector<std::unique_ptr<Abstract>> other_things;

    std::cout << "things:" << std::endl;
    for (const auto& thing_ptr : things)
        thing_ptr->print();
    std::cout << ".\n" << std::endl;

    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, things);
    seri::read_binary(stream, other_things);

    std::cout << "other_things:" << std::endl;
    for (const auto& thing_ptr : other_things)
        thing_ptr->print();
    std::cout << ".\n" << std::endl;

    std::cout << "EXIT_SUCCESS" << std::endl;
    return EXIT_SUCCESS;
}
```

## Example - Using *seri* in a CMake project

See the [basic cmake project](https://github.com/arapelle/seri/tree/master/example/basic_cmake_project) example, and more specifically the [CMakeLists.txt](https://github.com/arapelle/seri/tree/master/example/basic_cmake_project/CMakeLists.txt) to see how to use *wgen* in your CMake projects.

# License

[MIT License](https://github.com/arapelle/seri/blob/master/LICENSE.md) © seri
