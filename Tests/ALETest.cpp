#include "ALETest.hpp"

int main() {
    int ret = Qentem::Test::RunALETests();
    Qentem::TestHelper::PrintMemoryStatus();

    return ret;
}
