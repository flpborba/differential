#pragma once

#include <NTL/GF2X.h>

/// @brief Returns the bytes representing a polynomial over GF(2)[x].
/// @param a A polynomial over GF(2)[x].
auto bytes(const NTL::GF2X& elem) noexcept -> uint64_t;

/// @brief Constructs a polynomial over GF(2)[x].
/// @param bytes Bytes representing of the coefficients with the least significant bit of the
/// first byte corresponding to the constant term.
auto make_elem(uint64_t bytes) noexcept -> NTL::GF2X;
