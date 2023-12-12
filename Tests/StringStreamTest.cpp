#include "StringStreamTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    const int ret = Qentem::Test::RunStringStreamTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
