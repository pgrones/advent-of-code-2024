#include <math.h>

#include <fstream>
#include <unordered_map>
#include <unordered_set>

#include "../lib/lib.h"

using namespace std;

struct coordinate {
    int x;
    int y;

    bool operator==(const coordinate &other) const {
        return x == other.x && y == other.y;
    }

    string to_string() {
        return std::to_string(x) + "," + std::to_string(y);
    }
};

vector<vector<char>> map;
coordinate start;
coordinate endPos;
coordinate endPosLeft;
coordinate endPosDown;
unordered_map<string, int> minLengths;
unordered_set<string> shortestPathNodes;

void print_map() {
    ofstream out_file;
    out_file.open("out.txt");

    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[0].size(); x++) {
            coordinate c = {x, y};
            if (shortestPathNodes.find(c.to_string()) != shortestPathNodes.end()) {
                out_file << 'O';
                continue;
            }

            out_file << map[y][x];
        }
        out_file << '\n';
    }

    out_file.close();
}

int get_global_min_length() {
    return min(minLengths[endPos.to_string() + endPosLeft.to_string()], minLengths[endPos.to_string() + endPosDown.to_string()]);
}

int evaluate(coordinate &currPos, const coordinate &lastPos, coordinate nextPos, unordered_set<string> &path, int pathLength);

int traverse(coordinate &currPos, coordinate lastPos, unordered_set<string> &path, int pathLength = 0) {
    int minLength = get_global_min_length();

    if (pathLength > minLength) return minLength;

    string key = currPos.to_string() + lastPos.to_string();
    if (minLengths[key] != 0 && pathLength > minLengths[key]) return minLength;

    minLengths[key] = pathLength;

    if (currPos == endPos) {
        if (minLength > pathLength) {
            shortestPathNodes = path;
        } else {
            for (auto node : path) {
                shortestPathNodes.insert(node);
            }
        }

        return pathLength;
    }

    int eval = minLength;

    eval = evaluate(currPos, lastPos, {currPos.x, currPos.y - 1}, path, pathLength);
    eval = evaluate(currPos, lastPos, {currPos.x + 1, currPos.y}, path, pathLength);
    eval = evaluate(currPos, lastPos, {currPos.x, currPos.y + 1}, path, pathLength);
    eval = evaluate(currPos, lastPos, {currPos.x - 1, currPos.y}, path, pathLength);

    return eval;
}

bool is_turn(const coordinate &currPos, const coordinate &lastPos, const coordinate &nextPos) {
    int dx = currPos.x - lastPos.x;
    int dy = currPos.y - lastPos.y;
    int ndx = nextPos.x - currPos.x;
    int ndy = nextPos.y - currPos.y;

    return dx != ndx || dy != ndy;
}

int evaluate(coordinate &currPos, const coordinate &lastPos, coordinate nextPos, unordered_set<string> &path, int pathLength) {
    int minLength = get_global_min_length();

    if (lastPos == nextPos) return minLength;
    if (map[nextPos.y][nextPos.x] == '#') return minLength;

    if (!path.insert(nextPos.to_string()).second) return minLength;

    int weight = 1;
    if (is_turn(currPos, lastPos, nextPos)) weight += 1000;

    int result = traverse(nextPos, currPos, path, pathLength + weight);

    path.erase(nextPos.to_string());

    return result;
}

int main(int argc, char const *argv[]) {
    lib::timer timer;

    lib::read_file(argv[1], [](const string line, const int y) {
        vector<char> row;

        for (int x = 0; x < line.size(); x++) {
            char tile = line[x];

            if (tile == '#') {
                row.push_back(tile);
                continue;
            }

            row.push_back('.');

            if (tile == 'S') start = {x, y};

            if (tile == 'E') {
                endPos = {x, y};
                endPosLeft = {x - 1, y};
                endPosDown = {x, y + 1};

                minLengths[endPos.to_string() + endPosLeft.to_string()] = INT_MAX;
                minLengths[endPos.to_string() + endPosDown.to_string()] = INT_MAX;
            }
        }

        map.push_back(row);
    });

    int result = 0;
    unordered_set<string> path;

    timer.start();

    result = traverse(start, {start.x - 1, start.y}, path);

    timer.stop();

    cout << "Result: " << result << '\n';

    // PART II
    print_map();

    cout << "Result: " << shortestPathNodes.size() + 1 << '\n';

    return 0;
}