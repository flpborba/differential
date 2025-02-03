#include "bf.h"
#include <doctest/doctest.h>

TEST_SUITE("binary field")
{
    TEST_CASE("element construction")
    {
        SUBCASE("zero element")
        {
            CHECK_EQ(make_elem(0x0), NTL::GF2X());
        }

        SUBCASE("unit element")
        {
            CHECK_EQ(make_elem(0x1), NTL::GF2X(0x1));
        }

        SUBCASE("general case")
        {
            auto elem = NTL::GF2X();
            NTL::SetCoeff(elem, 2);
            NTL::SetCoeff(elem, 13);

            CHECK_EQ(make_elem(0x2004), elem);
        }
    }

    TEST_CASE("polynomial evaluation")
    {
        SUBCASE("zero polynomial")
        {
            const auto p = Polynomial(3, {});

            CHECK_EQ(p(make_elem(0x4)), make_elem(0x0));
        }

        SUBCASE("constant polynomial")
        {
            const auto p = Polynomial(3, { 0x5 });

            CHECK_EQ(p(make_elem(0x6)), make_elem(0x5));
        }

        SUBCASE("general case")
        {
            const auto p = Polynomial(3, { 0x7, 0x0, 0x2 });

            CHECK_EQ(p(make_elem(0x7)), make_elem(0x1));
        }
    }
}
