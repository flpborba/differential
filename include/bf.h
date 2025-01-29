#pragma once

#include <NTL/GF2X.h>
#include <mutex>
#include <vector>

/// @brief A tag used to dispatch calls to functions using a row lookup.
struct RowLookupTag { };

/// @brief Base class for functions over finite fields.
class Function {
public:
    /// @brief Returns the degree of the finite field in which the function is defined.
    auto field_degree() const noexcept -> int;

    /// @brief Returns the order of the finite field in which the function is defined.
    auto field_order() const noexcept -> size_t;

    /// @brief Applies the function.
    /// @param x Point in which to evaluate.
    virtual auto operator()(const NTL::GF2X& x) const noexcept -> NTL::GF2X = 0;

    /// @brief Computes the difference map f(x + a) + f(x) of this function.
    auto difference_map(const NTL::GF2X& x, const NTL::GF2X& a) const noexcept -> NTL::GF2X;

    /// @brief Computes the differential uniformity of this function.
    virtual auto uniformity() const noexcept -> size_t;

    /// @brief Computes the differential uniformity of this function using a lookup table.
    virtual auto uniformity(RowLookupTag) const noexcept -> size_t;

    /// @brief Computes the maximum number solutions for the difference map, with fixed `a`.
    /// @param a An element in the same field which this function is defined.
    auto row_max_delta(const NTL::GF2X& a) const noexcept -> size_t;

    /// @brief Computes the maximum number solutions for the difference map, with fixed `a`, using a
    /// lookup table.
    /// @param a An element in the same field which this function is defined.
    auto row_max_delta(const NTL::GF2X& a, RowLookupTag) const noexcept -> size_t;

    /// @brief Computes the number solutions for the difference map, with fixed `a` and `b`.
    /// @param a An element in the same field which this function is defined.
    /// @param b An element in the same field which this function is defined.
    auto delta(const NTL::GF2X& a, const NTL::GF2X& b) const noexcept -> size_t;

protected:
    /// @brief Constructs an instance of the inverse function over a finite field.
    /// @param field_deg Degree of the field modulus.
    explicit Function(int field_deg) noexcept;

public:
    NTL::GF2XModulus mod;
};

/// @brief The Dickson polynomial of the first kind.
class Dickson : public Function {
public:
    /// @brief Constructs a Dickson polynomial of the first kind.
    /// @param field_deg Degree of the field modulus.
    /// @param alpha Parameter used to construct the polynomial.
    Dickson(int field_deg, int deg, NTL::GF2X alpha) noexcept;

    /// @brief Checks whether this function is a permutation.
    auto is_permutation() const noexcept -> bool;

    /// @brief Applies the function.
    /// @param x Point in which to evaluate.
    auto operator()(const NTL::GF2X& x) const noexcept -> NTL::GF2X override;

    // protected:
    int deg;
    std::vector<std::pair<int, NTL::GF2X>> coeffs;
};

/// @brief The inverse function over a finite field.
class Inverse : public Function {
public:
    /// @brief Constructs an instance of the inverse function over a finite field.
    /// @param field_deg Degree of the field modulus.
    explicit Inverse(int field_deg) noexcept;

    /// @brief Applies the function.
    /// @param x Point in which to evaluate.
    auto operator()(const NTL::GF2X& x) const noexcept -> NTL::GF2X override;

    /// @brief Computes the differential uniformity of this function.
    auto uniformity() const noexcept -> size_t override;

    /// @brief Computes the differential uniformity of this function using a lookup table.
    auto uniformity(RowLookupTag) const noexcept -> size_t override;
};

/// @brief The function presented in Tu et al. (2018).
class Tu : public Function {
public:
    /// @brief Constructs an instance of the Tu function over a finite field.
    /// @param field_deg Degree of the field modulus.
    /// @param delta Value of delta used in the function.
    Tu(int field_deg, NTL::GF2X delta);

    /// @brief Applies this function.
    /// @param x Point in which to evaluate;
    auto operator()(const NTL::GF2X& x) const noexcept -> NTL::GF2X override;

protected:
    NTL::GF2X d;
};

template <typename T>
class Cached : public T {
public:
    /// @brief Constructs a cached instance of a function `T` over a finite field.
    /// @param field_deg Degree of the field modulus.
    /// @param args Extra arguments needed to construct function `T`.
    template <typename... Args>
    Cached(int field_deg, Args... args) noexcept(noexcept(T(field_deg, args...)));

    /// @brief Applies this function.
    /// @param x Point in which to evaluate;
    auto operator()(const NTL::GF2X& x) const noexcept -> NTL::GF2X override;

protected:
    mutable std::vector<std::pair<std::once_flag, NTL::GF2X>> cache;
};

/// @brief Returns the bytes representing a polynomial over GF(2)[x].
/// @param a A polynomial over GF(2)[x].
auto bytes(const NTL::GF2X& elem) noexcept -> uint64_t;

/// @brief Constructs a polynomial over GF(2)[x].
/// @param bytes Bytes representing of the coefficients with the least significant bit of the
/// first byte corresponding to the constant term.
auto make_elem(uint64_t bytes) noexcept -> NTL::GF2X;

template <typename T>
template <typename... Args>
Cached<T>::Cached(int field_deg, Args... args) noexcept(noexcept(T(field_deg, args...)))
    : T(field_deg, args...)
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
