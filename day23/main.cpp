#include <algorithm>
#include <cassert>
#include <numeric>

#include "../lib/lib.h"
#include "unordered_map"
#include "unordered_set"
using namespace std;

struct computer {
    string id;
    unordered_set<computer *> connected_to;

    computer() {}
    computer(const string &id) : id(id) {}

    void add_connection(computer *c) {
        connected_to.insert(c);
    }
};

struct ordered_list {
    vector<string> ids;

    ordered_list(string id) {
        ids.push_back(id);
    }

    void add(const string &id) {
        ids.push_back(id);
        sort(ids.begin(), ids.end());
    }

    string to_string() const {
        return accumulate(next(ids.begin()), ids.end(), ids[0], [](string a, string b) {
            return a + ',' + b;
        });
    }
};

vector<string> find_cycles(const computer *root, computer *curr, ordered_list path, int max_depth, int depth = 0) {
    if (depth == max_depth - 1 && curr == root) return {path.to_string()};
    if (depth == max_depth) return {};

    for (auto id : path.ids) {
        if (find_if(curr->connected_to.begin(), curr->connected_to.end(), [&id](const computer *conn) { return conn->id == id; }) == curr->connected_to.end()) return {};
        if (curr->id < id) return {};
    }

    path.add(curr->id);

    vector<string> result;

    for (auto connection : curr->connected_to) {
        for (auto hash : find_cycles(root, connection, path, max_depth, depth + 1)) {
            result.push_back(hash);
        }
    }

    return result;
}

unordered_set<string> find_cycles(const computer *root, int max_depth = 3) {
    unordered_set<string> result;

    for (auto connection : root->connected_to) {
        ordered_list path(root->id);
        for (auto hash : find_cycles(root, connection, path, max_depth)) {
            result.insert(hash);
        }
    }

    return result;
}

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

    int result = 0;

    timer.start();

    // find all cycles of length 3 in the graph
    for (auto c : computers) {
        string c_id = c.first;
        if (c_id.at(0) != 't') continue;

        for (auto cc : c.second.connected_to) {
            if (cc->id.at(0) == 't' && cc->id > c.first) continue;
            for (auto ccc : cc->connected_to) {
                if (ccc->id.at(0) == 't' && ccc->id > c.first) continue;
                if (ccc->id > cc->id) continue;

                if (ccc->connected_to.find(&computers[c_id]) != ccc->connected_to.end()) {
                    result++;
                }
            }
        }
    }

    timer.stop();

    cout << "Result: " << result << '\n';

    // PART II

    result = 0;
    unordered_set<string> cycles;

    timer.start();

    // binary search over depths
    // find largest depth for which find_cycles() isn't empty (and should have exactly one element)
    // return the sorted version of that result

    size_t l = 3, r = 20;
    string max_cycle;
    while (l < r) {
        cycles.clear();
        int mid = l + (r - l) / 2;

        for (auto c : computers) {
            for (auto cycle : find_cycles(&computers[c.first], mid)) {
                cycles.insert(cycle);
            }
        }

        if (cycles.size() > 0) {
            max_cycle = *cycles.begin();
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }

    timer.stop();

    cout << "Result: " << max_cycle << '\n';

    return 0;
}