#include <math.h>

#include <algorithm>
#include <unordered_map>
#include <unordered_set>

#include "../lib/lib.h"

using namespace std;

unordered_map<char, lib::coordinate> keypad{
    {'7', {0, 0}},
    {'8', {1, 0}},
    {'9', {2, 0}},
    {'4', {0, 1}},
    {'5', {1, 1}},
    {'6', {2, 1}},
    {'1', {0, 2}},
    {'2', {1, 2}},
    {'3', {2, 2}},
    {'-', {0, 3}},
    {'0', {1, 3}},
    {'A', {2, 3}},
};

unordered_map<char, lib::coordinate> directional_keypad{
    {'-', {0, 0}},
    {'^', {1, 0}},
    {'A', {2, 0}},
    {'<', {0, 1}},
    {'v', {1, 1}},
    {'>', {2, 1}},
};

char get_dir(const int &delta, const bool &isVertical) {
    if (isVertical) return delta < 0 ? '^' : 'v';
    return delta < 0 ? '<' : '>';
}

unordered_set<string> permutate(string sequence) {
    sort(sequence.begin(), sequence.end());

    unordered_set<string> permutations;

    do {
        permutations.insert(sequence);
    } while (next_permutation(sequence.begin(), sequence.end()));

    return permutations;
}

bool panics(const char &start, const string &sequence, unordered_map<char, lib::coordinate> &keypad) {
    auto curr = keypad[start];
    for (auto dir : sequence) {
        switch (dir) {
            case '<':
                curr.x--;
                break;
            case '>':
                curr.x++;
                break;
            case '^':
                curr.y--;
                break;
            case 'v':
                curr.y++;
                break;
            default:
                return false;
        }

        if (curr == keypad['-']) return true;
    }

    return false;
}

vector<string> get_permutations_of_step(const char &start, const char &end, unordered_map<char, lib::coordinate> &keypad) {
    auto startPos = keypad[start];
    auto endPos = keypad[end];

    int dx = endPos.x - startPos.x;
    int dy = endPos.y - startPos.y;

    string result;
    string result2;

    result += string(abs(dy), get_dir(dy, true));
    result += string(abs(dx), get_dir(dx, false));

    result2 += string(abs(dx), get_dir(dx, false));
    result2 += string(abs(dy), get_dir(dy, true));

    vector<string> sequences;
    // for (auto permutation : permutate(result)) {
    if (!panics(start, result, keypad))
        sequences.push_back(result);

    if (result != result2 & !panics(start, result2, keypad))
        sequences.push_back(result2);
    // }

    return sequences;
}

void add_next_button_sequences(vector<string> &sequences, const vector<string> &permutations) {
    size_t length = sequences.size();

    for (auto permutation : permutations) {
        for (size_t i = 0; i < length; i++) {
            sequences.push_back(sequences[i] + permutation + 'A');
        }
    }

    sequences.erase(sequences.begin(), sequences.begin() + length);
}

int main(int argc, char const *argv[]) {
    lib::timer timer;
    vector<string> codes;

    lib::read_file(argv[1], [&codes](const string line, const int _) {
        codes.push_back(line);
    });

    int result = 0;

    // vector<string> res{""};
    // for (size_t j = 0; j < ('A' + codes[0]).length() - 1; j++) {
    //     res = add_next_button_sequences(res, get_permutations_of_step(('A' + codes[0])[j], ('A' + codes[0])[j + 1], keypad));
    // }

    // lib::print_collection(res);

    timer.start();

    size_t N = 3;

    for (auto code : codes) {
        vector<string> sequences{code};

        // maps for each level from sequence to previous sequence
        // vector<unordered_map<string, string>> prev_sequences_per_level;

        for (size_t i = 0; i < N; i++) {
            // lib::print_collection(sequences);
            // sequences = lib::map<string, string>(sequences, [](const string sequence, const int _) { return 'A' + sequence; });

            // unordered_map<string, string> prev_sequences;

            vector<string> next_sequences;

            for (auto seq1 : sequences) {
                vector<string> curr_sequences{""};
                string sequence_from_a = 'A' + seq1;

                for (size_t j = 0; j < sequence_from_a.length() - 1; j++) {
                    auto ways_to_press_next_button = get_permutations_of_step(sequence_from_a[j], sequence_from_a[j + 1], i == 0 ? keypad : directional_keypad);
                    add_next_button_sequences(curr_sequences, ways_to_press_next_button);
                }

                for (auto seq2 : curr_sequences) {
                    next_sequences.push_back(seq2);
                    // prev_sequences[seq2] = seq1;
                }
            }
            // prev_sequences_per_level.push_back(prev_sequences);

            sequences = next_sequences;
        }
        // lib::print_collection(sequences);

        // cout << sequence.length() << " " << sequence << '\n';

        size_t best_length = sequences[0].length();
        size_t best_idx = 0;
        for (int i = 0; i < sequences.size(); i++) {
            if (sequences[i].length() < best_length) {
                best_length = sequences[i].length();
                best_idx = i;
            }
        }

        result += sequences[best_idx].length() * stoi(code.substr(0, code.length() - 1));

        // // print the optimal sequences
        // string best_sequence = sequences[best_idx];
        // best_length = sequences[best_idx].length();
        // for (int i = 0; i < N; i++) {
        //     cout << best_length << " " << best_sequence << '\n';
        //     best_sequence = prev_sequences_per_level[N - 1 - i][best_sequence];
        //     // printf("Got here \n");
        //     best_length = best_sequence.length();
        // }
        // cout << best_length << " " << best_sequence << "\n\n";
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

// v<<A>>^A<A>AvA<^AA>A<vAAA>^A
// <A^A>^^AvvvA

// <v<A>^>AvA^A<v<A>A<A>^>AAvA<^A>AAvA^A<vA>^AA<A>A<v<A>A>^AAAvA<^A>A
// <v<A>>^AvA^A<vA<AA>>^AAvA<^A>AAvA^A<vA>^AA<A>A<v<A>A>^AAAvA<^A>A