#pragma once

#include "changevaluedisplay.h"
#include "menudisplay.h"
#include "staticmenudefinition.h"
#include "menuitem.h"
#include "actions/dummyaction.h"
#include "actions/bluetoothbeginaction.h"
#include "actions/bluetoothbeginmasteraction.h"
#include "actions/bluetoothflushaction.h"
#include "actions/bluetoothendaction.h"
#include "actions/bluetoothdisconnectaction.h"
#include "actions/switchscreenaction.h"
#include "bluetoothtexthelpers.h"
#include "settingsaccessors.h"
#include "icons/back.h"
#include "texts.h"

namespace {
class SettingsMenu;
}

namespace {
class BluetoothSettingsMenu;

using AutoBluetoothModeChangeDisplay = makeComponent<
    ChangeValueDisplay<BluetoothMode>,
    StaticText<TEXT_AUTOBLUETOOTHMODE>,
    AutoBluetoothModeAccessor,
    BackActionInterface<SwitchScreenAction<BluetoothSettingsMenu>>,
    SwitchScreenAction<BluetoothSettingsMenu>
>;

class BluetoothSettingsMenu :
    public MenuDisplay,
    public StaticText<TEXT_BLUETOOTHSETTINGS>,
    public BackActionInterface<SwitchScreenAction<SettingsMenu>>,
    public StaticMenuDefinition<
        makeComponent<MenuItem, BluetoothAvailableText,                 DisabledColor, DummyAction>,
        makeComponent<MenuItem, BluetoothHasClientText,                 DisabledColor, DummyAction>,
//        makeComponent<MenuItem, BluetoothConnectedText,                 DisabledColor, DummyAction>, // crashes
        makeComponent<MenuItem, BluetoothIsReadyText,                   DisabledColor, DummyAction>,
        makeComponent<MenuItem, BluetoothIsReadyMasterText,             DisabledColor, DummyAction>,
        makeComponent<MenuItem, StaticText<TEXT_BLUETOOTHBEGIN>,        BluetoothBeginAction>,
        makeComponent<MenuItem, StaticText<TEXT_BLUETOOTHBEGINMASTER>,  BluetoothBeginMasterAction>,
        makeComponent<MenuItem, StaticText<TEXT_BLUETOOTHFLUSH>,        BluetoothFlushAction>,
        makeComponent<MenuItem, StaticText<TEXT_BLUETOOTHEND>,          BluetoothEndAction>,
        makeComponent<MenuItem, StaticText<TEXT_BLUETOOTHDISCONNECT>,   BluetoothDisconnectAction>,
        makeComponent<MenuItem, StaticText<TEXT_AUTOBLUETOOTHMODE>,     SwitchScreenAction<AutoBluetoothModeChangeDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_BACK>,                  SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&icons::back>>
    >
{};
}
