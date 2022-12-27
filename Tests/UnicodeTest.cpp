#include "UnicodeTest.hpp"

int main() {
    int ret = Qentem::Test::RunUnicodeTests();
    Qentem::Test::TestHelper::PrintMemoryStatus();

    return ret;
}
