#include "bf.h"
#include <doctest/doctest.h>

TEST_SUITE("binary field")
{
    TEST_CASE("element construction")
    {
        SUBCASE("zero polynomial")
        {
            CHECK_EQ(make_elem(0x0), NTL::GF2X::zero());
        }

        SUBCASE("unit polynomial")
        {
            CHECK_EQ(make_elem(0x1), NTL::GF2X(0x1));
        }

        SUBCASE("general case")
        {
            auto elem = NTL::GF2X();
            NTL::SetCoeff(elem, 0x2);
            NTL::SetCoeff(elem, 0xD);

            CHECK_EQ(make_elem(0x2004), elem);
        }
    }
}
