#pragma once

#include "menudisplay.h"
#include "staticmenudefinition.h"
#include "utils.h"
#include "menuitem.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "texts.h"

namespace {
class StarfieldDisplay;
class PingPongDisplay;
class SpiroDisplay;
class GameOfLifeDisplay;
class MainMenu;
}

namespace {
class DemosMenu :
    public MenuDisplay,
    public StaticText<TEXT_DEMOS>,
    public BackActionInterface<SwitchScreenAction<MainMenu>>,
    public StaticMenuDefinition<
        makeComponent<MenuItem, StaticText<TEXT_STARFIELD>,  SwitchScreenAction<StarfieldDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_PINGPONG>,   SwitchScreenAction<PingPongDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_SPIRO>,      SwitchScreenAction<SpiroDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_GAMEOFLIFE>, SwitchScreenAction<GameOfLifeDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_BACK>,       SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&icons::back>>
    >
{};
}
