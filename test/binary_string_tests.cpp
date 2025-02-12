#include <arba/seri/binary/io.hpp>

#include <gtest/gtest.h>

#include <cstdlib>
#include <sstream>
#include <string>

TEST(binary_string_tests, test_read_write_std_string)
{
    std::string value("Hello!\nHow are you?");
    std::string stream_value("?");
    std::stringstream stream(std::ios::binary | std::ios::in | std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_string_tests, test_read_write_std_u8string)
{
    std::u8string value(u8"Hello¶ぁ中!");
    ASSERT_EQ(value.size(), 14);
    std::u8string stream_value(u8"?");
    std::stringstream stream(std::ios::binary | std::ios::in | std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_TRUE(value == stream_value);
}

// TEST(binary_string_tests, test_read_write_std_u16string)
//{
//     std::u16string value(u"Hello!\nHow are you?");
//     std::u16string stream_value(u"?");
//     std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
//     seri::write_binary(stream, value);
//     seri::read_binary(stream, stream_value);
//     ASSERT_EQ(value, stream_value);
// }
