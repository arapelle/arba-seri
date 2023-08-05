#include <arba/seri/version.hpp>
#include <gtest/gtest.h>
#include <cstdlib>

TEST(project_version_tests, test_version_macros)
{
    ASSERT_EQ(ARBA_SERI_VERSION_MAJOR, 0);
    ASSERT_EQ(ARBA_SERI_VERSION_MINOR, 4);
    ASSERT_EQ(ARBA_SERI_VERSION_PATCH, 0);
    ASSERT_STREQ(ARBA_SERI_VERSION, "0.4.0");
}
