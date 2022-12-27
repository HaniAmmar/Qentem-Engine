#include "StringTest.hpp"

int main() {
    int ret = Qentem::Test::RunStringTests();
    Qentem::Test::TestHelper::PrintMemoryStatus();

    return ret;
}
