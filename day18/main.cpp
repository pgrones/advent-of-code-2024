#include <math.h>

#include <algorithm>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "../lib/lib.h"

using namespace std;

constexpr int HEIGHT = 71;
constexpr int WIDTH = 71;
constexpr int BYTES = 1024;
vector<vector<char>> map;

void populate_map(const vector<lib::coordinate> &positions) {
    vector<lib::coordinate> bytes;

    for (int byte = 0; byte < BYTES; byte++) {
        bytes.push_back(positions[byte]);
    }

    for (int y = 0; y < HEIGHT; y++) {
        vector<char> row;

        for (int x = 0; x < WIDTH; x++) {
            lib::coordinate coord = {x, y};
            if (find(bytes.begin(), bytes.end(), coord) != bytes.end()) {
                row.push_back('#');
                continue;
            }

            row.push_back('.');
        }

        map.push_back(row);
    }
}

struct node {
    int x;
    int y;
    int fScore;

    int manhattan(const node &end) const {
        return abs(x - end.x) + abs(y - end.y);
    }

    bool operator==(const node &other) const {
        return x == other.x && y == other.y;
    }

    bool operator<(const node &other) const {
        return fScore < other.fScore;
    }

    bool operator>(const node &other) const {
        return fScore > other.fScore;
    }

    string to_string() const {
        return std::to_string(x) + "," + std::to_string(y);
    }
};

constexpr node endNode = {WIDTH - 1, HEIGHT - 1};

vector<node> get_neighbors(const node &curr) {
    vector<node> allNeighbors{{curr.x, curr.y - 1}, {curr.x, curr.y + 1}, {curr.x - 1, curr.y}, {curr.x + 1, curr.y}};
    vector<node> neighbors;

    copy_if(allNeighbors.begin(), allNeighbors.end(), back_inserter(neighbors), [](node n) {
        if (n.x < 0 || n.y < 0 || n.x >= WIDTH || n.y >= HEIGHT) return false;
        if (map[n.y][n.x] == '#') return false;

        return true;
    });

    return neighbors;
}

int a_star(bool partII = false) {
    const node start{0, 0, 0};

    priority_queue<node, vector<node>, greater<node>> openSet;
    openSet.push(start);

    vector<node> closedSet;
    unordered_map<string, int> gScore;

    while (!openSet.empty()) {
        // get next node from open set
        auto curr = openSet.top();
        openSet.pop();

        // path found, DONE
        if (curr == endNode) return gScore[endNode.to_string()];

        // add to closed set (best value is known)
        closedSet.push_back(curr);

        for (auto neighbor : get_neighbors(curr)) {
            if (partII && neighbor == endNode) return 1;

            // neighbor already in closed list, skip
            if (find(closedSet.begin(), closedSet.end(), neighbor) != closedSet.end()) continue;

            int pathLength = gScore[curr.to_string()] + 1;
            string neighborKey = neighbor.to_string();

            // if neighbor is in open set and new path is not better, continue
            if (gScore.find(neighbor.to_string()) == gScore.end() || pathLength < gScore[neighbor.to_string()]) {
                // update g value
                gScore[neighborKey] = pathLength;
                // update f value
                neighbor.fScore = pathLength + neighbor.manhattan(endNode);
                openSet.push(neighbor);
            }
        }
    }

    return 0;
}

int main(int argc, char const *argv[]) {
    lib::timer timer;
    vector<lib::coordinate> positions;

    lib::read_file(argv[1], [&positions](const string line, const int _) {
        auto coords = lib::map<string, int>(lib::split(line, ","), [](const string item, const int _) { return stoi(item); });
        positions.push_back({coords[0], coords[1]});
    });

    int result = 0;
    unordered_set<string> path;

    populate_map(positions);

    timer.start();

    result = a_star();

    timer.stop();

    cout << "Result: " << result << '\n';

    // PART II

    timer.start();

    // binary search
    size_t l = BYTES, r = positions.size();
    while (l <= r) {
        int mid = l + (r - l) / 2;
        auto map_curr = map;

        // test mid
        for (size_t i = BYTES; i < mid; i++) {
            auto coord = positions[i];
            map[coord.y][coord.x] = '#';
        }

        if (a_star(true))
            l = mid + 1;
        else
            r = mid - 1;

        map = map_curr;
    }

    timer.stop();

    cout << "Result: " << positions[r].to_string() << '\n';

    return 0;
}