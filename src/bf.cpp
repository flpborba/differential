#include "bf.h"

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
