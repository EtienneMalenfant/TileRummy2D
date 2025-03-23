#pragma once

#include <stdexcept>
#include <game/melds_manager.h>


namespace test {
    void runTests();
}
    
class Test {
    public:
    static void validate(bool result, const char* testName);
};