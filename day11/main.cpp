#include <math.h>

#include <algorithm>
#include <unordered_set>

#include "../lib/lib.h"

using namespace std;

vector<long long> calculate_stones(const vector<long long>& stones) {
    vector<long long> result;

    for (auto stone : stones) {
        if (stone == 0) {
            result.push_back(1);
            continue;
        }

        int digits = floor(log10(stone)) + 1;
        if (digits % 2 == 0) {
            result.push_back(floor(stone / pow(10, digits / 2)));
            result.push_back(stone % (int)pow(10, digits / 2));
            continue;
        }

        result.push_back(stone * 2024);
    }

    return result;
}

int main(int argc, char const* argv[]) {
    lib::timer timer;
    vector<long long> stones;

    lib::read_file(argv[1], [&stones](const string line, const int _) {
        stones = lib::map<string, long long>(lib::split(line, " "), [](const string stone, const int _) { return stoll(stone); });
    });

    vector<long long> initialStones = stones;

    timer.start();

    for (size_t i = 0; i < 25; i++) {
        stones = calculate_stones(stones);
    }

    timer.stop();

    cout << "Result: " << stones.size() << '\n';

    // PART II

    long result = 0;

    timer.start();

    stones = initialStones;

    // long long counts[10];

    // for (long long i = 0; i < 10; i++) {
    //     vector<long long> stones = {i};

    //     for (size_t j = 0; j < 75; j++) {
    //         stones = calculate_stones(stones);
    //     }

    //     counts[i] = stones.size();
    // }

    // for (auto item : counts) {
    //     cout << item << ", ";
    // }

    // cout << "\n";

    unordered_set<long long> skipped;

    for (size_t i = 0; i < 75; i++) {
        vector<long long> tempStones;

        cout << i << ", " << stones.size() << '\n';
        cout << *max_element(stones.begin(), stones.end()) << '\n';

        for (auto stone : stones) {
            if (stone == 0) {
                skipped.insert(stone);
                continue;
            }

            int digits = floor(log10(stone)) + 1;
            if (digits % 2 == 0) {
                // if (digits % 4 == 0) {
                //     skipped.insert(stone);
                //     continue;
                // }

                tempStones.push_back(floor(stone / pow(10, digits / 2)));
                tempStones.push_back(stone % (int)pow(10, digits / 2));
                continue;
            }

            tempStones.push_back(stone * 2024);
        }

        stones = tempStones;
    }

    vector<long long> v(skipped.begin(), skipped.end());
    lib::print_collection(v);

    timer.stop();

    cout << "Result: " << stones.size() << '\n';

    return 0;
}