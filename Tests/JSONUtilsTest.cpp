#include "JSONUtilsTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunJSONUtilsTests();
    Qentem::QTest::PrintMemoryStatus();

    return ret;
}
