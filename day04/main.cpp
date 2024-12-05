#include "../lib/lib.h"

using namespace std;

const string letters[3]{"M", "A", "S"};

bool check_match(const vector<vector<string>> &map, function<tuple<const int, const int>(int)> func) {
    bool isMatch = true;
    int x, y;

    for (size_t i = 1; i <= 3; i++) {
        tie(x, y) = func(i);

        isMatch = map[y][x] == letters[i - 1];

        if (!isMatch)
            break;
    }

    return isMatch;
}

int check_cardinal_directions(const vector<vector<string>> &map, const int &x, const int &y) {
    int result = 0;

    // Up
    if (y >= 3)
        result += check_match(map, [&x, &y](int i) { return make_tuple(x, y - i); });

    // Down
    if (y < map.size() - 3)
        result += check_match(map, [&x, &y](int i) { return make_tuple(x, y + i); });

    // Left
    if (x >= 3)
        result += check_match(map, [&x, &y](int i) { return make_tuple(x - i, y); });

    // Right
    if (x < map[0].size() - 3)
        result += check_match(map, [&x, &y](int i) { return make_tuple(x + i, y); });

    return result;
}

int check_diagonals(const vector<vector<string>> &map, const int &x, const int &y) {
    int result = 0;

    // Up-Left
    if (y >= 3 && x >= 3)
        result += check_match(map, [&x, &y](int i) { return make_tuple(x - i, y - i); });

    // Down-Left
    if (y < map.size() - 3 && x >= 3)
        result += check_match(map, [&x, &y](int i) { return make_tuple(x - i, y + i); });

    // Up-Right
    if (y >= 3 && x < map[0].size() - 3)
        result += check_match(map, [&x, &y](int i) { return make_tuple(x + i, y - i); });

    // Down-Right
    if (y < map.size() - 3 && x < map[0].size() - 3)
        result += check_match(map, [&x, &y](int i) { return make_tuple(x + i, y + i); });

    return result;
}

int check_directions(const vector<vector<string>> &map, const int &x, const int &y) {
    return check_cardinal_directions(map, x, y) + check_diagonals(map, x, y);
}

int main(int argc, char const *argv[]) {
    lib::timer timer;
    vector<vector<string>> map;

    lib::read_file(argv[1], [&map](const string &line, int i) {
        auto chars = lib::split(line, "");
        map.push_back(chars);
    });

    timer.start();

    int result = 0;

    for (size_t y = 0; y < map.size(); y++) {
        for (size_t x = 0; x < map[0].size(); x++) {
            if (map[y][x] == "X")
                result += check_directions(map, x, y);
        }
    }

    timer.stop();

    cout << "Result: " << result << '\n';

    return 0;
}
