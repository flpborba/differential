#include "bf.h"
#include "differential.h"
#include "hash.h"
#include <doctest/doctest.h>
#include <tbb/concurrent_unordered_set.h>
#include <atomic>

struct FunctionVisitor : public Function {
    FunctionVisitor(size_t field_deg)
        : Function(field_deg)
    {
    }

    auto operator()(const NTL::GF2X& x) const noexcept -> NTL::GF2X override
    {
        return NTL::GF2X();
    }
};

struct InputDifferenceCount : public Differential {
    using Differential::Differential;

    auto max_preimage_size(NTL::GF2X a) noexcept -> size_t override
    {
        visited.insert(bytes(a));
        ++visits;

        return 0;
    }

    tbb::concurrent_unordered_set<size_t> visited;
    std::atomic<size_t> visits;
};

struct DifferentialCount : public Differential {
    using Differential::Differential;

    auto preimage_size(NTL::GF2X a, NTL::GF2X b) noexcept -> size_t override
    {
        visited.insert(std::make_tuple(bytes(a), bytes(b)));
        ++visits;

        return 0;
    }

    tbb::concurrent_unordered_set<std::tuple<size_t, size_t>, Hash> visited;
    std::atomic<size_t> visits;
};

struct SolutionCheckingCount : public Differential {
    using Differential::Differential;

    auto is_solution(NTL::GF2X x, NTL::GF2X a, NTL::GF2X b) noexcept -> bool override
    {
        visited.insert(std::make_tuple(bytes(x), bytes(a), bytes(b)));
        ++visits;

        return false;
    }

    tbb::concurrent_unordered_set<std::tuple<size_t, size_t, size_t>, Hash> visited;
    std::atomic<size_t> visits;
};

TEST_SUITE("binary field")
{
    TEST_CASE("input difference iteration")
    {
        auto v = InputDifferenceCount(FunctionVisitor(5));

        v.uniformity();

        CHECK_EQ(v.visited.size(), 31);
        CHECK_EQ(v.visits, 31);
    }

    TEST_CASE("differential iteration")
    {
        auto v = DifferentialCount(FunctionVisitor(5));

        v.uniformity();

        CHECK_EQ(v.visited.size(), 992);
        CHECK_EQ(v.visits, 992);
    }

    TEST_CASE("solution checking iteration")
    {
        auto v = SolutionCheckingCount(FunctionVisitor(5));

        v.uniformity();

        CHECK_EQ(v.visited.size(), 15872);
        CHECK_EQ(v.visits, 15872);
    }

    TEST_CASE("polynomial uniformity")
    {
        SUBCASE("zero polynomial")
        {
            auto p = Polynomial(3, {});
            auto d = Differential(p);

            CHECK_EQ(d.uniformity(), 8);
        }

        SUBCASE("constant polynomial")
        {
            auto p = Polynomial(5, { make_elem(0x1) });
            auto d = Differential(p);

            CHECK_EQ(d.uniformity(), 32);
        }

        SUBCASE("degree 2 polynomial")
        {
            auto p = Polynomial(2, { make_elem(0x1), make_elem(0x1) });
            auto d = Differential(p);

            CHECK_EQ(d.uniformity(), 4);
        }
    }
}
