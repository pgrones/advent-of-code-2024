#include "../lib/lib.h"
#include "cassert"

using namespace std;

struct node {
    string name;
    bool is_empty;
    bool value;
    string left_name;
    const node* left;
    string right_name;
    const node* right;
    char op;

    node() {}

    node(string name, string left_name, string right_name, char op) {
        this->name = name;
        this->left_name = left_name;
        this->right_name = right_name;
        is_empty = true;
        left = right = nullptr;
        this->op = op;
    }

    node(string name, bool value) {
        this->name = name;
        this->value = value;
        is_empty = false;
        left = right = nullptr;
    }

    void reset() {
        if (!left) return;
        is_empty = true;
    }

    virtual void calculate_value() {
        is_empty = false;
    }
};

struct and_wire : node {
    using node::node;

    and_wire(string name, string left_name, string right_name) : node(name, left_name, right_name, 'A') {
    }

    void calculate_value() override {
        value = left->value & right->value;
        node::calculate_value();
    }
};

struct or_wire : node {
    using node::node;

    or_wire(string name, string left_name, string right_name) : node(name, left_name, right_name, 'O') {
    }

    void calculate_value() override {
        value = left->value | right->value;
        node::calculate_value();
    }
};

struct xor_wire : node {
    using node::node;

    xor_wire(string name, string left_name, string right_name) : node(name, left_name, right_name, 'X') {}

    void calculate_value() override {
        value = left->value ^ right->value;
        node::calculate_value();
    }
};

vector<node*> wires;
int number_of_z = 0;

void connect_wires(vector<node*> wires) {
    for (size_t i = 0; i < wires.size(); i++) {
        if (!wires[i]->is_empty) continue;

        string left_name = wires[i]->left_name, right_name = wires[i]->right_name;

        wires[i]->left = *find_if(wires.begin(), wires.end(), [&left_name](const node* n) { return n->name == left_name; });
        wires[i]->right = *find_if(wires.begin(), wires.end(), [&right_name](const node* n) { return n->name == right_name; });
    }
}

void reset_wires(vector<node*> wires) {
    for (auto&& wire : wires) {
        wire->reset();
    }
}

long long to_int(const vector<bool>& bits) {
    long long result = 0;

    for (int i = bits.size() - 1; i >= 0; i--) {
        result = (result << 1) | bits[i];
    }

    return result;
}

bool has_children(node* parent, node* child1, node* child2) {
    return (parent->left == child1 && parent->right == child2) || (parent->left == child2 && parent->right == child1);
}

