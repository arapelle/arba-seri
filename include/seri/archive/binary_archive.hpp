#pragma once

#include "archive_base.hpp"
#include <string>
#include <exception>

namespace seri
{
class binary_iarchive : public iarchive
{
public:
    binary_iarchive(std::istream& stream) : stream_(stream) {}
    virtual ~binary_iarchive() = default;

    void read(void* const data, std::size_t size) final
    {
        const std::size_t read_bytes = stream_.rdbuf()->sgetn(reinterpret_cast<char*>(data), size);
        if (read_bytes != size)
            throw std::runtime_error("Failed to read " + std::to_string(size)
                                     + " bytes from input stream! Read " + std::to_string(read_bytes));
    }

private:
    virtual void rtti_() {}

private:
    std::istream& stream_;
};

class binary_oarchive : public oarchive
{
public:
    binary_oarchive(std::ostream& stream) : stream_(stream) {}
    virtual ~binary_oarchive() = default;

    void write(const void* data, std::size_t size) final
    {
        const std::size_t written_bytes = stream_.rdbuf()->sputn(reinterpret_cast<const char*>(data), size);
        if (written_bytes != size)
            throw std::runtime_error("Failed to write " + std::to_string(size)
                                     + " bytes to output stream! Wrote " + std::to_string(written_bytes));
    }

private:
    virtual void rtti_() {}

private:
    std::ostream& stream_;
};
}
