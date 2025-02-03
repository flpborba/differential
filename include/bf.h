#pragma once

#include <NTL/GF2X.h>
#include <unordered_map>

using namespace std;

/// @brief Base class for functions over GF(2)[x].
class Function {
public:
    /// @brief Applies the function.
    /// @param x Point in which to evaluate.
    virtual auto operator()(const NTL::GF2X& x) const noexcept -> NTL::GF2X = 0;

protected:
    /// @brief Constructs an instance of a function over GF(2)[x].
    /// @param field_deg Degree of the field modulus.
    explicit Function(int field_deg) noexcept;

protected:
    NTL::GF2XModulus mod;
};

/// @brief A polynomial function over a finite field.
class Polynomial : public Function {
public:
    /// @brief Constructs an instance of a polynomial function over a finite field.
    /// @param field_deg Degree of the field modulus.
    /// @param coeffs List of coefficients with the first element corresponding to the constant
    /// term.
    explicit Polynomial(int field_deg, initializer_list<NTL::GF2X> coeffs) noexcept;

public:
    /// @brief Applies the function.
    /// @param x Point in which to evaluate.
    auto operator()(const NTL::GF2X& x) const noexcept -> NTL::GF2X override;

protected:
    unordered_map<size_t, NTL::GF2X> coeffs;
};

/// @brief Returns the bytes representing an element of GF(2)[x].
/// @param a An element of GF(2)[x].
auto bytes(const NTL::GF2X& elem) noexcept -> uint64_t;

/// @brief Constructs an element of GF(2)[x].
/// @param bytes Bytes representing of the coefficients with the least significant bit of the
/// first byte corresponding to the constant term.
auto make_elem(uint64_t bytes) noexcept -> NTL::GF2X;
