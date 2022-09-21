#include "bf.h"
#include <doctest/doctest.h>

TEST_SUITE("binary field")
{
    TEST_CASE("inverse uniformity")
    {
        SUBCASE("field degree 3")
        {
            CHECK_EQ(Inverse(3).uniformity(), 2);
            CHECK_EQ(Inverse(3).row_max_delta(make_elem(0x1)), 2);
            CHECK_EQ(Inverse(3).row_max_delta(make_elem(0x7)), 2);
        }

        SUBCASE("field degree 4")
        {
            CHECK_EQ(Inverse(4).uniformity(), 4);
            CHECK_EQ(Inverse(4).row_max_delta(make_elem(0x1)), 4);
            CHECK_EQ(Inverse(4).row_max_delta(make_elem(0xF)), 4);
        }
    }

    TEST_CASE("inverse uniformity with lookup table")
    {
        SUBCASE("field degree 5")
        {
            CHECK_EQ(Inverse(5).uniformity(RowLookupTag()), 2);
            CHECK_EQ(Inverse(5).row_max_delta(make_elem(0x1), RowLookupTag()), 2);
            CHECK_EQ(Inverse(5).row_max_delta(make_elem(0x1F), RowLookupTag()), 2);
        }

        SUBCASE("field degree 6")
        {
            CHECK_EQ(Inverse(6).uniformity(RowLookupTag()), 4);
            CHECK_EQ(Inverse(6).row_max_delta(make_elem(0x1), RowLookupTag()), 4);
            CHECK_EQ(Inverse(6).row_max_delta(make_elem(0x3F), RowLookupTag()), 4);
        }
    }

    TEST_CASE("Tu uniformity")
    {
        SUBCASE("field degree 4")
        {
            CHECK_LE(Tu(4, make_elem(0xA)).uniformity(), 6);
        }

        SUBCASE("field degree 6")
        {
            CHECK_LE(Tu(6, make_elem(0x3E)).uniformity(), 6);
        }
    }

    TEST_CASE("Tu uniformity with row lookup table")
    {
        SUBCASE("field degree 4")
        {
            CHECK_LE(Tu(4, make_elem(0xF)).uniformity(RowLookupTag()), 6);
        }

        SUBCASE("field degree 6")
        {
            CHECK_LE(Tu(6, make_elem(0x32)).uniformity(RowLookupTag()), 6);
        }
    }

    TEST_CASE("Cached function uniformity")
    {
        CHECK_EQ(Cached<Inverse>(3).uniformity(), 2);
        CHECK_LE(Cached<Tu>(4, make_elem(0xA)).uniformity(RowLookupTag()), 6);
    }
}
