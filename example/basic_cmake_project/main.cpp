#include <arba/seri/binary/io.hpp>
#include <sstream>
#include <iostream>
#include <cstdlib>

int main()
{
    core::uuid id("ced1cc82-37ea-450f-a5ec-00e3c10a00f2");
    std::stringstream stream(std::ios::in|std::ios::out|std::ios::binary);
    seri::write_binary(stream, id);
    core::uuid other_id;
    seri::read_binary(stream, other_id);
    std::cout << other_id << std::endl;
    return EXIT_SUCCESS;
}
