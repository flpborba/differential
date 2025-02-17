#include "bf.h"
#include "differential.h"
#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>

Differential::Differential(const Function& f) noexcept
    : f(f)
{
}

auto Differential::uniformity() noexcept -> size_t
{
    const auto range = tbb::blocked_range<size_t>(0x1, f.field_order());

    const auto body = [this](const auto& r, auto val) {
        for (auto i = r.begin(); i < r.end(); ++i) {
            const auto a = make_elem(i);
            val = std::max(val, max_preimage_size(a));
        }

        return val;
    };

    return tbb::parallel_reduce(range, size_t(0), body, Max());
}

auto Differential::max_preimage_size(NTL::GF2X a) noexcept -> size_t
{
    const auto range = tbb::blocked_range<uint64_t>(0x0, f.field_order());

    const auto body = [this, &a](const auto& r, auto val) {
        for (auto i = r.begin(); i < r.end(); ++i) {
            const auto b = make_elem(i);
            const auto d = preimage_size(a, b);

            val = std::max(val, d);
        }

        return val;
    };

    return tbb::parallel_reduce(range, size_t(0), body, Max());
}

auto Differential::preimage_size(NTL::GF2X a, NTL::GF2X b) noexcept -> size_t
{
    const auto bit_floor = uint64_t(0x1) << NTL::deg(a);
    const auto range = tbb::blocked_range<uint64_t>(0x0, bit_floor);

    const auto body = [this, &a, &b, bit_floor](const auto& r, auto val) {
        const auto bit_ceil = bit_floor << 1;

        for (auto i = uint64_t(0x0); i < f.field_order(); i += bit_ceil)
            for (auto j = r.begin(); j < r.end(); ++j) {
                auto x = make_elem(i ^ j);

                if (is_solution(x, a, b))
                    val += 2;
            }

        return val;
    };

    return tbb::parallel_reduce(range, size_t(0), body, std::plus());
}

auto Differential::is_solution(NTL::GF2X x, NTL::GF2X a, NTL::GF2X b) noexcept -> bool
{
    return f.derivative(x, a) == b;
}

auto Max::operator()(size_t a, size_t b) const noexcept -> size_t
{
    return std::max(a, b);
}
