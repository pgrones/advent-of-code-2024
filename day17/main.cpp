#include <math.h>

#include "../lib/lib.h"

using namespace std;

class computer {
    int a;
    int b;
    int c;
    int pointer;
    vector<int> out_buffer;

    int get_combo_operand(int operand) {
        if (operand <= 3) return operand;

        switch (operand) {
            case 4:
                return a;
            case 5:
                return b;
            case 6:
                return c;
            default:
                throw runtime_error("invalid operand");
        }
    }

    void run_instruction(int instruction, int operand) {
        switch (instruction) {
            case 0:
                adv(operand);
                break;
            case 1:
                bxl(operand);
                break;
            case 2:
                bst(operand);
                break;
            case 3:
                jnz(operand);
                break;
            case 4:
                bxc();
                break;
            case 5:
                out(operand);
                break;
            case 6:
                bdv(operand);
                break;
            case 7:
                cdv(operand);
                break;
            default:
                runtime_error("invalid instruction");
        }
    }

    void increment_pointer() {
        pointer += 2;
    }

    void adv(int operand) {
        a /= pow(2, get_combo_operand(operand));
        increment_pointer();
    }

    void bxl(int operand) {
        b ^= operand;
        increment_pointer();
    }

    void bst(int operand) {
        b = get_combo_operand(operand) % 8;
        increment_pointer();
    }

    void jnz(int operand) {
        if (a == 0) {
            increment_pointer();
            return;
        }

        pointer = operand;
    }

    void bxc() {
        b ^= c;
        increment_pointer();
    }

    void out(int operand) {
        out_buffer.push_back(get_combo_operand(operand) % 8);
        increment_pointer();
    }

    void bdv(int operand) {
        b = a / pow(2, get_combo_operand(operand));
        increment_pointer();
    }

    void cdv(int operand) {
        c = a / pow(2, get_combo_operand(operand));
        increment_pointer();
    }

   public:
    computer(int a, int b, int c) {
        this->a = a;
        this->b = b;
        this->c = c;
        pointer = 0;
    }

    string run(const vector<int> &instructions) {
        while (pointer < instructions.size()) {
            run_instruction(instructions[pointer], instructions[pointer + 1]);
        }

        string result;
        for (auto outValue : out_buffer)
            result += to_string(outValue) + ",";

        return result.substr(0, result.size() - 1);
    }
};

int main(int argc, char const *argv[]) {
    lib::timer timer;
    int registers[3];
    vector<int> instructions;
    string program;

    bool isInstructions = false;
    lib::read_file(argv[1], [&registers, &instructions, &program, &isInstructions](const string line, const int i) {
        if (line == "") {
            isInstructions = true;
            return;
        }

        if (!isInstructions) {
            registers[i] = stoi(lib::split(line, ": ")[1]);
            return;
        }

        program = lib::split(line, ": ")[1];

        instructions = lib::map<string, int>(lib::split(program, ","), [](const string item, const int _) { return stoi(item); });
    });

    string result;

    timer.start();

    result = computer(registers[0], registers[1], registers[2]).run(instructions);

    timer.stop();

    cout << "Result: " << result << '\n';

    // PART II

    result = "";

    timer.start();

    int registerA = -1;
    int onePercentOfIntMax = INT_MAX * 0.01;
    while (result != program || registerA == registers[0]) {
        registerA++;
        result = computer(registerA, registers[1], registers[2]).run(instructions);

        if (registerA % onePercentOfIntMax == 0)
            cout << to_string(registerA / onePercentOfIntMax) + "\% of INT_MAX searched" << '\n';
    }

    timer.stop();

    cout << "Result: " << registerA << '\n';

    return 0;
}