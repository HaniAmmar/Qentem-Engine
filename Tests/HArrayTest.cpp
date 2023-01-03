#include "HArrayTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    int ret = Qentem::Test::RunHArrayTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
