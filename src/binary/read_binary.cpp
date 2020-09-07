#include <seri/binary/read_binary.hpp>
#include <istream>

namespace seri
{
std::istream& read_bytes(std::istream& stream, void* buffer, std::size_t number_of_bytes_to_read)
{
    const std::size_t read_bytes = stream.rdbuf()->sgetn(reinterpret_cast<char*>(buffer), number_of_bytes_to_read);
    if (read_bytes != number_of_bytes_to_read)
        throw std::runtime_error("Failed to read " + std::to_string(number_of_bytes_to_read)
                                 + " bytes from input stream! Read " + std::to_string(read_bytes));
    return stream;
}
}
