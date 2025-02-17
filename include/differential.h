#pragma once

#include <NTL/GF2X.h>

class Differential {
public:
    /// @brief Constructs an instance of an algorithm to calculate the differential uniformity of a
    /// function.
    /// @param f Function to calculate the differential uniformity.
    explicit Differential(const Function& f) noexcept;

public:
    /// @brief Computes the differential uniformity of the underlying function.
    virtual auto uniformity() noexcept -> size_t;

    /// @brief Computes the maximum preimage size among all `b` under the derivative of the
    /// underlying function with respect to `a`.
    virtual auto max_preimage_size(NTL::GF2X a) noexcept -> size_t;

    /// @brief Computes the preimage size of `b` under the derivative of the underlying function
    /// with respect to `a`.
    virtual auto preimage_size(NTL::GF2X a, NTL::GF2X b) noexcept -> size_t;

    /// @brief Checks if `x` is in the preimage of `b` under the derivative of the underlying
    /// function with respect to `a`.
    virtual auto is_solution(NTL::GF2X x, NTL::GF2X a, NTL::GF2X b) noexcept -> bool;

protected:
    const Function& f;
};

/// @brief Function object for returning the greater of two values.
struct Max {
    /// @brief Returns the greater of the given values.
    auto operator()(size_t a, size_t b) const noexcept -> size_t;
};
