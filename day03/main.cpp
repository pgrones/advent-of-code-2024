#include <regex>

#include "../lib/lib.h"

using namespace std;

int calculateResult(string const memory) {
    regex const pattern{R"~(mul\(\d{1,3},\d{1,3}\))~"};

    auto mul_begin = sregex_iterator(memory.begin(), memory.end(), pattern);
    auto mul_end = sregex_iterator();

    int result = 0;

    for (sregex_iterator i = mul_begin; i != mul_end; ++i) {
        smatch match = *i;
        string match_str = match.str();
        string numbers = match_str.substr(4, match_str.length() - 5);

        auto parts = lib::map<string, int>(lib::split(numbers, ","), [](string element, int i) { return stoi(element); });

        result += parts[0] * parts[1];
    }

    return result;
}

int main(int argc, char *argv[]) {
    lib::timer timer;
    string memory = "";

    lib::read_file(argv[1], [&memory](const string &line, const int i) -> void { memory += line; });

    timer.start();

    int result = calculateResult(memory);

    timer.stop();

    cout << "Result: " << result << '\n';

    // PART II

    regex const disabledPattern{R"~(don't\(\).*?(do\(\)|$))~"};
    result = 0;

    timer.start();

    memory += "do()";
    memory = regex_replace(memory, disabledPattern, "");

    result = calculateResult(memory);

    timer.stop();

    cout << "Result: " << result << '\n';

    return 0;
}
