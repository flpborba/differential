#include "bf.h"
#include <doctest/doctest.h>

TEST_SUITE("binary field")
{
    TEST_CASE("element construction")
    {
        SUBCASE("zero element")
        {
            CHECK_EQ(make_elem(0x0), NTL::GF2X::zero());
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
            const auto p = Polynomial(3, { make_elem(0x5) });

            CHECK_EQ(p(make_elem(0x6)), make_elem(0x5));
        }

        SUBCASE("general case")
        {
            const auto p = Polynomial(3, { make_elem(0x7), make_elem(0x0), make_elem(0x2) });

            CHECK_EQ(p(make_elem(0x7)), make_elem(0x1));
        }
    }

    TEST_CASE("power evaluation")
    {
        SUBCASE("zero exponent")
        {
            const auto p = Power(3, 0);

            CHECK_EQ(p(make_elem(0x4)), make_elem(0x1));
        }

        SUBCASE("general case")
        {
            const auto p = Power(3, 3);

            CHECK_EQ(p(make_elem(0x6)), make_elem(0x7));
        }
    }
}
