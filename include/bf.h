#pragma once

#include <NTL/GF2X.h>

/// @brief Returns the bytes representing an element of GF(2)[x].
/// @param elem An element of GF(2)[x].
auto bytes(const NTL::GF2X& elem) noexcept -> uint64_t;

/// @brief Constructs an element of GF(2)[x].
/// @param coefficients Bytes representing of the coefficients with the least significant bit of the
/// first byte corresponding to the constant term.
auto make_elem(uint64_t coefficients) noexcept -> NTL::GF2X;
