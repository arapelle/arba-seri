#include <seri/binary/io.hpp>
#include <gtest/gtest.h>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <list>
#include <sstream>
#include <cstdlib>

TEST(binary_range_tests, test_read_write_std_array)
{
    std::array<std::string, 3> value{ "abc", "mno", "xyz" };
    std::array<std::string, 3> stream_value;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_range_tests, test_read_write_std_vector)
{
    std::vector<std::string> value{ "abc", "xyz" };
    std::vector<std::string> stream_value;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_range_tests, test_read_write_std_list)
{
    std::list<std::string> value{ "abc", "xyz" };
    std::list<std::string> stream_value;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_range_tests, test_read_write_std_set)
{
    std::set<std::string> value{ "abc", "xyz" };
    std::set<std::string> stream_value;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_range_tests, test_read_write_std_map)
{
    std::map<std::string, std::string> value{ {"abc", "xyz"}, {"def", "rst"} };
    std::map<std::string, std::string> stream_value;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_range_tests, test_read_write_std_unordered_set)
{
    std::unordered_set<std::string> value{ "abc", "xyz" };
    std::unordered_set<std::string> stream_value;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_range_tests, test_read_write_std_unordered_map)
{
    std::unordered_map<std::string, std::string> value{ {"abc", "xyz"}, {"def", "rst"} };
    std::unordered_map<std::string, std::string> stream_value;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}
