#include "../lib/lib.h"

using namespace std;

int main(int argc, char const *argv[]) {
    lib::timer timer;
    vector<vector<int>> keys;
    vector<vector<int>> locks;

    int key_or_lock = 0;
    lib::read_file(argv[1], [&keys, &locks, &key_or_lock](const string line, const int _) {
        if (line == "") {
            key_or_lock = 0;
            return;
        }

        if (key_or_lock == 0) {
            key_or_lock = line.find('.') == line.npos ? 1 : -1;

            if (key_or_lock == 1) {
                locks.push_back({0, 0, 0, 0, 0});
                return;
            }

            keys.push_back({-1, -1, -1, -1, -1});
            return;
        }

        for (size_t i = 0; i < line.length(); i++) {
            if (line[i] != '#') continue;

            if (key_or_lock == 1) {
                locks.back()[i]++;
                continue;
            }

            keys.back()[i]++;
        }
    });

    int result = 0;

    timer.start();

    for (auto key : keys) {
        for (auto lock : locks) {
            bool fits = true;

            for (size_t i = 0; i < 5; i++) {
                if (key[i] + lock[i] > 5) {
                    fits = false;
                    break;
                }
            }

            result += fits;
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