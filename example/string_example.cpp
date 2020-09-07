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
