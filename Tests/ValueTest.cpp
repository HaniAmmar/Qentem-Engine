#include "ValueTest.hpp"

int main() {
    int ret = Qentem::Test::RunValueTests();
    Qentem::Test::TestHelper::PrintMemoryStatus();

    return ret;
}
