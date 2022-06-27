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
}
