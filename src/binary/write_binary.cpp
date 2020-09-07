#include <seri/binary/write_binary.hpp>
#include <ostream>

namespace seri
{
std::ostream& write_bytes(std::ostream& stream, const void* data, std::size_t number_of_bytes_to_write)
{
    const std::size_t written_bytes = stream.rdbuf()->sputn(reinterpret_cast<const char*>(data), number_of_bytes_to_write);
    if (written_bytes != number_of_bytes_to_write)
        throw std::runtime_error("Failed to write " + std::to_string(number_of_bytes_to_write)
                                 + " bytes to output stream! Wrote " + std::to_string(written_bytes));
    return stream;
}
}
