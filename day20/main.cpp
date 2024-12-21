#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "../lib/lib.h"

using namespace std;

vector<vector<string>> map;
size_t WIDTH, HEIGHT;

struct node {
    int x;
    int y;
    int fScore;

    bool operator==(const node &other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const node &other) const {
        return x != other.x || y != other.y;
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

node startPos, endPos;

vector<node> get_neighbors(const node &curr) {
    vector<node> allNeighbors{{curr.x, curr.y - 1}, {curr.x, curr.y + 1}, {curr.x - 1, curr.y}, {curr.x + 1, curr.y}};
    vector<node> neighbors;

    copy_if(allNeighbors.begin(), allNeighbors.end(), back_inserter(neighbors), [](node n) {
        return n.x >= 0 && n.y >= 0 && n.x < WIDTH && n.y < HEIGHT;
    });

    return neighbors;
}

unordered_map<string, int> dijkstra(const node &start, const int &radius) {
    priority_queue<node, vector<node>, greater<node>> openSet;
    openSet.push(start);

    vector<node> closedSet;
    unordered_map<string, int> gScore;

    while (!openSet.empty()) {
        // get next node from open set
        auto curr = openSet.top();
        openSet.pop();

        // add to closed set (best value is known)
        closedSet.push_back(curr);

        // path found, DONE
        // if (curr != start && map[curr.y][curr.x] == "." ) continue;
        if (gScore[curr.to_string()] >= radius) continue;

        for (auto neighbor : get_neighbors(curr)) {
            // neighbor already in closed list, skip
            if (find(closedSet.begin(), closedSet.end(), neighbor) != closedSet.end()) continue;

            int pathLength = gScore[curr.to_string()] + 1;
            string neighborKey = neighbor.to_string();

            // if neighbor is in open set and new path is not better, continue
            if (gScore.find(neighbor.to_string()) == gScore.end() || pathLength < gScore[neighbor.to_string()]) {
                // update g value
                gScore[neighborKey] = pathLength;
                // update f value
                neighbor.fScore = pathLength;
                openSet.push(neighbor);
            }
        }
    }

    return gScore;
}

void print_map(unordered_map<string, int> &way_to_go) {
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[0].size(); x++) {
            node c = {x, y};

            if (way_to_go.find(c.to_string()) != way_to_go.end()) {
                string a = to_string(way_to_go[c.to_string()]);
                cout << a.substr(a.size() - 1);
                continue;
            }

            cout << map[y][x];
        }
        cout << '\n';
    }
}

node get_neighbor(const node &curr, const unordered_map<string, int> &distances) {
    node neighbors[4]{{curr.x, curr.y + 1}, {curr.x, curr.y - 1}, {curr.x + 1, curr.y}, {curr.x - 1, curr.y}};

    for (auto neighbor : neighbors) {
        if (map[neighbor.y][neighbor.x] == "#") continue;
        if (distances.find(neighbor.to_string()) != distances.end()) continue;

        return neighbor;
    }

    return {-1, -1};
}

pair<vector<node>, unordered_map<string, int>> find_path() {
    unordered_map<string, int> way_to_go;
    vector<node> path;
    auto curr = endPos;
    int dist = 0;

    while (curr != startPos) {
        way_to_go[curr.to_string()] = dist++;
        path.insert(path.begin(), curr);
        curr = get_neighbor(curr, way_to_go);
    }

    // add start pos
    way_to_go[curr.to_string()] = dist;
    path.insert(path.begin(), curr);

    return {path, way_to_go};
}

int cheat(const vector<node> &path, unordered_map<string, int> &way_to_go, const int max_cheat_length) {
    int good_cheats = 0, cheat;
    size_t cheat_length;
    string coordKey;

    for (auto coord : path) {
        auto gScores = dijkstra(coord, max_cheat_length);

        for (auto wall_or_behind_wall : gScores) {
            tie(coordKey, cheat_length) = wall_or_behind_wall;
            lib::coordinate c(coordKey);

            if (map[c.y][c.x] == ".") {
                cheat = way_to_go[coord.to_string()] - way_to_go[coordKey] - cheat_length;
                if (cheat >= 100) good_cheats++;
            }
        }
    }

    return good_cheats;
}

int main(int argc, char const *argv[]) {
    lib::timer timer;

    lib::read_file(argv[1], [](const string line, const int y) {
        map.push_back(lib::split(line, ""));

        for (int x = 0; x < line.size(); x++) {
            if (line[x] == 'S') {
                startPos = {x, y};
                map[y][x] = ".";
            }

            if (line[x] == 'E') {
                endPos = {x, y};
                map[y][x] = ".";
            }
        }
    });

    HEIGHT = map.size();
    WIDTH = map[0].size();

    timer.start();

    auto res = find_path();
    auto path = res.first;
    auto way_to_go = res.second;

    int result = cheat(path, way_to_go, 2);

    timer.stop();

    cout << "Result: " << result << '\n';

    // PART II

    result = 0;

    timer.start();

    result = cheat(path, way_to_go, 20);

    timer.stop();

    cout << "Result: " << result << '\n';

    return 0;
}