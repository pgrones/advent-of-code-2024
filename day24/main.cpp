#include "../lib/lib.h"

using namespace std;

struct node {
    string name;
    bool is_empty;
    bool value;
    string left_name;
    const node* left;
    string right_name;
    const node* right;

    node() {}

    node(string name, string left_name, string right_name) {
        this->name = name;
        this->left_name = left_name;
        this->right_name = right_name;
        is_empty = true;
        left = right = nullptr;
    }

    node(string name, bool value) {
        this->name = name;
        this->value = value;
        is_empty = false;
        left = right = nullptr;
    }

    virtual void calculate_value() {
        is_empty = false;
    }
};

struct and_wire : node {
    using node::node;
    void calculate_value() override {
        value = left->value & right->value;
        node::calculate_value();
    }
};

struct or_wire : node {
    using node::node;
    void calculate_value() override {
        value = left->value | right->value;
        node::calculate_value();
    }
};

struct xor_wire : node {
    using node::node;
    void calculate_value() override {
        value = left->value ^ right->value;
        node::calculate_value();
    }
};

void connect_wires(vector<node*> wires) {
    for (size_t i = 0; i < wires.size(); i++) {
        if (!wires[i]->is_empty) continue;

        string left_name = wires[i]->left_name, right_name = wires[i]->right_name;

        wires[i]->left = *find_if(wires.begin(), wires.end(), [&left_name](const node* n) { return n->name == left_name; });
        wires[i]->right = *find_if(wires.begin(), wires.end(), [&right_name](const node* n) { return n->name == right_name; });
    }
}

long long to_int(const vector<bool>& bits) {
    long long result = 0;

    for (int i = bits.size() - 1; i >= 0; i--) {
        result = (result << 1) | bits[i];
    }

    return result;
}

int main(int argc, char const* argv[]) {
    lib::timer timer;
    vector<node*> wires;
    int number_of_z = 0;

    bool is_inital_wires = true;
    lib::read_file(argv[1], [&wires, &is_inital_wires, &number_of_z](const string line, const int _) {
        if (line == "") {
            is_inital_wires = false;
            return;
        }

        node* wire = nullptr;
        if (is_inital_wires) {
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

    timer.start();

    connect_wires(wires);

    bool not_done = true;
    while (not_done) {
        not_done = false;

        for (auto wire : wires) {
            if (!wire->is_empty) continue;

            if (wire->left->is_empty || wire->right->is_empty) {
                not_done = true;
                continue;
            }

            wire->calculate_value();

            if (wire->name.at(0) == 'z') {
                bits[stoi(wire->name.substr(1))] = wire->value;
            }
        }
    }

    result = to_int(bits);

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