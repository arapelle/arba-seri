#include <arba/seri/binary/read_binary.hpp>

#include <istream>

inline namespace arba
{
namespace seri
{

std::istream& read_bytes(std::istream& stream, void* buffer, std::size_t number_of_bytes_to_read)
{
    return stream.read(reinterpret_cast<char*>(buffer), number_of_bytes_to_read);
}

} // namespace seri
} // namespace arba
