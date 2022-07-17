#include "bf.h"

auto main(int argc, char* argv[]) -> int
{
    auto field_deg = std::stoi(argv[1]);
    Inverse(field_deg).uniformity(RowLookupTag());
}
