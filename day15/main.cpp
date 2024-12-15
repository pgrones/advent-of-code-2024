#include <algorithm>

#include "../lib/lib.h"

using namespace std;

int WIDTH;
int HEIGHT;

struct coordinate {
    int x;
    int y;

    bool operator==(const coordinate &other) const {
        return x == other.x && y == other.y;
    }
};

int calc_GPS_sum(const vector<vector<char>> &map) {
    int res = 0;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (map[y][x] != 'O' && map[y][x] != '[') {
                continue;
            }

            res += 100 * y + x;
        }
    }

    return res;
}

void print_map(const vector<vector<char>> &map, const coordinate &robot) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == robot.x && y == robot.y) {
                cout << "@";
                continue;
            }

            cout << map[y][x];
        }
        cout << '\n';
    }
    cout << "--------------------------------------------------\n";
}

void update_map(vector<vector<char>> &map, const coordinate &currPos, const coordinate &delta) {
    char temp = map[currPos.y][currPos.x];
    map[currPos.y][currPos.x] = '.';
    map[currPos.y + delta.y][currPos.x + delta.x] = temp;
}

void make_move(vector<vector<char>> &map, coordinate &robot, const char &move) {
    coordinate delta = {0, 0};

    switch (move) {
        case '<':
            delta.x = -1;
            break;
        case '>':
            delta.x = 1;
            break;
        case '^':
            delta.y = -1;
            break;
        case 'v':
            delta.y = 1;
            break;
    }

    coordinate curr = {robot.x + delta.x, robot.y + delta.y};
    vector<coordinate> box_coords;
    while (map[curr.y][curr.x] == 'O') {
        box_coords.insert(box_coords.begin(), {curr.x, curr.y});
        curr.x += delta.x;
        curr.y += delta.y;
    }

    if (map[curr.y][curr.x] == '#') {
        return;
    }

    // update all boxes between start and curr
    for (auto coord : box_coords) {
        update_map(map, coord, delta);
    }

    // update robot
    robot.x += delta.x;
    robot.y += delta.y;
}

void make_move_2(vector<vector<char>> &map, coordinate &robot, const char &move) {
    coordinate delta = {0, 0};

    switch (move) {
        case '<':
            delta.x = -1;
            break;
        case '>':
            delta.x = 1;
            break;
        case '^':
            delta.y = -1;
            break;
        case 'v':
            delta.y = 1;
            break;
    }

    coordinate curr = {robot.x + delta.x, robot.y + delta.y};
    vector<coordinate> box_coords;

    // if the robot isn't moving up or down we can do the same as in part 1
    if (delta.y == 0) {
        while (map[curr.y][curr.x] == '[' || map[curr.y][curr.x] == ']') {
            box_coords.insert(box_coords.begin(), {curr.x, curr.y});
            curr.x += delta.x;
        }

        if (map[curr.y][curr.x] == '#') {
            return;
        }
    } else {
        vector<int> x_coords = {curr.x};

        while (!x_coords.empty()) {
            vector<int> next_x_coords;

            for (auto x_coord : x_coords) {
                if (map[curr.y][x_coord] == '#') {
                    return;
                }

                if (map[curr.y][x_coord] == '[') {
                    next_x_coords.push_back(x_coord);
                    next_x_coords.push_back(x_coord + 1);
                    box_coords.insert(box_coords.begin(), {x_coord, curr.y});
                    box_coords.insert(box_coords.begin(), {x_coord + 1, curr.y});
                }

                if (map[curr.y][x_coord] == ']') {
                    coordinate find_coord = {x_coord, curr.y};
                    // if coord not already added
                    if (find(box_coords.begin(), box_coords.end(), find_coord) == box_coords.end()) {
                        next_x_coords.push_back(x_coord - 1);
                        next_x_coords.push_back(x_coord);
                        box_coords.insert(box_coords.begin(), {x_coord - 1, curr.y});
                        box_coords.insert(box_coords.begin(), {x_coord, curr.y});
                    }
                }
            }

            curr.y += delta.y;
            x_coords = next_x_coords;
        }
    }

    // update all boxes between start and curr
    for (auto coord : box_coords) {
        update_map(map, coord, delta);
    }

    // update robot
    robot.x += delta.x;
    robot.y += delta.y;
}

int main(int argc, char const *argv[]) {
    lib::timer timer;
    vector<vector<char>> map;
    vector<char> moves;
    coordinate robot;

    bool isMoves = false;
    lib::read_file(argv[1], [&map, &robot, &moves, &isMoves](const string line, const int i) {
        if (line == "") {
            isMoves = true;
            return;
        }

        if (isMoves) {
            copy(line.begin(), line.end(), back_inserter(moves));
            return;
        }

        vector<char> row;

        for (int j = 0; j < line.length(); j++) {
            if (line[j] == '@') {
                robot = {j, i};
                row.push_back('.');
                continue;
            }

            row.push_back(line[j]);
        }

        map.push_back(row);
    });

    HEIGHT = map.size();
    WIDTH = map[0].size();
    vector<vector<char>> initial_map = map;
    coordinate inital_robot = robot;

    timer.start();

    for (auto move : moves) {
        // print_map(map, robot);
        make_move(map, robot, move);
    }

    // print_map(map, robot);
    int result = calc_GPS_sum(map);

    timer.stop();

    cout << "Result: " << result << '\n';

    // PART II

    result = 0;
    coordinate robot2 = {inital_robot.x * 2, inital_robot.y};
    vector<vector<char>> map2;

    for (int y = 0; y < HEIGHT; y++) {
        vector<char> line;
        for (int x = 0; x < WIDTH; x++) {
            switch (initial_map[y][x]) {
                case '#':
                    line.push_back('#');
                    line.push_back('#');
                    break;
                case 'O':
                    line.push_back('[');
                    line.push_back(']');
                    break;
                default:
                    line.push_back('.');
                    line.push_back('.');
                    break;
            }
        }
        map2.push_back(line);
    }

    WIDTH = map2[0].size();

    timer.start();

    for (size_t i = 0; i < moves.size(); i++) {
        auto move = moves[i];
        // cout << "Move " << move << ":\n";
        // print_map(map2, robot2);
        make_move_2(map2, robot2, move);
    }

    // print_map(map2, robot2);
    result = calc_GPS_sum(map2);

    timer.stop();

    cout << "Result: " << result << '\n';

    return 0;
}