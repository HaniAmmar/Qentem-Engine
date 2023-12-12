#include "DigitTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    const int ret = Qentem::Test::RunDigitTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
