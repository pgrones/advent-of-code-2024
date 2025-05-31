#include <algorithm>
#include <cmath>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "../lib/lib.h"

using namespace std;

struct key {
    char key;
    lib::coordinate coordinate;
};

vector<key> numericKeys = {
    {'7', {0, 0}},
    {'8', {1, 0}},
    {'9', {2, 0}},
    {'4', {0, 1}},
    {'5', {1, 1}},
    {'6', {2, 1}},
    {'1', {0, 2}},
    {'2', {1, 2}},
    {'3', {2, 2}},
    {'0', {1, 3}},
    {'A', {2, 3}},
};

vector<key> directionalKeys{
    {'^', {1, 0}},
    {'A', {2, 0}},
    {'<', {0, 1}},
    {'v', {1, 1}},
    {'>', {2, 1}},
};

map<pair<char, char>, vector<string>> optimalSequences;

void precompute_optimal_sequences() {
    for (int i = 0; i < numericKeys.size(); i++) {
        for (int j = 0; j < numericKeys.size(); j++) {
            if (i == j) continue;

            auto start = numericKeys[i].coordinate;
            auto end = numericKeys[j].coordinate;
            int deltaX = end.x - start.x;
            int deltaY = end.y - start.y;
            bool panics = min(start.x, end.x) == 0 && max(start.y, end.y) == 3;

            vector<string> sequences;
            string sequence;

            // Right then down
            if (!signbit(deltaX)) {
                for (int k = 0; k < deltaX; k++) {
                    sequence.push_back('>');
                }
            }

            if (!signbit(deltaY)) {
                for (int k = 0; k < deltaY; k++) {
                    sequence.push_back('v');
                }
            }

            // Up then left
            if (signbit(deltaY)) {
                for (int k = 0; k < abs(deltaY); k++) {
                    sequence.push_back('^');
                }
            }

            if (signbit(deltaX)) {
                for (int k = 0; k < abs(deltaX); k++) {
                    sequence.push_back('<');
                }
            }

            sequences.push_back(sequence);

            // Reverse path if there is no hance to panic
            if (!panics) {
                string copy(sequence);
                reverse(copy.begin(), copy.end());
                sequences.push_back(copy);
            }

            optimalSequences[{numericKeys[i].key, numericKeys[j].key}] = sequences;
        }
    }

    for (int i = 0; i < directionalKeys.size(); i++) {
        for (int j = 0; j < directionalKeys.size(); j++) {
            if (i == j) continue;

            auto start = directionalKeys[i].coordinate;
            auto end = directionalKeys[j].coordinate;
            int deltaX = end.x - start.x;
            int deltaY = end.y - start.y;
            bool panics = min(start.x, end.x) == 0 && min(start.y, end.y) == 0;

            vector<string> sequences;
            string sequence;

            // Right then up
            if (!signbit(deltaX)) {
                for (int k = 0; k < deltaX; k++) {
                    sequence.push_back('>');
                }
            }

            if (signbit(deltaY)) {
                for (int k = 0; k < abs(deltaY); k++) {
                    sequence.push_back('^');
                }
            }

            // Down then left

            if (!signbit(deltaY)) {
                for (int k = 0; k < deltaY; k++) {
                    sequence.push_back('v');
                }
            }

            if (signbit(deltaX)) {
                for (int k = 0; k < abs(deltaX); k++) {
                    sequence.push_back('<');
                }
            }

            sequences.push_back(sequence);

            // Reverse path if there is no hance to panic
            if (!panics) {
                string copy(sequence);
                reverse(copy.begin(), copy.end());
                sequences.push_back(copy);
            }

            optimalSequences[{directionalKeys[i].key, directionalKeys[j].key}] = sequences;
        }
    }
}

unordered_set<string> get_optimal_sequences(const string& sequence, const string& optimalSequence = "", const int& index = 0) {
    if (index == sequence.length() - 1)
        return {optimalSequence};

    auto nextSequences = get_optimal_sequences(sequence, optimalSequence, index + 1);

    unordered_set<string> allOptions;

    if (sequence[index] == sequence[index + 1]) {
        for (auto&& nextSequence : nextSequences) {
            allOptions.insert("A" + nextSequence);
        }

        return allOptions;
    }

    auto sequences = optimalSequences[{sequence[index], sequence[index + 1]}];

    for (auto&& nextSequence : nextSequences) {
        allOptions.insert(sequences[0] + "A" + nextSequence);

        if (sequences.size() == 2)
            allOptions.insert(sequences[1] + "A" + nextSequence);
    }

    return allOptions;
}

string trace_path(const string& code, const int& robots) {
    unordered_set<string> sequences = {code};

    for (int i = 0; i < robots + 1; i++) {
        unordered_set<string> nextSequences;

        for (auto&& sequence : sequences) {
            auto nextIteration = get_optimal_sequences("A" + sequence);
            nextSequences.insert(nextIteration.begin(), nextIteration.end());
        }

        sequences = nextSequences;
    }

    return *min_element(sequences.begin(), sequences.end());
}

int main(int argc, char* argv[]) {
    lib::timer timer;
    vector<string> codes;

    lib::read_file(argv[1], [&codes](const string line, const int _) {
        codes.push_back(line);
    });

    precompute_optimal_sequences();

    int result = 0;

    timer.start();

    for (auto&& code : codes) {
        result += trace_path(code, 2).length() * stoi(code.substr(0, code.length() - 1));
    }

    timer.stop();

    cout << "Result: " << result << '\n';

    // PART II

    result = 0;

    timer.start();

    for (auto&& code : codes) {
        result += trace_path(code, 25).length() * stoi(code.substr(0, code.length() - 1));
        cout << result;
    }

    timer.stop();

    cout << "Result: " << result << '\n';

    return 0;
}