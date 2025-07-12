#include "JSONTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunJSONTests();
    Qentem::QTest::PrintMemoryStatus();

    return ret;
}
