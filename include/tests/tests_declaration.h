#pragma once
#include <game/melds_manager.h>

namespace GameTest {

    void runMeldTests();
    void runMeldManagerTests();
    void buildMelds(IMeldsManager* meldsManager);

}

namespace ActionsAnalyserTest {

    void runTests();

    namespace SmartUtils {
        void runMeldBuilderTests();
    }
}

namespace GameBuilderTest {

    void runTests();
}

namespace SettingsTest {
    void runTests();
}
