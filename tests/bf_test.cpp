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

    TEST_CASE("monomial evaluation")
    {
        SUBCASE("zero monomial")
        {
            const auto m = Monomial(3, 0, 0x0);

            CHECK_EQ(m(make_elem(0x4)), make_elem(0x0));
        }

        SUBCASE("constant monomial")
        {
            const auto m = Monomial(3, 1, 0x5);

            CHECK_EQ(m(make_elem(0x6)), make_elem(0x3));
        }

        SUBCASE("general case")
        {
            const auto m = Monomial(3, 2, 0x7);

            CHECK_EQ(m(make_elem(0x7)), make_elem(0x2));
        }
    }

    TEST_CASE("cached function")
    {
        const auto c = Cached<Polynomial>(3, { 0x7, 0x4, 0x2 });
        const auto p = Polynomial(3, { 0x7, 0x4, 0x2 });

        for (auto i = 0; i < 8; ++i) {
            auto x = make_elem(i);

            CHECK_EQ(c(x), p(x));
        }
    }
}
