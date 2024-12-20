#include <unordered_map>
#include <unordered_set>

#include "../lib/lib.h"

using namespace std;

vector<vector<string>> map;
lib::coordinate startPos, endPos;
size_t WIDTH, HEIGHT;

void print_map(unordered_map<string, int> &way_to_go) {
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[0].size(); x++) {
            lib::coordinate c = {x, y};

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

lib::coordinate get_neighbor(const lib::coordinate &curr, const unordered_map<string, int> &distances) {
    lib::coordinate neighbors[4]{{curr.x, curr.y + 1}, {curr.x, curr.y - 1}, {curr.x + 1, curr.y}, {curr.x - 1, curr.y}};

    for (auto neighbor : neighbors) {
        if (map[neighbor.y][neighbor.x] == "#") continue;
        if (distances.find(neighbor.to_string()) != distances.end()) continue;

        return neighbor;
    }

    return {-1, -1};
}

pair<vector<lib::coordinate>, unordered_map<string, int>> find_path() {
    unordered_map<string, int> way_to_go;
    vector<lib::coordinate> path;
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

void find_paths_through_walls(const lib::coordinate &curr, const lib::coordinate &start, unordered_set<string> &path, unordered_map<string, size_t> &neighbors_behind_walls, const int max_cheat_length) {
    if (curr == start) return;
    if (path.size() > max_cheat_length) return;
    if (curr.x < 0 || curr.y < 0 || curr.x >= WIDTH || curr.y >= HEIGHT) return;

    if (map[curr.y][curr.x] == ".") {
        neighbors_behind_walls[curr.to_string() + ";" + to_string(path.size())] = path.size();
        return;
    }

    lib::coordinate neighbors[4]{{curr.x, curr.y + 1}, {curr.x, curr.y - 1}, {curr.x + 1, curr.y}, {curr.x - 1, curr.y}};

    for (auto neighbor : neighbors) {
        if (!path.insert(neighbor.to_string()).second) continue;
        find_paths_through_walls(neighbor, start, path, neighbors_behind_walls, max_cheat_length);
        path.erase(neighbor.to_string());
    }
}

unordered_map<string, size_t> get_neighbors_behind_walls(const lib::coordinate &curr, const int max_cheat_length) {
    unordered_map<string, size_t> neighbors_behind_walls;
    unordered_set<string> path;

    vector<lib::coordinate> neighbors{{curr.x, curr.y + 1}, {curr.x, curr.y - 1}, {curr.x + 1, curr.y}, {curr.x - 1, curr.y}};
    vector<lib::coordinate> walls;

    copy_if(neighbors.begin(), neighbors.end(), back_inserter(walls), [](const lib::coordinate coord) { return map[coord.y][coord.x] == "#"; });

    for (auto wall : walls) {
        path.insert(wall.to_string());
        find_paths_through_walls(wall, curr, path, neighbors_behind_walls, max_cheat_length);
        path.erase(wall.to_string());
    }

    return neighbors_behind_walls;
}

int cheat(const vector<lib::coordinate> &path, unordered_map<string, int> &way_to_go, const int max_cheat_length) {
    int good_cheats = 0, cheat;
    size_t cheat_length;
    string coordKey;

    for (auto coord : path) {
        for (auto behind_wall : get_neighbors_behind_walls(coord, max_cheat_length)) {
            tie(coordKey, cheat_length) = behind_wall;
            coordKey = coordKey.substr(0, coordKey.find(';'));
            cheat = way_to_go[coord.to_string()] - way_to_go[coordKey] - cheat_length;

            if (cheat >= 72) good_cheats++;
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