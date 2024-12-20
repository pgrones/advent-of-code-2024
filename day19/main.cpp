#include <unordered_map>
#include <unordered_set>

#include "../lib/lib.h"

using namespace std;

bool realizable(const string &design, const vector<string> &patterns, unordered_set<string> &seen) {
    // return if we have already searched this (sub-)design and haven't found an end
    if (!seen.insert(design).second) return false;

    for (auto pattern : patterns) {
        // reached the end of the design, bingo
        if (design == pattern) return true;

        // design does not start with pattern
        if (design.rfind(pattern, 0) != 0) continue;

        // cut off the found pattern from the start and search through the rest
        if (realizable(design.substr(pattern.size()), patterns, seen)) return true;
    }

    return false;
}

long long count_designs(const string &design, const vector<string> &patterns, unordered_map<string, long long> &seen) {
    // return the calculated count if we have already searched this (sub-)design
    if (seen.find(design) != seen.end()) return seen[design];

    long long result = 0;

    for (auto pattern : patterns) {
        // reached the end of the design
        if (design == pattern) {
            result++;
            continue;
        }

        // design does not start with pattern
        if (design.rfind(pattern, 0) != 0) continue;

        // cut off the found pattern from the start and search through the rest
        result += count_designs(design.substr(pattern.size()), patterns, seen);
    }

    // add the count to the map so that it can be reused if we come across this design again
    seen[design] = result;

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

    long long result = 0;
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
    unordered_map<string, long long> seen_map;

    timer.start();

    for (auto design : designs) {
        result += count_designs(design, patterns, seen_map);
        seen_map.clear();
    }

    timer.stop();

    cout << "Result: " << result << '\n';

    return 0;
}