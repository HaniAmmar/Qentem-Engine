#include "StringTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    int ret = Qentem::Test::RunStringTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
