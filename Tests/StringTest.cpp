#include "StringTest.hpp"

int main() {
    int ret = Qentem::Test::RunStringTests();
    Qentem::TestHelper::PrintMemoryStatus();

    return ret;
}
