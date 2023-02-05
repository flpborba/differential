#include "bf.h"
#include <numeric>

auto main(int argc, char* argv[]) -> int
{
    auto field_deg = std::stoi(argv[1]);
    auto field_order = 1 << field_deg;

    for (auto i = field_order - 1; i > 0; --i) {
        auto m = 1 << (2 * field_deg);
        if (std::gcd(i, m - 1) == 1) {
            Cached<Dickson>(field_deg, i, make_elem(0x1)).uniformity(RowLookupTag());
            return 0;
        }
    }
}
