#pragma once

/// @brief Function object for returning the greater of two values.
struct Max {
    /// @brief Returns the greater of the given values.
    auto operator()(size_t a, size_t b) const noexcept -> size_t;
};
