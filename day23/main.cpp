#include "../lib/lib.h"
#include "unordered_set"
#include "unordered_map"
using namespace std;

struct computer {
    string id;
    unordered_set<computer*> connected_to;

    computer() : id("") {}
    computer(const string &id) : id(id) {}
    computer(string id, unordered_set<computer*> connected_to)
        : id(id), connected_to(connected_to) {}

    // function to add a connection
    void add_connection(computer *c) {
        connected_to.insert(c);
    }
};

unordered_map<string, computer> computers;

int main(int argc, char const *argv[]) {
    lib::timer timer;

    lib::read_file(argv[1], [](const string line, const int _) {
        vector<string> connection = lib::split(line, "-");
        for (string c : connection) {
            if (computers.find(c) == computers.end()) {
                computers[c] = computer(c);
            }
        }
        for (string c : connection) {
            for (string cc : connection) {
                if (c == cc) continue;
                computers[c].add_connection(&computers[cc]);
            }
        }
    });

    for (auto c : computers) {
        cout << c.first << " is connected to: ";
        for (auto cc : c.second.connected_to) {
            cout << cc->id << ", ";
        }
        cout << '\n';
    }

    int result = 0;

    timer.start();

    // find all cycles of length 3 in the graph
    for (auto c : computers) {
        for (auto cc : c.second.connected_to) {
            for (auto ccc : cc->connected_to) {
                if (ccc->connected_to.find(&c.second) != ccc->connected_to.end()) {
                    result++;
                }
            }
        }
    }

    timer.stop();

    cout << "Result: " << result << '\n';

    // PART II

    result = 0;

    timer.start();

    // do something

    timer.stop();

    cout << "Result: " << result << '\n';

    return 0;
}