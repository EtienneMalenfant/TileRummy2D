#include <tests/test.h>
#include <tests/tests_declaration.h>
#include <iostream>

void Test::validate(bool result, const char* testName) {
    if (!result) {
        throw std::runtime_error({testName + std::string(" a échoué")});
    }
}

void test::runTests() {
    GameTest::runMeldTests();
    GameTest::runMeldManagerTests();
    ActionsAnalyserTest::SmartUtils::runMeldBuilderTests();
    ActionsAnalyserTest::runTests();
    GameBuilderTest::runTests();

    std::cout << "Tous les tests ont réussi" << std::endl;
}
