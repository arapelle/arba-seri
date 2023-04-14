#include <seri/binary/write_binary.hpp>
#include <ostream>

namespace seri
{
std::ostream& write_bytes(std::ostream& stream, const void* data, std::size_t number_of_bytes_to_write)
{
    return stream.write(reinterpret_cast<const char*>(data), number_of_bytes_to_write);
}
}
