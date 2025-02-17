#include "bf.h"
#include <NTL/GF2XFactoring.h>
#include <numeric>

Function::Function(int field_deg) noexcept
    : mod(NTL::BuildSparseIrred_GF2X(field_deg))
{
}

Polynomial::Polynomial(int field_deg, initializer_list<NTL::GF2X> coeffs) noexcept
    : Function(field_deg)
{
    for (auto it = coeffs.begin(); it != coeffs.end(); ++it)
        if (!NTL::IsZero(*it))
            this->coeffs.emplace(it - coeffs.begin(), *it);
}

auto Polynomial::operator()(const NTL::GF2X& x) const noexcept -> NTL::GF2X
{
    auto y = make_elem(0x0);

    for (const auto& [deg, coeff] : coeffs) {
        auto t = NTL::PowerMod(x, deg, mod);
        t = NTL::MulMod(coeff, t, mod);
        y += t;
    }

    return y;
}

Power::Power(int field_deg, int exp) noexcept
    : Polynomial(field_deg, {})
{
    coeffs.emplace(exp, make_elem(0x0));
}

auto Power::operator()(const NTL::GF2X& x) const noexcept -> NTL::GF2X
{
    auto exp = coeffs.begin()->first;
    auto y = NTL::PowerMod(x, exp, mod);

    return y;
}

auto bytes(const NTL::GF2X& elem) noexcept -> uint64_t
{
    return NTL::IsZero(elem) ? 0x0 : elem.xrep[0];
}

auto make_elem(uint64_t bytes) noexcept -> NTL::GF2X
{
    if (bytes == 0x0)
        return NTL::GF2X();

    auto elem = NTL::GF2X(0x1);
    elem.xrep[0] = bytes;

    return elem;
}
