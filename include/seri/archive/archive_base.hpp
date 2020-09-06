#pragma once

#include <istream>
#include <ostream>

namespace seri
{
class iarchive
{
public:
    iarchive() = default;
    iarchive(iarchive&&) noexcept {}
    virtual ~iarchive() noexcept = default;
    iarchive& operator=(iarchive&&) noexcept { return *this; }
    virtual void read(void* const data, std::size_t size) = 0;

private:
    virtual void rtti_() {}
};

class oarchive
{
public:
    oarchive() = default;
    oarchive(oarchive&&) noexcept {}
    virtual ~oarchive() noexcept = default;
    oarchive& operator=(oarchive&&) noexcept { return *this; }
    virtual void write(const void* data, std::size_t size) = 0;

private:
    virtual void rtti_() {}
};
}
