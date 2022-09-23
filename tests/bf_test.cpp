#include "bf.h"
#include <NTL/GF2XFactoring.h>
#include <doctest/doctest.h>

auto compute_dickson(size_t field_deg,
    const NTL::GF2X& x,
    const NTL::GF2X& a,
    const std::vector<size_t>& indexes) noexcept -> NTL::GF2X
{
    const auto deg = indexes.front();
    const auto mod = NTL::BuildSparseIrred_GF2X(field_deg);

    auto result = NTL::GF2X::zero();

    for (auto i : indexes) {
        const auto c = (deg - i) / 2;
        const auto coeff = NTL::PowerMod(a, c, mod);
        const auto term = NTL::MulMod(coeff, NTL::PowerMod(x, i, mod), mod);

        result += term;
    }

    return result;
}

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

    TEST_CASE("dickson of the first kind")
    {
        SUBCASE("degree 5")
        {
            auto a = make_elem(0x5);
            auto x = make_elem(0x3);
            auto indexes = std::vector<size_t> { 5, 3, 1 };

            auto d = Dickson(8, 5, a);

            CHECK_EQ(d(x), compute_dickson(d.field_degree(), x, a, indexes));
            CHECK_FALSE(d.is_permutation());
        }

        SUBCASE("degree 6")
        {
            auto a = make_elem(0xB);
            auto x = make_elem(0x3);
            auto indexes = std::vector<size_t> { 6, 2 };

            auto d = Dickson(7, 6, a);

            CHECK_EQ(d(x), compute_dickson(d.field_degree(), x, a, indexes));
            CHECK_FALSE(d.is_permutation());
        }

        SUBCASE("degree 9")
        {
            auto a = make_elem(0x6C);
            auto x = make_elem(0x16B);
            auto indexes = std::vector<size_t> { 9, 7, 5, 1 };

            auto d = Dickson(10, 9, a);

            CHECK_EQ(d(x), compute_dickson(d.field_degree(), x, a, indexes));
            CHECK_FALSE(d.is_permutation());
        }

        SUBCASE("degree 10")
        {
            auto a = make_elem(0x272);
            auto x = make_elem(0x3CB);
            auto indexes = std::vector<size_t> { 10, 6, 2 };

            auto d = Dickson(11, 10, a);

            CHECK_EQ(d(x), compute_dickson(d.field_degree(), x, a, indexes));
            CHECK(d.is_permutation());
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
