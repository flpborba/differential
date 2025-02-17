#pragma once

struct Hash {
    template <typename... Args>
    auto operator()(const std::tuple<Args...>& t) const -> size_t
    {
        return 0;
    }
};
