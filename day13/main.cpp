#include <numeric>
#include <regex>
#include <cmath>
#include <cassert>

#include <climits>

#include "../lib/lib.h"

using namespace std;

struct coordinate {
    int x;
    int y;
};

struct claw_machine {
    coordinate a;
    coordinate b;
    coordinate prize;
};

double solve(claw_machine m, bool partII = false) {
    //  c1 * ax + c2 * bx == px
    //  c1 * ay + c2 * by == py

    double ax = m.a.x;
    double ay = m.a.y;
    double bx = m.b.x;
    double by = m.b.y;

    double px = m.prize.x;
    double py = m.prize.y;

    if (partII) {
        // part II
        px += 10000000000000;
        py += 10000000000000;
    }

    assert(ax != bx || ay != by);

    // [ax bx; ay by] (c1 c2) == p
    // c2 = 1/bx (px - c1 ax)
    // c1 = 1 / ay (py - by/bx (px - c1 ax))
    // c1 (1 - 1/ay by/bx ax) = 1/ay (py - by/bx px)
    double c1 = 1 / (1 - 1/ay * by/bx * ax) * 1/ay * (py - by/bx * px);
    double c2 = 1/bx * (px - c1 * ax);

    // printf("%f, %f\n", c1, c2);
    // printf("check: (%f, %f) ?= (%f, %f)\n\n", c1*ax+c2*bx, c1*ay+c2*by, px, py);

    // check if int (rounding)
    if (!(abs(c1 - (unsigned long long)round(c1)) < 1e-2 && abs(c2 - (unsigned long long)round(c2)) < 1e-2)) return 0;
    // not negative
    if (c1 < 0 || c2 < 0) return 0;
    // not larger than 100
    // if (c1 > 100 || c2 > 100) return 0;
    
    return 3 * round(c1) + round(c2);
}

int main(int argc, char const *argv[]) {
    lib::timer timer;
    vector<claw_machine> clawMachines = {{}};

    const regex pattern(R"~((\d+).+?(\d+))~");
    int lineNr = 0;
    lib::read_file(argv[1], [&clawMachines, &lineNr, pattern](const string line, const int i) {
        if (line == "") {
            clawMachines.push_back({});
            lineNr = 0;
            return;
        }

        smatch matches;
        regex_search(line, matches, pattern);

        if (lineNr == 0)
            clawMachines.back().a = {stoi(matches[1]), stoi(matches[2])};

        if (lineNr == 1)
            clawMachines.back().b = {stoi(matches[1]), stoi(matches[2])};

        if (lineNr == 2)
            clawMachines.back().prize = {stoi(matches[1]), stoi(matches[2])};

        lineNr++;
    });

    double result = 0;

    timer.start();

    for (auto clawMachine : clawMachines) {
        result += solve(clawMachine);
    }

    timer.stop();

    cout << "Result: " << result << '\n';

    // PART II

    result = 0;

    timer.start();

    for (auto clawMachine : clawMachines) {
        result += solve(clawMachine, true);
    }

    timer.stop();

    printf("Result: %.0f\n", result);

    return 0;
}