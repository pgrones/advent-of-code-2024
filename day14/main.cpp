#include <math.h>

#include <regex>
#include <thread>
#include <unordered_set>

#include "../lib/lib.h"

using namespace std;

int constexpr WIDTH = 101;
int constexpr HEIGHT = 103;

struct robot {
    int id;
    int x;
    int y;
    int vx;
    int vy;
};

void step(robot &robot) {
    robot.x += robot.vx;
    robot.y += robot.vy;

    if (robot.x >= WIDTH)
        robot.x -= WIDTH;

    if (robot.x < 0)
        robot.x += WIDTH;

    if (robot.y >= HEIGHT)
        robot.y -= HEIGHT;

    if (robot.y < 0)
        robot.y += HEIGHT;
}

void draw(const vector<robot> &robots) {
    for (size_t y = 0; y < HEIGHT; y++) {
        for (size_t x = 0; x < WIDTH; x++) {
            int robotCount = 0;

            for (auto &&robot : robots) {
                if (robot.x == x && robot.y == y)
                    robotCount++;
            }

            if (robotCount == 0)
                cout << '.';
            else
                cout << robotCount;
        }

        cout << "\n";
    }

    cout << "\n\n";
}

int count_neighbors(const vector<robot> &robots) {
    unordered_set<int> robots_w_neighbors;

    for (size_t x = 1; x < robots.size(); x++) {
        for (size_t y = 0; y < x; y++) {
            bool has_neighbor = false;

            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) continue;

                    if (robots[x].x == robots[y].x + i && robots[x].y == robots[y].y + j) {
                        robots_w_neighbors.insert(robots[x].id);
                        robots_w_neighbors.insert(robots[y].id);
                        has_neighbor = true;
                        break;
                    }
                }

                if (has_neighbor) break;
            }
        }
    }

    return robots_w_neighbors.size();
}

int main(int argc, char const *argv[]) {
    lib::timer timer;
    vector<robot> robots;

    const regex pattern(R"~((-?\d+,-?\d+).*?(-?\d+,-?\d+))~");
    lib::read_file(argv[1], [&robots, &pattern](const string line, const int i) {
        smatch matches;
        regex_search(line, matches, pattern);
        string numbersCommaSeparated = matches[1].str() + ',' + matches[2].str();
        auto numbers = lib::map<string, int>(lib::split(numbersCommaSeparated, ","), [](const string item, const int _) { return stoi(item); });
        robots.push_back({i, numbers[0], numbers[1], numbers[2], numbers[3]});
    });

    vector<robot> initalRobots = robots;

    int result = 0;

    timer.start();

    for (size_t i = 0; i < 100; i++) {
        for (size_t j = 0; j < robots.size(); j++) {
            step(robots[j]);
        }
    }

    int quadrants[4] = {0};
    int xCenter = floor(WIDTH / 2);
    int yCenter = floor(HEIGHT / 2);

    for (auto robot : robots) {
        if (robot.x < xCenter) {
            if (robot.y < yCenter)
                quadrants[0]++;
            else if (robot.y > yCenter)
                quadrants[2]++;
        } else if (robot.x > xCenter) {
            if (robot.y < yCenter)
                quadrants[1]++;
            else if (robot.y > yCenter)
                quadrants[3]++;
        }
    }

    result = quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3];

    timer.stop();

    cout << "Result: " << result << '\n';

    // PART II

    result = 0;
    robots = initalRobots;

    timer.start();

    int yay_xmas_moment = 0;
    int const threshold = robots.size() - 150;
    while (true) {
        // check whether "most" robots have a neighbor. If they do, we're happy and done.
        if (count_neighbors(robots) > threshold) {
            break;
        }

        // step all robots
        for (size_t j = 0; j < robots.size(); j++) {
            step(robots[j]);
        }

        yay_xmas_moment++;
    }

    draw(robots);

    result = yay_xmas_moment;

    timer.stop();

    cout << "Result: " << result << '\n';

    return 0;
}