#include <math.h>

#include <algorithm>
#include <numeric>

#include "../lib/lib.h"

using namespace std;

class computer {
    long long a;
    long long b;
    long long c;
    int pointer;
    vector<string> out_buffer;

    long long get_combo_operand(int operand) {
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

    void run_instruction(int instruction, long long operand) {
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

    void adv(long long operand) {
        a /= pow(2, get_combo_operand(operand));
        increment_pointer();
    }

    void bxl(long long operand) {
        b ^= operand;
        increment_pointer();
    }

    void bst(long long operand) {
        b = get_combo_operand(operand) % 8;
        increment_pointer();
    }

    void jnz(long long operand) {
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

    void out(long long operand) {
        out_buffer.push_back(to_string(get_combo_operand(operand) % 8));
        increment_pointer();
    }

    void bdv(long long operand) {
        b = a / pow(2, get_combo_operand(operand));
        increment_pointer();
    }

    void cdv(long long operand) {
        c = a / pow(2, get_combo_operand(operand));
        increment_pointer();
    }

   public:
    computer(long long a, long long b, long long c) {
        this->a = a;
        this->b = b;
        this->c = c;
        pointer = 0;
    }

    string run(const vector<int> &instructions) {
        while (pointer < instructions.size()) {
            run_instruction(instructions[pointer], instructions[pointer + 1]);
        }

        return accumulate(next(out_buffer.begin()), out_buffer.end(), out_buffer[0], [](string a, string b) {
            return a + ',' + b;
        });
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

    vector<long long> as = {(long long)pow(8, 15)};
    for (int i = 0; i < 16; i++) {
        long long digit = pow(8, 15 - i);
        vector<long long> next_as;

        for (int j = 0; j < 8; j++) {
            for (auto ai : as) {
                long long a = ai + j * digit;

                result = computer(a, registers[1], registers[2]).run(instructions);
                auto result_vec = lib::map<string, int>(lib::split(result, ","), [](const string item, const int _) { return stoi(item); });

                if (result_vec[15 - i] == instructions[15 - i]) {
                    next_as.push_back(a);
                }
            }
        }

        as = next_as;
    }

    sort(as.begin(), as.end());

    timer.stop();

    cout << "Result: " << as[0] << '\n';

    return 0;
}
