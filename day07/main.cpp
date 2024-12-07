#include "../lib/lib.h"

using namespace std;

struct equation {
    long long value;
    vector<long long> operands;
};

long long test_equation(long long &testValue, vector<long long> &operands, long long currValue = 0) {
    if (testValue == currValue && operands.empty()) return testValue;

    if (testValue < currValue || operands.empty()) return 0;

    vector<long long> remainingOperands = {operands.begin() + 1, operands.end()};

    long long result = test_equation(testValue, remainingOperands, currValue + operands[0]);

    if (result) return result;

    if (currValue == 0) currValue = 1;

    return test_equation(testValue, remainingOperands, currValue * operands[0]);
}

int main(int argc, char const *argv[]) {
    lib::timer timer;
    vector<equation> equations;

    lib::read_file(argv[1], [&equations](const string line, const int _) {
        auto equationParts = lib::split(line, ": ");
        long long value = stoll(equationParts[0]);
        vector<long long> operands = lib::map<string, long long>(lib::split(equationParts[1], " "), [](string number, int _) { return stoll(number); });
        equations.push_back({value, operands});
    });

    long long result = 0;

    timer.start();

    for (auto equation : equations) {
        result += test_equation(equation.value, equation.operands);
    }

    timer.stop();

    cout << "Result: " << result << '\n';

    return 0;
}