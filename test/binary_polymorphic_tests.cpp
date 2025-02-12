#include "classes.hpp"
#include <arba/seri/binary/io.hpp>

#include <gtest/gtest.h>

#include <cstdlib>
#include <sstream>

TEST(binary_polymorphic_tests, test_read_write_std_unique_ptr_polymorphic)
{
    std::unique_ptr<Object> value = std::make_unique<Colored_polygon>(5, "white");
    std::unique_ptr<Object> stream_value;
    std::stringstream stream(std::ios::binary | std::ios::in | std::ios::out);
    arba::seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(dynamic_cast<const Colored_polygon&>(*value), dynamic_cast<const Colored_polygon&>(*stream_value));
}

TEST(binary_polymorphic_tests, test_read_write_std_unique_ptr)
{
    std::unique_ptr<Text> value = std::make_unique<Text>("Awesome!");
    std::unique_ptr<Text> stream_value;
    std::stringstream stream(std::ios::binary | std::ios::in | std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(*value, *stream_value);
}

TEST(binary_polymorphic_tests, test_read_write_std_shared_ptr_polymorphic)
{
    std::shared_ptr<Object> value = std::make_shared<Colored_polygon>(5, "white");
    std::shared_ptr<Object> stream_value;
    std::stringstream stream(std::ios::binary | std::ios::in | std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(dynamic_cast<const Colored_polygon&>(*value), dynamic_cast<const Colored_polygon&>(*stream_value));
}

TEST(binary_polymorphic_tests, test_read_write_std_shared_ptr)
{
    std::shared_ptr<Text> value = std::make_shared<Text>("Awesome!");
    std::shared_ptr<Text> stream_value;
    std::stringstream stream(std::ios::binary | std::ios::in | std::ios::out);
    seri::write_binary(stream, value);
    seri::read_binary(stream, stream_value);
    ASSERT_EQ(*value, *stream_value);
}
