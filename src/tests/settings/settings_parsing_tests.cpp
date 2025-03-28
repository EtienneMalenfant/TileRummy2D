#include <tests/tests_declaration.h>
#include <tests/test.h>
#include <settings/settings_loader.h>

namespace SettingsTest {
    void runTests() {
        runAppSettingsTests();
        runGameSettingsTests();
    }
}
