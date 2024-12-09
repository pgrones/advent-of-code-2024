#include "../lib/lib.h"

using namespace std;

int main(int argc, char const *argv[]) {
    lib::timer timer;
    vector<int> files;

    lib::read_file(argv[1], [&files](const string line, const int _) {
        // read file (empty spaces = -1) (worst case 180k items)
    });

    int result = 0;

    timer.start();

    // move last item to first empty space until vector doesn't have empty spaces anymore

    // calculate result -> loop; result += i * files[i];

    timer.stop();

    cout << "Result: " << result << '\n';

    return 0;
}
