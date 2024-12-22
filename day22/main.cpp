#include <math.h>

#include <unordered_set>

#include "../lib/lib.h"
using namespace std;

constexpr int N = 2000;

struct monkey {
    unsigned long long bananas;
    unsigned long long difference;

    string to_string() const {
        return std::to_string(difference);
    }
};

string get_sequence(const vector<monkey> &monkeys, const size_t &start) {
    string hash;

    for (size_t i = 0; i < 4; i++) {
        hash += monkeys[start + i].to_string();
    }

    return hash;
}

int mix(unsigned long long secret_number, int value) {
    return secret_number ^ value;
}

int prune(unsigned long long secret_number) {
    return secret_number % 16777216;
}

int next(unsigned long long secret_number) {
    unsigned long long result = prune(mix(secret_number, secret_number * 64));
    result = prune(mix(result, result / 32));
    return prune(mix(result, result * 2048));
}

int main(int argc, char const *argv[]) {
    lib::timer timer;
    vector<unsigned long long> secret_numbers;

    lib::read_file(argv[1], [&secret_numbers](const string line, const int _) {
        secret_numbers.push_back(stoull(line));
    });

    unsigned long long result = 0;

    timer.start();

    for (auto secret_number : secret_numbers) {
        for (size_t i = 0; i < N; i++) {
            secret_number = next(secret_number);
        }

        result += secret_number;
    }

    timer.stop();

    cout << "Result: " << result << '\n';

    // PART II

    result = 0;

    timer.start();

    unordered_map<string, unsigned long long> bananas;

    for (auto secret_number : secret_numbers) {
        vector<monkey> monkeys;

        for (size_t i = 0; i < N + 1; i++) {
            unsigned long long bananas = secret_number % 10;
            secret_number = next(secret_number);
            monkeys.push_back({bananas, (secret_number % 10) - bananas});
        }

        unordered_set<string> distinct_sequences_monkey;

        for (size_t window_start = 0; window_start < N - 4; window_start++) {
            string sequence = get_sequence(monkeys, window_start);

            if (distinct_sequences_monkey.insert(sequence).second) {
                bananas[sequence] += monkeys[window_start + 4].bananas;
            }
        }
    }

    for (auto banana : bananas) {
        result = max(banana.second, result);
    }

    timer.stop();

    cout << "BANANANANANAAAAAAS: " << result << '\n';

    return 0;
}