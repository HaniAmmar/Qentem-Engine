#include "Base64Test.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::Encoding::RunBase64Tests();
    Qentem::QTest::PrintMemoryStatus();

    return ret;
}
