#include "bf.h"
#include <NTL/GF2XFactoring.h>
#include <numeric>

Function::Function(int field_deg) noexcept
    : mod(NTL::BuildSparseIrred_GF2X(field_deg))
{
}

auto Function::field_degree() const noexcept -> int
{
    return mod.n;
}

auto Function::field_order() const noexcept -> size_t
{
    return size_t(1) << field_degree();
}

Dickson::Dickson(int field_deg, int deg, NTL::GF2X alpha) noexcept
    : Function(field_deg)
    , deg(deg)
{
    for (auto i = 1; i <= deg / 2; ++i) {
        auto numerator = NTL::ZZ(deg);
        auto denominator = NTL::ZZ(1);

        for (auto j = 1; j < i; ++j) {
            numerator *= deg - i - j;
            denominator *= j + 1;
        }

        const auto coeff = numerator / denominator;

        if (NTL::bit(coeff, 0) == 1)
            coeffs.emplace_back(deg - 2 * i, NTL::PowerMod(alpha, i, mod));
    }
}

auto Dickson::is_permutation() const noexcept -> bool
{
    auto m = 1 << (2 * field_degree());
    return std::gcd(deg, m - 1) == 1;
}

auto Dickson::operator()(const NTL::GF2X& x) const noexcept -> NTL::GF2X
{
    auto t = NTL::PowerMod(x, deg, mod);

    for (const auto& c : coeffs) {
        auto z = NTL::PowerMod(x, c.first, mod);
        const auto term = NTL::MulMod(z, c.second, mod);
        t += term;
    }

    return t;
}

Inverse::Inverse(int field_deg) noexcept
    : Function(field_deg)
{
}

auto Inverse::operator()(const NTL::GF2X& x) const noexcept -> NTL::GF2X
{
    return (bytes(x) <= 0x1) ? x : NTL::InvMod(x, mod);
}

Tu::Tu(int field_deg, NTL::GF2X delta)
    : Function(field_deg)
    , d(delta)
{
    if (NTL::TraceMod(delta, mod) == NTL::GF2::zero())
        throw std::invalid_argument("Tr(delta) equals 0");
}

auto Tu::operator()(const NTL::GF2X& x) const noexcept -> NTL::GF2X
{
    auto t = NTL::MulMod(d, x, mod);

    if (NTL::IsZero(NTL::TraceMod(t, mod))) {
        NTL::SqrMod(t, x, mod);
        NTL::add(t, t, x);
    }

    if (!NTL::IsZero(t))
        NTL::InvMod(t, t, mod);

    return t;
}

auto bytes(const NTL::GF2X& elem) noexcept -> uint64_t
{
    return NTL::IsZero(elem) ? 0x0 : elem.xrep[0];
}

auto make_elem(uint64_t bytes) noexcept -> NTL::GF2X
{
    if (bytes == 0x0)
        return NTL::GF2X();

    auto value = NTL::GF2X(0x1);
    value.xrep[0] = bytes;

    return value;
}
