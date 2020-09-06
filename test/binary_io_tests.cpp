#include "polymorphic_classes.hpp"
#include <seri/binary/binary_io.hpp>
#include <seri/archive/binary_archive.hpp>
#include <gtest/gtest.h>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <list>
#include <sstream>
#include <cstdlib>

enum class enum_u8 : uint8_t
{
    DEFAULT = 0,
    VALUE = 6,
};

enum class enum_i8 : int8_t
{
    DEFAULT = 0,
    VALUE = 6,
};

TEST(binary_io_tests, test_read_write_bool)
{
    bool value = true;
    bool stream_value = false;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_io_tests, test_read_write_u8)
{
    uint8_t value = 0xab;
    uint8_t stream_value = 0;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_io_tests, test_read_write_eu8)
{
    enum_u8 value = enum_u8::VALUE;
    enum_u8 stream_value = enum_u8::DEFAULT;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_io_tests, test_read_write_u16)
{
    uint16_t value = 0xaabb;
    uint16_t stream_value = 0;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_io_tests, test_read_write_u32)
{
    uint32_t value = 0xaabbccdd;
    uint32_t stream_value = 0;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_io_tests, test_read_write_u64)
{
    uint64_t value = 0x1122334455667788;
    uint64_t stream_value = 0;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_io_tests, test_read_write_i8)
{
    int8_t value = -4;
    int8_t stream_value = 0;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_io_tests, test_read_write_ei8)
{
    enum_i8 value = enum_i8::VALUE;
    enum_i8 stream_value = enum_i8::DEFAULT;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_io_tests, test_read_write_i16)
{
    int16_t value = 0xaabb;
    int16_t stream_value = 0;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_io_tests, test_read_write_i32)
{
    int32_t value = 0xaabbccdd;
    int32_t stream_value = 0;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_io_tests, test_read_write_i64)
{
    int64_t value = static_cast<int64_t>(0xff22334455667788);
    int64_t stream_value = 0;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

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

TEST(binary_io_tests, test_read_write_std_string)
{
    std::string value("Hello!\nHow are you?");
    std::string stream_value("?");
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

//TEST(binary_io_tests, test_read_write_std_u16string)
//{
//    std::u16string value(u"Hello!\nHow are you?");
//    std::u16string stream_value(u"?");
//    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
//    seri::write_binary(stream, value);
//    seri::read_binary(stream, stream_value);
//    ASSERT_EQ(value, stream_value);
//}

TEST(binary_io_tests, test_read_write_std_array)
{
    std::array<std::string, 3> value{ "abc", "mno", "xyz" };
    std::array<std::string, 3> stream_value;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_io_tests, test_read_write_std_vector)
{
    std::vector<std::string> value{ "abc", "xyz" };
    std::vector<std::string> stream_value;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_io_tests, test_read_write_std_list)
{
    std::list<std::string> value{ "abc", "xyz" };
    std::list<std::string> stream_value;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_io_tests, test_read_write_std_set)
{
    std::set<std::string> value{ "abc", "xyz" };
    std::set<std::string> stream_value;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_io_tests, test_read_write_std_map)
{
    std::map<std::string, std::string> value{ {"abc", "xyz"}, {"def", "rst"} };
    std::map<std::string, std::string> stream_value;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_io_tests, test_read_write_std_unordered_set)
{
    std::unordered_set<std::string> value{ "abc", "xyz" };
    std::unordered_set<std::string> stream_value;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_io_tests, test_read_write_std_unordered_map)
{
    std::unordered_map<std::string, std::string> value{ {"abc", "xyz"}, {"def", "rst"} };
    std::unordered_map<std::string, std::string> stream_value;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_io_tests, test_read_write_uuid)
{
    core::uuid value("2289d5f9-8bbe-4e27-87be-9cc3bbfd8ea0");
    core::uuid stream_value;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(value, stream_value);
}

TEST(binary_io_tests, test_read_write_std_unique_ptr)
{
    std::unique_ptr<Object> value = std::make_unique<Colored_polygon>(5, "white");
    std::unique_ptr<Object> stream_value;
    std::stringstream stream(std::ios::binary|std::ios::in|std::ios::out);
    seri::binary_oarchive oarchive(stream);
    seri::write_binary(oarchive, value);
    seri::binary_iarchive iarchive(stream);
    seri::read_binary(iarchive, stream_value);
    ASSERT_EQ(dynamic_cast<const Colored_polygon&>(*value), dynamic_cast<const Colored_polygon&>(*stream_value));
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
