#include <arba/seri/binary/io.hpp>

#include <gtest/gtest.h>

#include <cstdlib>
#include <sstream>

enum class enum_i8 : int8_t
{
    DEFAULT = 0,
    VALUE = 6,
};

TEST(binary_signed_integral_tests, test_read_write_bool)
{
    bool value = true;
    bool stream_value = false;
    std::stringstream stream(std::ios::binary | std::ios::in | std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_signed_integral_tests, test_read_write_i8)
{
    int8_t value = -4;
    int8_t stream_value = 0;
    std::stringstream stream(std::ios::binary | std::ios::in | std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_signed_integral_tests, test_read_write_ei8)
{
    enum_i8 value = enum_i8::VALUE;
    enum_i8 stream_value = enum_i8::DEFAULT;
    std::stringstream stream(std::ios::binary | std::ios::in | std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_signed_integral_tests, test_read_write_i16)
{
    int16_t value = 0x7a8b;
    int16_t stream_value = 0;
    std::stringstream stream(std::ios::binary | std::ios::in | std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_signed_integral_tests, test_read_write_i32)
{
    int32_t value = 0xaabbccdd;
    int32_t stream_value = 0;
    std::stringstream stream(std::ios::binary | std::ios::in | std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_signed_integral_tests, test_read_write_i64)
{
    int64_t value = static_cast<int64_t>(0xff22334455667788);
    int64_t stream_value = 0;
    std::stringstream stream(std::ios::binary | std::ios::in | std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}