bool check_adder(int bit = 0, node* carry = nullptr) {  // unordered_map<string, string>

    if (bit == number_of_z - 1) return true;

    char bit_chars[2];
    sprintf(bit_chars, "%02d", bit);
    string bit_str(bit_chars);
    node *x, *y;
    for (auto w : wires) {
        if (w->name == "x" + bit_str) x = w;
        if (w->name == "y" + bit_str) y = w;
    }

    if (bit == 0) {
        // bit == 0
        assert(carry == nullptr);

        node* and1 = nullptr;

        for (auto w : wires) {
            // cout << w->name << " " << w->left << " " << w->right << " " << bit << "\n";
            if (has_children(w, x, y)) {
                if (w->op == 'A') {
                    and1 = w;
                } else if (w->op == 'X') {
                    if (w->name != "z" + bit_str) return false;
                } else {
                    cout << w->name << " " << w->left << " " << w->right << " " << bit << "\n";
                    return false;
                }
            }
        }
        if (and1 == nullptr) {
            cout << x->name << " " << y->name << " not found" << " " << bit << "\n";
            return false;
        }
        return check_adder(1, and1);
    }

    // cout << bit_str;
    // cout << x->name << " " << y->name << " " << bit << "\n";

    node* xor1 = nullptr;
    node* and1 = nullptr;

    for (auto w : wires) {
        // cout << w->name << " " << w->left << " " << w->right << " " << bit << "\n";
        if (has_children(w, x, y)) {
            if (w->op == 'A') {
                and1 = w;
            } else if (w->op == 'X') {
                xor1 = w;
            } else {
                cout << w->name << " " << w->left << " " << w->right << " " << bit << "\n";
                return false;
            }
        }
    }
    if (xor1 == nullptr || and1 == nullptr) {
        cout << x->name << " " << y->name << " not found 0" << " " << bit << "\n";
        return false;
    }

    node* xor2 = nullptr;
    node* and2 = nullptr;

    for (auto w : wires) {
        if (xor2 != nullptr && and2 != nullptr) break;

        if (has_children(w, xor1, carry)) {
            if (w->op == 'A') {
                and2 = w;
            } else if (w->op == 'X') {
                if (w->name != "z" + bit_str) {
                    // cout << w->name << xor1->name << carry->name << " " << " " << bit << "\n";
                    // return false;

                    const node *left_temp, *right_temp;

                    node* z = *find_if(wires.begin(), wires.end(), [&bit_str](const node* n) { return n->name == "z" + bit_str; });

                    // z->name = w->name;
                    // w->name = "z" + bit_str;

                    left_temp = z->left;
                    right_temp = z->right;
                    char op_temp = z->op;

                    z->left = w->left;
                    z->right = w->right;
                    z->op = 'X';

                    w->left = left_temp;
                    w->right = right_temp;
                    w->op = op_temp;
                    xor2 = z;
                    continue;
                }
                xor2 = w;
            } else {
                cout << w->name << " " << w->left << " " << w->right << " " << bit << "\n";
                return false;
            }
        }
    }
    if (xor2 == nullptr || and2 == nullptr) {
        cout << xor1->name << " " << carry->name << " not found 1" << " " << bit << "\n";
        return false;
    }

    node* or1 = nullptr;

    for (auto w : wires) {
        if (has_children(w, and2, and1)) {
            if (w->op == 'O') {
                or1 = w;
            } else {
                cout << w->name << " " << w->left << " " << w->right << " " << bit << "\n";
                return false;
            }
        }
    }
    if (or1 == nullptr) {
        cout << and2->name << " " << and1->name << " not found 2" << " " << bit << "\n";
        return false;
    }

    return check_adder(bit + 1, or1);
}

int main(int argc, char const* argv[]) {
    lib::timer timer;

    bool is_initial_wires = true;
    lib::read_file(argv[1], [&is_initial_wires](const string line, const int _) {
        if (line == "") {
            is_initial_wires = false;
            return;
        }

        node* wire = nullptr;
        if (is_initial_wires) {
            auto parts = lib::split(line, ": ");
            wire = new node{parts[0], parts[1] == "1"};
            wires.push_back(wire);
            return;
        }

        auto parts = lib::split(line, " ");
        string left_name = parts[0], right_name = parts[2], root_name = parts[4];

        switch (parts[1].at(0)) {
            case 'A':
                wire = new and_wire(root_name, left_name, right_name);
                break;
            case 'O':
                wire = new or_wire(root_name, left_name, right_name);
                break;
            case 'X':
                wire = new xor_wire(root_name, left_name, right_name);
                break;
            default:
                throw runtime_error("Unknown operation");
        }

        wires.push_back(wire);

        if (root_name.at(0) == 'z') number_of_z++;
    });

    long long result = 0;
    vector<bool> bits;
    bits.resize(number_of_z);

    // timer.start();

    connect_wires(wires);

    // bool not_done = true;
    // while (not_done) {
    //     not_done = false;

    //     for (auto wire : wires) {
    //         if (!wire->is_empty) continue;

    //         if (wire->left->is_empty || wire->right->is_empty) {
    //             not_done = true;
    //             continue;
    //         }

    //         wire->calculate_value();

    //         if (wire->name.at(0) == 'z') {
    //             bits[stoi(wire->name.substr(1))] = wire->value;
    //         }
    //     }
    // }

    // result = to_int(bits);

    // timer.stop();

    cout << "Result: " << result << '\n';

    // PART II

    result = 0;

    // reset_wires(wires);

    timer.start();

    printf("CHECK: %i\n", check_adder());

    timer.stop();

    cout << "Result: " << result << '\n';

    return 0;
}