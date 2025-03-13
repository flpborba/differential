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

public:
    /// @brief Applies the function.
    /// @param x Point in which to evaluate.
    auto operator()(const NTL::GF2X& x) const noexcept -> NTL::GF2X override;

protected:
    std::vector<std::pair<size_t, NTL::GF2X>> terms;
};

/// @brief A polynomial function over a finite field.
class Monomial : public Polynomial {
public:
    /// @brief Constructs an instance of a mononomial function over a finite field.
    /// @param field_deg Degree of the field modulus.
    /// @param deg Degree of the monomial.
    /// @param coeff Monomial coefficient.
    explicit Monomial(size_t field_deg, size_t deg, NTL::GF2X coeff) noexcept;
};

template <typename T>
class Cached : public T {
public:
    /// @brief Constructs a cached instance of a function `T` over a finite field.
    /// @param field_deg Degree of the field modulus.
    /// @param args Extra arguments needed to construct function `T`.
    template <typename... Arg>
    Cached(Arg&&... args) noexcept(noexcept(T(std::forward<Arg>(args)...)));

public:
    /// @brief Applies this function.
    /// @param x Point in which to evaluate;
    auto operator()(const NTL::GF2X& x) const noexcept -> NTL::GF2X override;

protected:
    mutable std::vector<std::pair<std::once_flag, NTL::GF2X>> cache;
};

/// @brief Returns the bytes representing an element of GF(2)[x].
/// @param elem An element of GF(2)[x].
auto bytes(const NTL::GF2X& elem) noexcept -> uint64_t;

/// @brief Constructs an element of GF(2)[x].
/// @param coefficients Bytes representing of the coefficients with the least significant bit of the
/// first byte corresponding to the constant term.
auto make_elem(uint64_t coefficients) noexcept -> NTL::GF2X;

template <typename T>
template <typename... Arg>
Cached<T>::Cached(Arg&&... args) noexcept(noexcept(T(std::forward<Arg>(args)...)))
    : T(std::forward<Arg>(args)...)
    , cache(T::field_order())
{
}

template <typename T>
auto Cached<T>::operator()(const NTL::GF2X& x) const noexcept -> NTL::GF2X
{
    const auto n = bytes(x);

    std::call_once(cache[n].first, [this, &x, n]() {
        cache[n].second = T::operator()(x);
    });

    return cache[n].second;
}
