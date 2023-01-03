#include "StringStreamTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    int ret = Qentem::Test::RunStringStreamTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
