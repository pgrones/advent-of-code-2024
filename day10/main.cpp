#include <unordered_set>

#include "../lib/lib.h"

using namespace std;

struct coordinate {
    int x;
    int y;
    string to_string() {
        return std::to_string(x) + "," + std::to_string(y);
    }
};

int traverse(coordinate currPos, const vector<vector<int>> &map, unordered_set<string> &seen, const bool all = false) {
    if (map[currPos.y][currPos.x] == 9) {
        return (all || seen.insert(currPos.to_string()).second) ? 1 : 0;
    }

    int result = 0;

    // Up
    if (currPos.y > 0 && map[currPos.y][currPos.x] == map[currPos.y - 1][currPos.x] - 1) {
        result += traverse({currPos.x, currPos.y - 1}, map, seen, all);
    }

    // Right
    if (currPos.x < map[0].size() - 1 && map[currPos.y][currPos.x] == map[currPos.y][currPos.x + 1] - 1) {
        result += traverse({currPos.x + 1, currPos.y}, map, seen, all);
    }

    // Down
    if (currPos.y < map.size() - 1 && map[currPos.y][currPos.x] == map[currPos.y + 1][currPos.x] - 1) {
        result += traverse({currPos.x, currPos.y + 1}, map, seen, all);
    }

    // Left
    if (currPos.x > 0 && map[currPos.y][currPos.x] == map[currPos.y][currPos.x - 1] - 1) {
        result += traverse({currPos.x - 1, currPos.y}, map, seen, all);
    }

    return result;
}

int traverse(coordinate currPos, const vector<vector<int>> &map) {
    unordered_set<string> seen;

    return traverse(currPos, map, seen);
}

int traverse_all(coordinate currPos, const vector<vector<int>> &map) {
    unordered_set<string> seen;

    return traverse(currPos, map, seen, true);
}

int main(int argc, char const *argv[]) {
    lib::timer timer;
    vector<vector<int>> map;

    lib::read_file(argv[1], [&map](const string line, const int _) {
        map.push_back(lib::map<string, int>(lib::split(line, ""), [](const string item, const int _) { return stoi(item); }));
    });

    int result = 0;

    timer.start();

    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[0].size(); x++) {
            if (!map[y][x] == 0) continue;

            result += traverse({x, y}, map);
        }
    }

    timer.stop();

    cout << "Result: " << result << '\n';

    // PART II

    result = 0;

    timer.start();

    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[0].size(); x++) {
            if (!map[y][x] == 0) continue;

            result += traverse_all({x, y}, map);
        }
    }

    timer.stop();

    cout << "Result: " << result << '\n';

    return 0;
}