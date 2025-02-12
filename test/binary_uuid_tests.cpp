#include <arba/seri/binary/io.hpp>

#include <gtest/gtest.h>

#include <cstdlib>
#include <sstream>

TEST(binary_uuid_tests, test_read_write_uuid)
{
    uuid::uuid value("2289d5f9-8bbe-4e27-87be-9cc3bbfd8ea0");
    uuid::uuid stream_value;
    std::stringstream stream(std::ios::binary | std::ios::in | std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}
