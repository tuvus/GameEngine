#include <chrono>
#include <thread>

#include "library.h"
using namespace std;

int main() {
    for (int i = 0; i < 100; ++i) {
        chrono::time_point<chrono::system_clock> frameStartTime = chrono::system_clock::now();
        hello(i);
        this_thread::sleep_until(frameStartTime + 16ms);
    }
    return 0;
}
