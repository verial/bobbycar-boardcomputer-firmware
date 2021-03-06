#pragma once

#include <HardwareSerial.h>

#include "menudisplay.h"
#include "staticmenudefinition.h"
#include "utils.h"
#include "actions/toggleboolaction.h"
#include "actions/switchscreenaction.h"
#include "checkboxicon.h"
#include "icons/wifi.h"
#include "icons/bluetooth.h"
#include "icons/hardware.h"
#include "icons/buzzer.h"
#include "icons/info.h"
#include "icons/back.h"
#include "texts.h"
#include "globals.h"

namespace {
class LimitsSettingsMenu;
class WifiSettingsMenu;
class BluetoothSettingsMenu;
class ModesSettingsMenu;
class ControllerHardwareSettingsMenu;
class BoardcomputerHardwareSettingsMenu;
class BuzzerMenu;
class AboutMenu;
class MainMenu;
}

namespace {
struct FrontLedAccessor : public RefAccessor<bool> { bool &getRef() const override { return controllers.front.command.led; } };
struct BackLedAccessor : public RefAccessor<bool> { bool &getRef() const override { return controllers.back.command.led; } };

class SettingsMenu :
    public MenuDisplay,
    public StaticText<TEXT_SETTINGS>,
    public BackActionInterface<SwitchScreenAction<MainMenu>>,
    public StaticMenuDefinition<
        makeComponent<MenuItem, StaticText<TEXT_LIMITSSETTINGS>,                SwitchScreenAction<LimitsSettingsMenu>>,
        makeComponent<MenuItem, StaticText<TEXT_WIFISETTINGS>,                  SwitchScreenAction<WifiSettingsMenu>, StaticMenuItemIcon<&icons::wifi>>,
        makeComponent<MenuItem, StaticText<TEXT_BLUETOOTHSETTINGS>,             SwitchScreenAction<BluetoothSettingsMenu>, StaticMenuItemIcon<&icons::bluetooth>>,
        makeComponent<MenuItem, StaticText<TEXT_MODESSETTINGS>,                 SwitchScreenAction<ModesSettingsMenu>>,
        makeComponent<MenuItem, StaticText<TEXT_CONTROLLERHARDWARESETTINGS>,    SwitchScreenAction<ControllerHardwareSettingsMenu>, StaticMenuItemIcon<&icons::hardware>>,
        makeComponent<MenuItem, StaticText<TEXT_BOARDCOMPUTERHARDWARESETTINGS>, SwitchScreenAction<BoardcomputerHardwareSettingsMenu>, StaticMenuItemIcon<&icons::hardware>>,
#ifdef FEATURE_BMS
        makeComponent<MenuItem, StaticText<TEXT_AUTOCONNECTBMS>,                ToggleBoolAction, CheckboxIcon, AutoConnectBmsAccessor>,
#endif
        makeComponent<MenuItem, StaticText<TEXT_BUZZER>,                        SwitchScreenAction<BuzzerMenu>, StaticMenuItemIcon<&icons::buzzer>>,
        makeComponent<MenuItem, StaticText<TEXT_FRONTLED>,                      ToggleBoolAction, CheckboxIcon, FrontLedAccessor>,
        makeComponent<MenuItem, StaticText<TEXT_BACKLED>,                       ToggleBoolAction, CheckboxIcon, BackLedAccessor>,
        makeComponent<MenuItem, StaticText<TEXT_ABOUT>,                         SwitchScreenAction<AboutMenu>, StaticMenuItemIcon<&icons::info>>,
        makeComponent<MenuItem, StaticText<TEXT_BACK>,                          SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&icons::back>>
    >
{};
}
