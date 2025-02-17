#pragma once

#include <NTL/GF2X.h>
#include <vector>

/// @brief Base class for functions over GF(2)[x].
class Function {
protected:
    /// @brief Constructs an instance of a function over GF(2)[x].
    /// @param field_deg Degree of the field modulus.
    explicit Function(size_t field_deg) noexcept;

public:
    /// @brief Returns the degree of the finite field in which the function is defined.
    auto field_degree() const noexcept -> size_t;

    /// @brief Returns the order of the finite field in which the function is defined.
    auto field_order() const noexcept -> size_t;

    /// @brief Computes the derivative of this function with respect to `a`.
    auto derivative(const NTL::GF2X& x, const NTL::GF2X& a) const noexcept -> NTL::GF2X;

    /// @brief Applies the function.
    /// @param x Point in which to evaluate.
    virtual auto operator()(const NTL::GF2X& x) const noexcept -> NTL::GF2X = 0;

protected:
    NTL::GF2XModulus mod;
};

/// @brief A polynomial function over a finite field.
class Polynomial : public Function {
public:
    /// @brief Constructs an instance of a polynomial function over a finite field.
    /// @param field_deg Degree of the field modulus.
    /// @param coeffs List of coefficients' with the first element corresponding to the constant
    /// term.
    explicit Polynomial(size_t field_deg, std::initializer_list<NTL::GF2X> coeffs) noexcept;

    /// @brief Applies the function.
    /// @param x Point in which to evaluate.
    auto operator()(const NTL::GF2X& x) const noexcept -> NTL::GF2X override;

protected:
    std::vector<std::pair<size_t, NTL::GF2X>> terms;
};

/// @brief Returns the bytes representing an element of GF(2)[x].
/// @param elem An element of GF(2)[x].
auto bytes(const NTL::GF2X& elem) noexcept -> uint64_t;

/// @brief Constructs an element of GF(2)[x].
/// @param coefficients Bytes representing of the coefficients with the least significant bit of the
/// first byte corresponding to the constant term.
auto make_elem(uint64_t coefficients) noexcept -> NTL::GF2X;
