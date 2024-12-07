#include "../lib/lib.h"

using namespace std;

struct equation {
    long long value;
    long long startValue;
    vector<long long> operands;
};

long long test_equation(long long &testValue, vector<long long> &operands, long long currValue, bool withConcat = false) {
    if (testValue == currValue && operands.empty()) return testValue;

    if (testValue < currValue || operands.empty()) return 0;

    vector<long long> remainingOperands = {operands.begin() + 1, operands.end()};

    long long result = test_equation(testValue, remainingOperands, currValue + operands[0], withConcat);

    if (result) return result;

    result = test_equation(testValue, remainingOperands, currValue * operands[0], withConcat);

    if (result) return result;

    if (!withConcat) return 0;

    return test_equation(testValue, remainingOperands, stoll(to_string(currValue) + to_string(operands[0])), withConcat);
}

int main(int argc, char const *argv[]) {
    lib::timer timer;
    vector<equation> equations;

    lib::read_file(argv[1], [&equations](const string line, const int _) {
        auto equationParts = lib::split(line, ": ");
        long long value = stoll(equationParts[0]);
        vector<long long> operands = lib::map<string, long long>(lib::split(equationParts[1], " "), [](string number, int _) { return stoll(number); });
        equations.push_back({value, operands[0], {operands.begin() + 1, operands.end()}});
    });

    long long result = 0;  // Initializes with 2 by default?

    timer.start();

    for (auto equation : equations) {
        result += test_equation(equation.value, equation.operands, equation.startValue);
    }

    timer.stop();

    cout << "Result: " << result << '\n';

    result = 0;

    timer.start();

    for (auto equation : equations) {
        result += test_equation(equation.value, equation.operands, equation.startValue, true);
    }

    timer.stop();

    cout << "Result: " << result << '\n';

    return 0;
}