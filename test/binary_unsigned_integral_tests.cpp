#include <seri/binary/io.hpp>
#include <gtest/gtest.h>
#include <sstream>
#include <cstdlib>

enum class enum_u8 : uint8_t
{
    DEFAULT = 0,
    VALUE = 6,
};

TEST(binary_unsigned_integral_tests, test_read_write_u8)
{
    uint8_t value = 0xab;
    uint8_t stream_value = 0;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_unsigned_integral_tests, test_read_write_eu8)
{
    enum_u8 value = enum_u8::VALUE;
    enum_u8 stream_value = enum_u8::DEFAULT;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_unsigned_integral_tests, test_read_write_u16)
{
    uint16_t value = 0xaabb;
    uint16_t stream_value = 0;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_unsigned_integral_tests, test_read_write_u32)
{
    uint32_t value = 0xaabbccdd;
    uint32_t stream_value = 0;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_unsigned_integral_tests, test_read_write_u64)
{
    uint64_t value = 0x1122334455667788;
    uint64_t stream_value = 0;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}
