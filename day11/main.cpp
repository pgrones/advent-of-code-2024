#include <math.h>

#include <algorithm>
#include <unordered_map>

#include "../lib/lib.h"

using namespace std;

tuple<long long, long long> apply_rule(const long long stone) {
    if (stone == 0) {
        return {1, -1};
    }

    int digits = floor(log10(stone)) + 1;
    if (digits % 2 == 0) {
        return {(long long)floor(stone / pow(10, digits / 2)), stone % (int)pow(10, digits / 2)};
    }

    return {stone * 2024, -1};
}

vector<long long> calculate_stones(const vector<long long>& stones) {
    vector<long long> result;
    long long one, two;

    for (auto stone : stones) {
        tie(one, two) = apply_rule(stone);

        result.push_back(one);

        if (two != -1)
            result.push_back(two);
    }

    return result;
}

long long calculate_stones_fast(const vector<long long>& initialStones) {
    unordered_map<long long, long long> stonesByAmount;

    for (auto stone : initialStones) {
        stonesByAmount[stone]++;
    }

    vector<pair<long long, long long>> entries;
    long long one, two;

    for (size_t i = 0; i < 75; i++) {
        for (const auto& [stone, amount] : stonesByAmount) {
            if (amount > 0) {
                entries.emplace_back(stone, amount);
            }
        }

        for (const auto& [stone, amount] : entries) {
            stonesByAmount[stone] -= amount;

            tie(one, two) = apply_rule(stone);

            stonesByAmount[one] += amount;

            if (two != -1)
                stonesByAmount[two] += amount;
        }

        entries.clear();
    }

    long long result = 0;
    for (const auto& [_, amount] : stonesByAmount) {
        result += amount;
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

    timer.start();

    auto result = calculate_stones_fast(initialStones);

    timer.stop();

    cout << "Result: " << result << '\n';

    return 0;
}