#include "bf.h"
#include <NTL/GF2XFactoring.h>

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
