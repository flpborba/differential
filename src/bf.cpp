#include "bf.h"

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
