#include <algorithm>
#include <unordered_set>

#include "../lib/lib.h"

using namespace std;

struct fence {
    int x;
    int y;
    char dir;
};

struct dimensions {
    int area;
    int perimeter;
    vector<fence> fences;
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
        dimensions.fences.push_back({position.x, position.y - 1, 'u'});
    } else {
        auto traversedPlots = get_dimensions(garden, {position.x, position.y - 1});
        dimensions.area += traversedPlots.area;
        dimensions.perimeter += traversedPlots.perimeter;
        copy(traversedPlots.fences.begin(), traversedPlots.fences.end(), back_inserter(dimensions.fences));
    }

    // Right
    if (position.x == garden[0].size() - 1 || garden[position.y][position.x + 1] != type) {
        dimensions.perimeter++;
        dimensions.fences.push_back({position.x + 1, position.y, 'r'});
    } else {
        auto traversedPlots = get_dimensions(garden, {position.x + 1, position.y});
        dimensions.area += traversedPlots.area;
        dimensions.perimeter += traversedPlots.perimeter;
        copy(traversedPlots.fences.begin(), traversedPlots.fences.end(), back_inserter(dimensions.fences));
    }

    // Down
    if (position.y == garden.size() - 1 || garden[position.y + 1][position.x] != type) {
        dimensions.perimeter++;
        dimensions.fences.push_back({position.x, position.y + 1, 'd'});
    } else {
        auto traversedPlots = get_dimensions(garden, {position.x, position.y + 1});
        dimensions.area += traversedPlots.area;
        dimensions.perimeter += traversedPlots.perimeter;
        copy(traversedPlots.fences.begin(), traversedPlots.fences.end(), back_inserter(dimensions.fences));
    }

    // Left
    if (position.x == 0 || garden[position.y][position.x - 1] != type) {
        dimensions.perimeter++;
        dimensions.fences.push_back({position.x - 1, position.y, 'l'});
    } else {
        auto traversedPlots = get_dimensions(garden, {position.x - 1, position.y});
        dimensions.area += traversedPlots.area;
        dimensions.perimeter += traversedPlots.perimeter;
        copy(traversedPlots.fences.begin(), traversedPlots.fences.end(), back_inserter(dimensions.fences));
    }

    return dimensions;
}

int count_horizontal_sides_in_same_dir(vector<fence> &fences) {
    int result = 0;

    sort(fences.begin(), fences.end(), [](fence &a, fence &b) {
        return a.y == b.y ? a.x < b.x : a.y < b.y;
    });

    do {
        result++;
        fence currFence = fences[0];
        fences.erase(fences.begin());

        while (!fences.empty() && currFence.y == fences[0].y && currFence.x == fences[0].x - 1) {
            currFence = fences[0];
            fences.erase(fences.begin());
        }
    } while (!fences.empty());

    return result;
}

int count_vertical_sides_in_same_dir(vector<fence> &fences) {
    int result = 0;

    sort(fences.begin(), fences.end(), [](fence &a, fence &b) {
        return a.x == b.x ? a.y < b.y : a.x < b.x;
    });

    do {
        result++;
        fence currFence = fences[0];
        fences.erase(fences.begin());

        while (!fences.empty() && currFence.x == fences[0].x && currFence.y == fences[0].y - 1) {
            currFence = fences[0];
            fences.erase(fences.begin());
        }
    } while (!fences.empty());

    return result;
}

int count_sides(vector<fence> &fences) {
    int result = 0;
    vector<fence> sameDirFences;

    copy_if(fences.begin(), fences.end(), back_inserter(sameDirFences), [](const fence &fence) {
        return fence.dir == 'u';
    });

    if (!sameDirFences.empty())
        result += count_horizontal_sides_in_same_dir(sameDirFences);

    copy_if(fences.begin(), fences.end(), back_inserter(sameDirFences), [](const fence &fence) {
        return fence.dir == 'd';
    });

    if (!sameDirFences.empty())
        result += count_horizontal_sides_in_same_dir(sameDirFences);

    copy_if(fences.begin(), fences.end(), back_inserter(sameDirFences), [](const fence &fence) {
        return fence.dir == 'l';
    });

    if (!sameDirFences.empty())
        result += count_vertical_sides_in_same_dir(sameDirFences);

    copy_if(fences.begin(), fences.end(), back_inserter(sameDirFences), [](const fence &fence) {
        return fence.dir == 'r';
    });

    if (!sameDirFences.empty())
        result += count_vertical_sides_in_same_dir(sameDirFences);

    return result;
}

int main(int argc, char const *argv[]) {
    lib::timer timer;
    vector<vector<char>> garden;

    lib::read_file(argv[1], [&garden](const string line, const int _) {
        vector<char> plots(line.begin(), line.end());
        garden.push_back(plots);
    });

    vector<pair<int, vector<fence>>> fences;
    int result = 0;

    timer.start();

    for (int y = 0; y < garden.size(); y++) {
        for (int x = 0; x < garden[0].size(); x++) {
            auto dimensions = get_dimensions(garden, {x, y});

            if (dimensions.area > 0) {
                result += dimensions.area * dimensions.perimeter;
                fences.push_back({dimensions.area, dimensions.fences});
            }
        }
    }

    timer.stop();

    cout << "Result: " << result << '\n';

    // PART II

    result = 0;

    timer.start();

    for (auto fence : fences) {
        result += fence.first * count_sides(fence.second);
    }

    timer.stop();

    cout << "Result: " << result << '\n';

    return 0;
}