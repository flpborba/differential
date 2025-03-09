#include "bf.h"
#include "differential.h"
#include <NTL/GF2XFactoring.h>

Function::Function(size_t field_deg) noexcept
    : mod(NTL::BuildSparseIrred_GF2X(field_deg))
{
}

Polynomial::Polynomial(size_t field_deg, std::initializer_list<uint64_t> coeffs) noexcept
    : Function(field_deg)
{
    for (auto i = 0; auto c : coeffs) {
        if (c != 0x0)
            terms.emplace_back(i, make_elem(c));

        ++i;
    }
}

auto Function::field_degree() const noexcept -> size_t
{
    return deg(mod);
}

auto Function::field_order() const noexcept -> size_t
{
    return size_t(1) << field_degree();
}

auto Function::derivative(const NTL::GF2X& x, const NTL::GF2X& a) const noexcept -> NTL::GF2X
{
    return operator()(x) + operator()(x + a);
}

auto Polynomial::operator()(const NTL::GF2X& x) const noexcept -> NTL::GF2X
{
    auto y = NTL::GF2X();

    for (const auto& [degree, term] : terms) {
        auto t = NTL::PowerMod(x, degree, mod);
        t = NTL::MulMod(term, t, mod);
        y += t;
    }

    return y;
}

Monomial::Monomial(size_t field_deg, size_t deg, uint64_t coeff) noexcept
    : Polynomial(field_deg, { coeff })
{
    if (!terms.empty())
        terms[0].first = deg;
}

auto bytes(const NTL::GF2X& elem) noexcept -> uint64_t
{
    return NTL::IsZero(elem) ? 0x0 : elem.xrep[0];
}

auto make_elem(uint64_t coefficients) noexcept -> NTL::GF2X
{
    if (coefficients == 0x0)
        return NTL::GF2X();

    auto elem = NTL::GF2X();

    elem.xrep.SetLength(1);
    elem.xrep[0] = coefficients;

    return elem;
}
