#include <unordered_set>

#include "../lib/lib.h"

using namespace std;

struct dimensions {
    int area;
    int perimeter;
};

struct coordinate {
    int x;
    int y;
    string to_string() {
        return std::to_string(x) + "," + std::to_string(y);
    }
};

unordered_set<string> visited;

dimensions get_dimensions(const vector<vector<char>> &garden, coordinate position) {
    if (!visited.insert(position.to_string()).second) {
        return {0, 0};
    }

    char type = garden[position.y][position.x];
    dimensions dimensions{1, 0};

    // Up
    if (position.y == 0 || garden[position.y - 1][position.x] != type) {
        dimensions.perimeter++;
    } else {
        auto traversedPlots = get_dimensions(garden, {position.x, position.y - 1});
        dimensions.area += traversedPlots.area;
        dimensions.perimeter += traversedPlots.perimeter;
    }

    // Right
    if (position.x == garden[0].size() - 1 || garden[position.y][position.x + 1] != type) {
        dimensions.perimeter++;
    } else {
        auto traversedPlots = get_dimensions(garden, {position.x + 1, position.y});
        dimensions.area += traversedPlots.area;
        dimensions.perimeter += traversedPlots.perimeter;
    }

    // Down
    if (position.y == garden.size() - 1 || garden[position.y + 1][position.x] != type) {
        dimensions.perimeter++;
    } else {
        auto traversedPlots = get_dimensions(garden, {position.x, position.y + 1});
        dimensions.area += traversedPlots.area;
        dimensions.perimeter += traversedPlots.perimeter;
    }

    // Left
    if (position.x == 0 || garden[position.y][position.x - 1] != type) {
        dimensions.perimeter++;
    } else {
        auto traversedPlots = get_dimensions(garden, {position.x - 1, position.y});
        dimensions.area += traversedPlots.area;
        dimensions.perimeter += traversedPlots.perimeter;
    }

    return dimensions;
}

int main(int argc, char const *argv[]) {
    lib::timer timer;
    vector<vector<char>> garden;

    lib::read_file(argv[1], [&garden](const string line, const int _) {
        vector<char> plots(line.begin(), line.end());
        garden.push_back(plots);
    });

    int result = 0;

    timer.start();

    for (int y = 0; y < garden.size(); y++) {
        for (int x = 0; x < garden[0].size(); x++) {
            auto dimensions = get_dimensions(garden, {x, y});

            result += dimensions.area * dimensions.perimeter;
        }
    }

    timer.stop();

    cout << "Result: " << result << '\n';

    // PART II

    result = 0;

    timer.start();

    // do stuff

    timer.stop();

    cout << "Result: " << result << '\n';

    return 0;
}