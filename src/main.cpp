#include "bf.h"

auto main(int argc, char* argv[]) -> int
{
    auto field_deg = std::stoi(argv[1]);
    Cached<Inverse>(field_deg).uniformity(RowLookupTag());
}
