#include <seri/binary/io.hpp>
#include <gtest/gtest.h>
#include <sstream>
#include <cstdlib>

TEST(binary_io_tests, test_read_write_float)
{
    float value = -12.3647f;
    float stream_value = 0;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_io_tests, test_read_write_double)
{
    double value = -127895.3647;
    double stream_value = 0;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
