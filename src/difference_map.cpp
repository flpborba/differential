#include "bf.h"
#include "difference_map.h"
#include <tbb/blocked_range.h>
#include <tbb/concurrent_vector.h>
#include <tbb/parallel_reduce.h>

auto Function::difference_map(const NTL::GF2X& x, const NTL::GF2X& a) const noexcept -> NTL::GF2X
{
    return operator()(x) + operator()(x + a);
}

auto Function::uniformity() const noexcept -> size_t
{
    const auto range = tbb::blocked_range<size_t>(0x1, field_order());

    const auto body = [this](const auto& r, auto val) {
        for (auto i = r.begin(); i < r.end(); ++i) {
            const auto a = make_elem(i);
            val = std::max(val, row_max_delta(a));
        }

        return val;
    };

    return tbb::parallel_reduce(range, size_t(0), body, Max());
}

auto Function::uniformity(RowLookupTag) const noexcept -> size_t
{
    const auto range = tbb::blocked_range<uint64_t>(0x1, field_order());

    const auto body = [this](const auto& r, auto val) {
        for (auto i = r.begin(); i < r.end(); ++i) {
            const auto a = make_elem(i);
            val = std::max(val, row_max_delta(a, RowLookupTag()));
        }

        return val;
    };

    return tbb::parallel_reduce(range, size_t(0), body, Max());
}

auto Function::row_max_delta(const NTL::GF2X& a) const noexcept -> size_t
{
    const auto range = tbb::blocked_range<uint64_t>(0x1, field_order());

    const auto body = [this, &a](const auto& r, auto val) {
        for (auto i = r.begin(); i < r.end(); ++i) {
            const auto b = make_elem(i);
            const auto d = delta(a, b);

            val = std::max(val, d);
        }

        return val;
    };

    return tbb::parallel_reduce(range, size_t(0), body, Max());
}

auto Function::row_max_delta(const NTL::GF2X& a, RowLookupTag) const noexcept -> size_t
{
    const auto bit_floor = uint64_t(0x1) << NTL::deg(a);
    const auto range = tbb::blocked_range<uint64_t>(0x0, bit_floor);

    auto row = tbb::concurrent_vector<size_t>(field_order());

    const auto body = [this, &row, &a, bit_floor](const auto& r, auto val) {
        const auto bit_ceil = bit_floor << 1;

        for (auto i = uint64_t(0x0); i < field_order(); i += bit_ceil)
            for (auto j = r.begin(); j < r.end(); ++j) {
                auto x = make_elem(i ^ j);
                auto diff = difference_map(x, a);

                val = std::max(val, row[bytes(diff)] += 2);
            }

        return val;
    };

    return tbb::parallel_reduce(range, size_t(0), body, Max());
}

auto Function::delta(const NTL::GF2X& a, const NTL::GF2X& b) const noexcept -> size_t
{
    const auto bit_floor = uint64_t(0x1) << NTL::deg(a);
    const auto range = tbb::blocked_range<uint64_t>(0x0, bit_floor);

    const auto body = [this, &a, &b, bit_floor](const auto& r, auto val) {
        const auto bit_ceil = bit_floor << 1;

        for (auto i = uint64_t(0x0); i < field_order(); i += bit_ceil)
            for (auto j = r.begin(); j < r.end(); ++j) {
                auto x = make_elem(i ^ j);
                auto diff = difference_map(x, a);

                if (diff == b)
                    val += 2;
            }

        return val;
    };

    return tbb::parallel_reduce(range, size_t(0), body, std::plus());
}

auto Inverse::uniformity() const noexcept -> size_t
{
    const auto a = make_elem(0x1);
    return row_max_delta(a);
}

auto Inverse::uniformity(RowLookupTag) const noexcept -> size_t
{
    const auto a = make_elem(0x1);
    return row_max_delta(a, RowLookupTag());
}

auto Max::operator()(size_t a, size_t b) const noexcept -> size_t
{
    return std::max(a, b);
}
