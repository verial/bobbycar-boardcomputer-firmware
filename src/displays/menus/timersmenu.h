#pragma once

#include "changevaluedisplay.h"
#include "menudisplay.h"
#include "staticmenudefinition.h"
#include "utils.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "texts.h"
#include "settingsaccessors.h"

namespace {
class BoardcomputerHardwareSettingsMenu;
}

namespace {
class TimersMenu;

using PotiReadRateChangeDisplay = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_POTIREADRATE>,
    PotiReadRateAccessor,
    BackActionInterface<SwitchScreenAction<TimersMenu>>,
    SwitchScreenAction<TimersMenu>
>;

using ModeUpdateRateChangeDisplay = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_MODEUPDATERATE>,
    ModeUpdateRateAccessor,
    BackActionInterface<SwitchScreenAction<TimersMenu>>,
    SwitchScreenAction<TimersMenu>
>;

using StatsUpdateRateChangeDisplay = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_STATSUPDATERATE>,
    StatsUpdateRateAccessor,
    BackActionInterface<SwitchScreenAction<TimersMenu>>,
    SwitchScreenAction<TimersMenu>
>;

using DisplayUpdateRateChangeDisplay = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_DISPLAYUPDATERATE>,
    DisplayUpdateRateAccessor,
    BackActionInterface<SwitchScreenAction<TimersMenu>>,
    SwitchScreenAction<TimersMenu>
>;

using DisplayRedrawRateChangeDisplay = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_DISPLAYREDRAWRATE>,
    DisplayRedrawRateAccessor,
    BackActionInterface<SwitchScreenAction<TimersMenu>>,
    SwitchScreenAction<TimersMenu>
>;

class TimersMenu :
    public MenuDisplay,
    public StaticText<TEXT_TIMERS>,
    public BackActionInterface<SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    public StaticMenuDefinition<
        makeComponent<MenuItem, StaticText<TEXT_POTIREADRATE>,      SwitchScreenAction<PotiReadRateChangeDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_MODEUPDATERATE>,    SwitchScreenAction<ModeUpdateRateChangeDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_STATSUPDATERATE>,   SwitchScreenAction<StatsUpdateRateChangeDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_DISPLAYUPDATERATE>, SwitchScreenAction<DisplayUpdateRateChangeDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_DISPLAYREDRAWRATE>, SwitchScreenAction<DisplayRedrawRateChangeDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_BACK>,              SwitchScreenAction<BoardcomputerHardwareSettingsMenu>, StaticMenuItemIcon<&icons::back>>
    >
{};
}
