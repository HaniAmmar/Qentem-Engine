#include "DigitTest.hpp"

int main() {
    int ret = Qentem::Test::RunDigitTests();
    Qentem::Test::TestHelper::PrintMemoryStatus();

    return ret;
}
