#include "JSONTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    int ret = Qentem::Test::RunJSONTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
