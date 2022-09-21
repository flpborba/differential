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

    TEST_CASE("tu function")
    {
        SUBCASE("trace of `delta` equals 0")
        {
            CHECK_THROWS(Tu(4, make_elem(0x0)));
        }

        SUBCASE("`x` equals 0")
        {
            const auto f = Tu(4, make_elem(0xE));

            CHECK_EQ(f(make_elem(0x0)), make_elem(0x0));
        }

        SUBCASE("trace equals 0 and `x` not equals 0")
        {
            const auto f = Tu(4, make_elem(0x9));

            CHECK_EQ(f(make_elem(0x2)), make_elem(0x7));
        }

        SUBCASE("trace equals 1")
        {
            const auto f = Tu(4, make_elem(0xD));

            CHECK_EQ(f(make_elem(0xA)), make_elem(0x5));
        }
    }

    TEST_CASE("Cached function")
    {
        auto f = Tu(4, make_elem(0xE));
        auto g = Cached<Tu>(4, make_elem(0xE));

        CHECK_EQ(f(make_elem(0x9)), g(make_elem(0x9)));
    }
}
