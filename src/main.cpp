#include "bf.h"
#include <NTL/GF2EX.h>
#include <tbb/tbb.h>
#include <map>

auto main(int argc, char* argv[]) -> int
{
    // tbb::global_control c(tbb::global_control::max_allowed_parallelism, 1);

    // auto field_deg = std::stoi(argv[1]);
    const auto start_field_deg = 1;
    const auto end_field_deg = 16;

    // auto f = Inverse(8);
    auto f = Cached<Dickson>(7, 5, make_elem(0x1));
    auto v = std::vector<NTL::GF2X>();
    auto s = std::map<int, int>();

    for (auto i = 0; i < f.field_order(); ++i) {
        auto e = make_elem(i);
        auto size = 0;

        while (std::find(v.begin(), v.end(), e) == v.end()) {
            v.push_back(e);
            ++size;

            e = f(e);
        }

        if (size != 0) {
            auto o = s[size];
            s[size] = o + 1;
        }
    }

    for (auto e : s) {
        std::cout << e.first << ": " << e.second << '\n';
    }

    std::cout << '\n';

    // auto d = Cached<Dickson>(3, 7, make_elem(0x1));

    // std::cout << d.mod << '\n';

    // f(0) = 0

    // const auto x = make_elem(6);

    // std::cout << NTL::PowerMod(x, 0, d.mod) << '\n';
    // std::cout << NTL::PowerMod(x, 1, d.mod) << '*' << '\n';
    // std::cout << NTL::PowerMod(x, 2, d.mod) << '\n';
    // std::cout << NTL::PowerMod(x, 3, d.mod) << '\n';
    // std::cout << NTL::PowerMod(x, 4, d.mod) << '\n';
    // std::cout << NTL::PowerMod(x, 5, d.mod) << '*' << '\n';
    // std::cout << NTL::PowerMod(x, 6, d.mod) << '\n';
    // std::cout << NTL::PowerMod(x, 7, d.mod) << '*' << '\n';

    // 011
    // 010
    // 001

    // f(x) = ?
    // 0: 1
    // 1: x
    // 2: x²
    // 3: x³ -> x + 1
    // 4: x² + x
    // 5: x³ + x² -> x² + x + 1
    // 6: x³ + x² + x -> x² + 1
    // 7: x³ + x -> 1

    // NTL::GF2E::init(d.mod);

    // auto d0 = NTL::GF2EX();

    // NTL::SetCoeff(d0, 7);
    // NTL::SetCoeff(d0, 5);
    // NTL::SetCoeff(d0, 1);

    // auto d1 = NTL::diff(d0);

    // auto d2 = NTL::GCD(d1, d0);

    // std::cout << d0 << '\n';
    // std::cout << d1 << '\n';
    // std::cout << d2 << '\n';

    // const auto du = d.uniformity();

    // std::cout << du << '\n';

    // for (auto i = start_field_deg; i < end_field_deg; ++i) {
    //     auto d = Cached<Dickson>(i, 11, make_elem(0x1));
    //     const auto du = d.uniformity(RowLookupTag());

    //     std::cout << d.deg << ' ';

    //     for (auto c : d.coeffs) {
    //         std::cout << c.first << ' ';
    //     }
    //     std::cout << '\n';

    //     std::cout << du << '\n';

    // if (d.is_permutation()) {
    //     std::cout << " & \\underline{\\textbf{" << i << "}}";
    // } else {
    //     std::cout << " & " << i;
    // }
    // }
    // std::cout
    //     << " \\\\\n";
    // std::cout << "done!\n";
}
