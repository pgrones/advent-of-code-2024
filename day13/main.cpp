#include <numeric>
#include <regex>

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

struct depth {
    int a;
    int b;
};

int find_fewest_tokens(const claw_machine &claw_machine, coordinate position = {0, 0}, depth depth = {0, 0}, int tokens = 0, int minTokens = INT_MAX) {
    if (depth.a >= 100 || depth.b >= 100) return minTokens;

    if (position.x > claw_machine.prize.x || position.y > claw_machine.prize.y) return minTokens;

    if (minTokens <= tokens) return minTokens;

    if (position.x == claw_machine.prize.x && position.y == claw_machine.prize.y) return tokens;

    int eval = find_fewest_tokens(claw_machine, {position.x + claw_machine.b.x, position.y + claw_machine.b.y}, {depth.a, depth.b + 1}, tokens + 1, minTokens);

    if (minTokens != INT_MAX && eval >= minTokens) return minTokens;

    return find_fewest_tokens(claw_machine, {position.x + claw_machine.a.x, position.y + claw_machine.a.y}, {depth.a + 1, depth.b}, tokens + 3, eval);
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

    int result = 0;

    timer.start();

    for (auto clawMachine : clawMachines) {
        int minTokens = INT_MAX;

        for (int b = 0; b < 100; b++) {
            for (int a = 0; a < 100; a++) {
                int x = b * clawMachine.b.x + a * clawMachine.a.x;
                int y = b * clawMachine.b.y + a * clawMachine.a.y;

                if (x > clawMachine.prize.x || y > clawMachine.prize.y) break;

                int tokens = b + a * 3;

                if (tokens >= minTokens) break;

                if (x == clawMachine.prize.x && y == clawMachine.prize.y)
                    minTokens = tokens;
            }
        }

        if (minTokens != INT_MAX) result += minTokens;
    }

    // for (auto clawMachine : clawMachines) {
    //     int minTokens = find_fewest_tokens(clawMachine);

    //     if (minTokens != INT_MAX)
    //         result += minTokens;
    // }

    timer.stop();

    cout << "Result: " << result << '\n';

    // PART II

    result = 0;

    timer.start();

    int i = 0;
    for (auto clawMachine : clawMachines) {
        // Can only be solved if the result is divisible by the gcd of (a,b)
        // I fell down a deep rabbit hole: https://en.m.wikipedia.org/wiki/Diophantine_equation
        // ax + by = c {a,b,c ints}

        // v This doesn't matter because even if both x and y are divisible, the results still need to line up on the same multiplier
        long long gx = gcd(clawMachine.a.x, clawMachine.b.x);
        long long gy = gcd(clawMachine.a.y, clawMachine.b.y);

        if (gx == 1 || gy == 1 || (clawMachine.prize.x + 10000000000000) % gx != 0LL || (clawMachine.prize.y + 10000000000000) % gy != 0LL)
            continue;

        cout << i << '\n';
        i++;
    }

    timer.stop();

    cout << "Result: " << result << '\n';

    return 0;
}