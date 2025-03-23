#include "arbiter.hpp"

int main() {
    srand(time(nullptr));
    Arbiter arbiter(5);
    arbiter.startRace();
    return 0;
}
