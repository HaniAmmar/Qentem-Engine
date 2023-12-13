#include "JSONUtilsTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunJSONUtilsTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
