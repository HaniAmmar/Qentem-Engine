#include "JSONTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    const int ret = Qentem::Test::RunJSONTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
