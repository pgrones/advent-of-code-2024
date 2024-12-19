#include <unordered_set>

#include "../lib/lib.h"

using namespace std;

bool realizable(const string &design, const vector<string> &patterns, unordered_set<string> &seen) {
    if (!seen.insert(design).second) return false;

    for (auto pattern : patterns) {
        if (design == pattern) return true;

        if (design.rfind(pattern, 0) != 0) continue;

        if (realizable(design.substr(pattern.size()), patterns, seen)) return true;
    }

    return false;
}

int count_designs(const string &design, const vector<string> &patterns, unordered_set<string> &seen) {
    if (!seen.insert(design).second) return 0;

    int result = 0;

    for (auto pattern : patterns) {
        if (design == pattern) {
            result++;
            continue;
        }

        if (design.rfind(pattern, 0) != 0) continue;

        result += count_designs(design.substr(pattern.size()), patterns, seen);
    }

    return result;
}

int main(int argc, char const *argv[]) {
    lib::timer timer;
    vector<string> patterns;
    vector<string> designs;

    bool isDesigns = false;
    lib::read_file(argv[1], [&patterns, &designs, &isDesigns](const string line, const int _) {
        if (line == "") {
            isDesigns = true;
            return;
        }

        if (!isDesigns) {
            patterns = lib::split(line, ", ");
            return;
        }

        designs.push_back(line);
    });

    int result = 0;
    unordered_set<string> seen;

    timer.start();

    for (auto design : designs) {
        result += realizable(design, patterns, seen);
        seen.clear();
    }

    timer.stop();

    cout << "Result: " << result << '\n';

    // PART II

    result = 0;
    seen.clear();

    timer.start();

    for (auto design : designs) {
        result += count_designs(design, patterns, seen);
        seen.clear();
    }

    timer.stop();

    cout << "Result: " << result << '\n';

    return 0;
}