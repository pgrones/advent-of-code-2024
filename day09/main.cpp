#include "../lib/lib.h"

using namespace std;

int main(int argc, char const *argv[]) {
    lib::timer timer;
    vector<int> files;

    lib::read_file(argv[1], [&files](const string line, const int _) {
        bool isSpace = false;
        int i = 0;

        for (auto file : line) {
            int id = isSpace ? -1 : i;

            for (size_t j = 0; j < file - '0'; j++) {
                files.push_back(id);
            }

            isSpace = !isSpace;

            if (!isSpace) i++;
        }
    });

    long long result = 0;
    int i = 0;
    vector<int> filesCopy = files;

    timer.start();

    while (true) {
        while (filesCopy[i] != -1) i++;

        if (filesCopy.size() <= i) break;

        filesCopy[i] = filesCopy.back();
        filesCopy.pop_back();
    };

    for (size_t j = 0; j < filesCopy.size(); j++) {
        result += j * filesCopy[j];
    }

    timer.stop();

    cout << "Result: " << result << '\n';

    // PART II

    result = 0;

    timer.start();

    // Go through all ids in the vector
    for (int id = files.back(); id >= 0; id--) {
        size_t start = -1;
        int length = 0;

        // Search for the id and save its length and starting position
        for (size_t i = 0; i < files.size(); i++) {
            if (files[i] != id && length != 0) break;

            if (files[i] != id) continue;

            if (start == -1) start = i;

            length++;
        }

        // Find a suitable free space somewhere before the id
        size_t freeSpaceStart = 0;
        for (size_t i = 0; i < start; i++) {
            if (files[i] != -1) continue;

            int freeSpaceLength = 0;
            for (size_t j = i; j < files.size(); j++) {
                if (files[j] != -1) break;

                freeSpaceLength++;
            }

            if (freeSpaceLength >= length) {
                freeSpaceStart = i;
                break;
            }
        }

        // There is not enough free space
        if (freeSpaceStart == 0) continue;

        // Put the id where the free space is and remove it from its original position
        for (size_t i = freeSpaceStart; i < freeSpaceStart + length; i++) {
            files[i] = id;
        }

        for (size_t i = start; i < start + length; i++) {
            files[i] = -1;
        }
    }

    for (size_t j = 0; j < files.size(); j++) {
        if (files[j] == -1) continue;

        result += j * files[j];
    }

    timer.stop();

    cout << "Result: " << result << '\n';

    return 0;
}
