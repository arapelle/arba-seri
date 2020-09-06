#include "polymorphic_classes.hpp"
#include <seri/polymorphism.hpp>
#include <gtest/gtest.h>
#include <sstream>
#include <cstdlib>

TEST(seri_tests, test_make_unique)
{
    std::unique_ptr<Object> object = seri::make_unique<Object>(seri::serializable_type_id<Colored_polygon>());
    bool is_a_colored_polygon = dynamic_cast<Colored_polygon*>(object.get());
    ASSERT_TRUE(is_a_colored_polygon);
    ASSERT_EQ(object->name(), "Colored_polygon");
}

TEST(seri_tests, test_make_shared)
{
    std::shared_ptr<Object> object = seri::make_shared<Object>(seri::serializable_type_id<Colored_polygon>());
    bool is_a_colored_polygon = dynamic_cast<Colored_polygon*>(object.get());
    ASSERT_TRUE(is_a_colored_polygon);
    ASSERT_EQ(object->name(), "Colored_polygon");
}

TEST(seri_tests, test_serializable_type_id_from_base_ref)
{
    std::unique_ptr<Object> object = std::make_unique<Colored_polygon>(5, "blue");
    seri::uutid object_stid = seri::serializable_type_id(*object);
    ASSERT_TRUE(!object_stid.is_nil());
    ASSERT_EQ(object_stid, seri::serializable_type_id<Colored_polygon>());
}

TEST(seri_tests, test_serializable_type_id_from_derived_ref)
{
    std::unique_ptr<Colored_polygon> object = std::make_unique<Colored_polygon>(4, "green");
    seri::uutid object_stid = seri::serializable_type_id(*object);
    ASSERT_TRUE(!object_stid.is_nil());
    ASSERT_EQ(object_stid, seri::serializable_type_id<Colored_polygon>());
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
