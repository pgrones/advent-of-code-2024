#include <functional>
#include <map>
#include <memory>
#include <stack>
#include <unordered_map>

#include "../lib/lib.h"

namespace std {
template <>
struct hash<lib::coordinate> {
    std::size_t operator()(const lib::coordinate& coord) const noexcept {
        std::size_t h1 = std::hash<int>{}(coord.x);
        std::size_t h2 = std::hash<int>{}(coord.y);
        return h1 ^ (h2 << 1);
    }
};
}  // namespace std

using namespace std;

unordered_map<char, lib::coordinate> coordinateByKey{
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

unordered_map<lib::coordinate, char> keyByCoordinate{
    {{0, 0}, '7'},
    {{1, 0}, '8'},
    {{2, 0}, '9'},
    {{0, 1}, '4'},
    {{1, 1}, '5'},
    {{2, 1}, '6'},
    {{0, 2}, '1'},
    {{1, 2}, '2'},
    {{2, 2}, '3'},
    {{1, 3}, '0'},
    {{2, 3}, 'A'},
};

unordered_map<char, lib::coordinate> coordinateByDirection{
    {'^', {1, 0}},
    {'A', {2, 0}},
    {'<', {0, 1}},
    {'v', {1, 1}},
    {'>', {2, 1}},
};

unordered_map<lib::coordinate, char> directionByCoordinate{
    {{1, 0}, '^'},
    {{2, 0}, 'A'},
    {{0, 1}, '<'},
    {{1, 1}, 'v'},
    {{2, 1}, '>'},
};

char directions[5]{'A', '^', '<', 'v', '>'};

struct node {
   protected:
    node* next = nullptr;

    void append(node* node) {
        if (next != nullptr) {
            next->append(node);
            return;
        }

        next = node;
    }

   public:
    virtual bool try_move(const char&) { return true; };

    virtual optional<char> press() { return nullopt; }

    virtual void undo() {}
};

struct keypad : node {
   private:
    lib::coordinate position;
    unordered_map<lib::coordinate, char>* keys;
    stack<lib::coordinate> history;

   protected:
    keypad(lib::coordinate initialPosition, unordered_map<lib::coordinate, char>* keys) {
        position = initialPosition;
        this->keys = keys;
    }

    bool try_move(const char& key) override {
        history.push(position);

        switch (key) {
            case '<':
                position.x--;
                break;
            case '>':
                position.x++;
                break;
            case '^':
                position.y--;
                break;
            case 'v':
                position.y++;
                break;
        }

        if (keys->find(position) == keys->end())
            return false;

        return true;
    }

    optional<char> press() override {
        auto key = keys->at(position);

        if (next == nullptr) {
            return optional(key);
        }

        if (key == 'A') {
            return next->press();
        }

        next->try_move(key);
        return nullopt;
    }

    void undo() {
        if (!history.empty()) {
            position = history.top();
            history.pop();
        }

        if (next == nullptr) return;

        next->undo();
    }
};

struct numeric_keypad : keypad {
    numeric_keypad() : keypad(coordinateByKey['A'], &keyByCoordinate) {}
};

struct directional_keypad : keypad {
    directional_keypad() : keypad(coordinateByDirection['A'], &directionByCoordinate) {}
};

struct your_keypad : node {
   private:
    vector<char> keyPresses;
    vector<unique_ptr<node>> keypads;
    map<size_t, char> code;

   public:
    your_keypad(const size_t& depth) {
        for (size_t i = 0; i < depth; i++) {
            keypads.push_back(make_unique<directional_keypad>());
            append(keypads.back().get());
        }

        keypads.push_back(make_unique<numeric_keypad>());
        append(keypads.back().get());
    }

    bool try_press(const char& key) {
        keyPresses.push_back(key);

        if (key == 'A') {
            auto result = next->press();

            if (result.has_value())
                code[keyPresses.size()] = result.value();

            return true;
        }

        return next->try_move(key);
    }

    void undo() {
        code.erase(keyPresses.size());
        keyPresses.pop_back();
        next->undo();
    }

    size_t length() {
        return keyPresses.size();
    }

    string get_code() {
        string result;
        for (const auto& [_, value] : code) {
            result += value;
        }
        return result;
    }

    string to_string() {
        string result(keyPresses.begin(), keyPresses.end());
        return result;
    }
};

void simulate_key_presses(const string& code, your_keypad& you, size_t& shortestSequence) {
    auto currentCode = you.get_code();

    if (currentCode == code) {
        shortestSequence = min(you.length(), shortestSequence);
        return;
    }

    if (you.length() >= shortestSequence) {
        return;
    }

    if (code.rfind(currentCode, 0) == string::npos) {
        // equivalent to !code.starts_with(currentCode)
        return;
    }

    auto sequence = you.to_string();

    if (sequence.length() > 3) {
        cout << currentCode;
        auto last = sequence.substr(sequence.length() - 4);
        if (last[0] != 'A' && last[1] != 'A' && last[2] != 'A')
            return;
    }

    for (const auto& direction : directions) {
        if (you.try_press(direction)) {
            simulate_key_presses(code, you, shortestSequence);
        }

        you.undo();
    }
}

int main(int argc, char* argv[]) {
    lib::timer timer;
    vector<string> codes;

    lib::read_file(argv[1], [&codes](const string line, const int _) {
        codes.push_back(line);
    });

    int result = 0;

    timer.start();

    for (auto&& code : codes) {
        size_t shortestSequence = 70;
        auto you = your_keypad(2);
        simulate_key_presses(code, you, shortestSequence);
        cout << you.to_string() << '\n';
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