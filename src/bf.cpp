#include "bf.h"
#include <NTL/GF2XFactoring.h>

Inverse::Inverse(int field_deg) noexcept
    : mod(NTL::BuildSparseIrred_GF2X(field_deg))
{
}

auto Inverse::field_degree() const noexcept -> int
{
    return mod.n;
}

auto Inverse::field_order() const noexcept -> size_t
{
    return size_t(1) << field_degree();
}

auto Inverse::operator()(const NTL::GF2X& x) const noexcept -> NTL::GF2X
{
    return (bytes(x) <= 0x1) ? x : NTL::InvMod(x, mod);
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
