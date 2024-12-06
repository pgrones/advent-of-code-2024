#include <math.h>

#include <unordered_set>

#include "../lib/lib.h"

using namespace std;

tuple<size_t, size_t> get_next_pos(const char &dir, const size_t &x, const size_t &y) {
    size_t nextX = x;
    size_t nextY = y;

    switch (dir) {
        case 'u':
            nextY--;
            break;
        case 'd':
            nextY++;
            break;
        case 'l':
            nextX--;
            break;
        case 'r':
            nextX++;
            break;
    }

    return {nextX, nextY};
}

char turn_right(const char &dir) {
    switch (dir) {
        case 'u':
            return 'r';
        case 'd':
            return 'l';
        case 'l':
            return 'u';
        case 'r':
            return 'd';
    }

    return dir;
}

bool is_out_of_bounds(vector<vector<string>> &map, const size_t &x, const size_t &y) {
    return x < 0 || y < 0 || x >= map[0].size() || y >= map.size();
}

tuple<size_t, size_t, char> take_step(vector<vector<string>> &map, const char &dir, const size_t &x, const size_t &y) {
    size_t nextX, nextY;

    tie(nextX, nextY) = get_next_pos(dir, x, y);

    if (is_out_of_bounds(map, nextX, nextY))
        return {nextX, nextY, dir};

    char nextDir = dir;

    while (map[nextY][nextX] == "#") {
        nextDir = turn_right(nextDir);

        tie(nextX, nextY) = get_next_pos(nextDir, x, y);

        if (is_out_of_bounds(map, nextX, nextY))
            return {nextX, nextY, nextDir};
    }

    return {nextX, nextY, nextDir};
}

tuple<size_t, size_t> find_start(vector<vector<string>> &map) {
    for (size_t y = 0; y < map.size(); y++) {
        for (size_t x = 0; x < map[0].size(); x++) {
            if (map[y][x] == "^")
                return {x, y};
        }
    }

    return {-1, -1};
}

int main(int argc, char const *argv[]) {
    lib::timer timer;
    vector<vector<string>> map;

    lib::read_file(argv[1], [&map](const string line, const int _) {
        map.push_back(lib::split(line, ""));
    });

    size_t result, x, y, startX, startY;
    char dir = 'u';
    unordered_set<string> positions;

    timer.start();

    tie(startX, startY) = find_start(map);
    x = startX;
    y = startY;

    while (!is_out_of_bounds(map, x, y)) {
        positions.insert(to_string(x) + "," + to_string(y));
        tie(x, y, dir) = take_step(map, dir, x, y);
    }

    result = positions.size();

    timer.stop();

    cout << "Result: " << result << '\n';

    // PART II

    result = 0;

    timer.start();

    for (size_t i = 0; i < map.size(); i++) {
        cout << round(((float)i / (float)map.size()) * 100) << "%" << '\n';

        for (size_t j = 0; j < map[0].size(); j++) {
            if (map[i][j] == "#")
                continue;

            x = startX;
            y = startY;
            dir = 'u';
            positions.clear();

            map[i][j] = "#";

            while (!is_out_of_bounds(map, x, y)) {
                auto res = positions.insert(to_string(x) + "," + to_string(y) + lib::ctos(dir));

                if (!res.second) {
                    result++;
                    break;
                }

                tie(x, y, dir) = take_step(map, dir, x, y);
            }

            map[i][j] = ".";
        }
    }

    timer.stop();

    cout << "Result: " << result << '\n';

    return 0;
}
