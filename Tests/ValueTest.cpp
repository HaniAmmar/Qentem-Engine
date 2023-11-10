#include "ValueTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    const int ret = Qentem::Test::RunValueTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
