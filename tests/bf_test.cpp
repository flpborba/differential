#include "bf.h"
#include <doctest/doctest.h>

struct FunctionVisitor : public Function {
    FunctionVisitor(size_t field_deg)
        : Function(field_deg)
    {
    }

    auto operator()(const NTL::GF2X& x) const noexcept -> NTL::GF2X override
    {
        ++visits;

        return NTL::GF2X();
    }

    mutable std::atomic<size_t> visits;
};

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

            const auto x = make_elem(0x4);
            const auto y = make_elem(0x0);

            CHECK_EQ(p(x), y);
        }

        SUBCASE("constant polynomial")
        {
            const auto p = Polynomial(3, { make_elem(0x5) });

            const auto x = make_elem(0x6);
            const auto y = make_elem(0x5);

            CHECK_EQ(p(x), y);
        }

        SUBCASE("general case")
        {
            const auto p = Polynomial(3, { make_elem(0x7), make_elem(0x0), make_elem(0x2) });

            const auto x = make_elem(0x7);
            const auto y = make_elem(0x1);

            CHECK_EQ(p(x), y);
        }
    }

    TEST_CASE("monomial evaluation")
    {
        SUBCASE("zero monomial")
        {
            const auto m = Monomial(3, 0, make_elem(0x0));

            CHECK_EQ(m(make_elem(0x4)), make_elem(0x0));
        }

        SUBCASE("constant monomial")
        {
            const auto m = Monomial(3, 1, make_elem(0x5));

            CHECK_EQ(m(make_elem(0x6)), make_elem(0x3));
        }

        SUBCASE("general case")
        {
            const auto m = Monomial(3, 2, make_elem(0x7));

            CHECK_EQ(m(make_elem(0x7)), make_elem(0x2));
        }
    }

    TEST_CASE("cached function")
    {
        SUBCASE("evaluation")
        {
            const auto coeffs = { make_elem(0x7), make_elem(0x4), make_elem(0x2) };

            const auto c = Cached<Polynomial>(3, coeffs);
            const auto p = Polynomial(3, coeffs);

            for (auto i = 0; i < 8; ++i) {
                const auto x = make_elem(i);

                CHECK_EQ(c(x), p(x));
            }
        }

        SUBCASE("number of calls to operator()")
        {
            const auto c = Cached<FunctionVisitor>(3);
            const auto x = make_elem(0x3);

            CHECK_EQ(c.visits, 0);

            for (auto i = 0; i < 5; ++i) {
                c(x);

                CHECK_EQ(c.visits, 1);
            }
        }
    }
}
