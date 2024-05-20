#include "bf.h"
#include "difference_map.h"

auto main(int argc, char* argv[]) -> int
{
    auto begin = 1;
    auto end = 20;
    auto deg = 19;
    auto uniformity = deg - 1;
    auto a = make_elem(0x01);

    for (auto i = begin; i != end; ++i) {
        auto dickson = Cached<Dickson>(i, deg, a);
        auto uniformity = dickson.uniformity(RowLookupTag());

        if (uniformity == deg - 1) {
            std::cout << i << ": .\n";
        } else {
            std::cout << i << ": " << uniformity << "\n";
        }
    }
}
