#include <math.h>

#include <unordered_map>
#include <unordered_set>

#include "../lib/lib.h"

using namespace std;

// So here's the plan:
// Collect all antenna locations of the same frequency
// Compare all pairs of antennas
// Calculate their antinodes
// Put the antinodes into a set to eliminate duplicates
// ???
// Profit

struct coordinate {
    int x;
    int y;
    string to_string() {
        return std::to_string(x) + "," + std::to_string(y);
    }
};

bool is_out_of_bounds(const int &x, const int &y, const int &maxX, const int &maxY) {
    return x < 0 || y < 0 || x >= maxX || y >= maxY;
}

void calculate_antinodes(unordered_set<string> &antinodes, const coordinate &firstAntenna, const coordinate &secondAntenna, const int &maxX, const int &maxY) {
    int deltaX = secondAntenna.x - firstAntenna.x;
    int deltaY = secondAntenna.y - firstAntenna.y;

    coordinate result;
    result.x = secondAntenna.x + deltaX;
    result.y = secondAntenna.y + deltaY;

    if (!is_out_of_bounds(result.x, result.y, maxX, maxY))
        antinodes.insert(result.to_string());

    // Flip the vector by multiplying by -1 (definitely didn't have to look this up)
    result.x = firstAntenna.x + deltaX * -1;
    result.y = firstAntenna.y + deltaY * -1;

    if (!is_out_of_bounds(result.x, result.y, maxX, maxY))
        antinodes.insert(result.to_string());
}

void calculate_antinodes_with_harmonics(unordered_set<string> &antinodes, coordinate &firstAntenna, coordinate &secondAntenna, const int &maxX, const int &maxY) {
    antinodes.insert(firstAntenna.to_string());
    antinodes.insert(secondAntenna.to_string());

    int deltaX = secondAntenna.x - firstAntenna.x;
    int deltaY = secondAntenna.y - firstAntenna.y;

    coordinate result;
    result.x = secondAntenna.x + deltaX;
    result.y = secondAntenna.y + deltaY;

    while (!is_out_of_bounds(result.x, result.y, maxX, maxY)) {
        antinodes.insert(result.to_string());
        result.x += deltaX;
        result.y += deltaY;
    }

    // Flip the vector by multiplying by -1 (definitely didn't have to look this up)
    deltaX *= -1;
    deltaY *= -1;

    result.x = firstAntenna.x + deltaX;
    result.y = firstAntenna.y + deltaY;

    while (!is_out_of_bounds(result.x, result.y, maxX, maxY)) {
        antinodes.insert(result.to_string());
        result.x += deltaX;
        result.y += deltaY;
    }
}

int main(int argc, char const *argv[]) {
    lib::timer timer;
    unordered_map<string, vector<coordinate>> antennas;
    int maxY, maxX;

    lib::read_file(argv[1], [&antennas, &maxX, &maxY](const string line, const int y) {
        auto positions = lib::split(line, "");

        for (int x = 0; x < positions.size(); x++) {
            string frequency = positions[x];

            if (frequency == ".") continue;

            antennas[frequency].push_back({x, y});
        }

        maxX = (int)positions.size();
        maxY = y;
    });

    unordered_set<string> antinodes;

    timer.start();

    for (auto kvp : antennas) {
        auto coordinates = kvp.second;
        for (size_t i = 0; i < coordinates.size() - 1; i++) {
            for (size_t j = i + 1; j < coordinates.size(); j++) {
                calculate_antinodes(antinodes, coordinates[i], coordinates[j], maxX, maxY + 1);
            }
        }
    }

    timer.stop();

    cout << "Result: " << antinodes.size() << '\n';

    // PART II

    antinodes.clear();

    timer.start();

    for (auto kvp : antennas) {
        auto coordinates = kvp.second;
        for (size_t i = 0; i < coordinates.size() - 1; i++) {
            for (size_t j = i + 1; j < coordinates.size(); j++) {
                calculate_antinodes_with_harmonics(antinodes, coordinates[i], coordinates[j], maxX, maxY + 1);
            }
        }
    }

    timer.stop();

    cout << "Result: " << antinodes.size() << '\n';

    return 0;
}
