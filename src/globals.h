#pragma once

#include <array>

#include <BluetoothSerial.h>
#include <WiFiType.h>
#include <TFT_eSPI.h>
#include <SPI.h>

#include "controller.h"
#include "display.h"
#include "modeinterface.h"
#include "settings.h"
#include "settingssaver.h"
#include "types.h"

namespace {
int16_t raw_gas, raw_brems;
float gas, brems;
#ifdef FEATURE_GAMETRAK
int16_t raw_gametrakX, raw_gametrakY, raw_gametrakDist;
float gametrakX, gametrakY, gametrakDist;
#endif
float avgSpeed, avgSpeedKmh, sumCurrent, sumAbsoluteCurrent;

char deviceName[32];

Settings settings;
SettingsSaver settingsSaver;

Controller front{Serial1, settings.controllerHardware.enableFrontLeft, settings.controllerHardware.enableFrontRight, settings.controllerHardware.invertFrontLeft, settings.controllerHardware.invertFrontRight};
Controller back{Serial2, settings.controllerHardware.enableBackLeft, settings.controllerHardware.enableBackRight, settings.controllerHardware.invertBackLeft, settings.controllerHardware.invertBackRight};

struct {
    millis_t lastTime = millis();
    int current{0};
    int last{0};
} performance;

BluetoothSerial bluetoothSerial;

TFT_eSPI tft = TFT_eSPI();

ModeInterface *currentMode{};

Display *currentDisplay{};

int rotated{};
bool requestFullRedraw{};
bool confirmButtonPressed{};
bool confirmButtonLongPressed{};
bool backButtonPressed{};
bool backButtonLongPressed{};

class InputDispatcher
{
public:
    static void rotate(int offset)
    {
        rotated += offset;
    }

    static void confirmButton(bool pressed)
    {
        static millis_t pressBegin = 0;

        const auto now = millis();

        if (pressed)
            pressBegin = now;
        else
        {
            const auto duration = now - pressBegin;

            if (duration < 500)
                confirmButtonPressed = true;
            else if (duration < 2000)
                confirmButtonLongPressed = true;
            else
                requestFullRedraw = true;

            pressBegin = 0;
        }
    }

    static void backButton(bool pressed)
    {
        static millis_t pressBegin = 0;

        const auto now = millis();

        if (pressed)
            pressBegin = now;
        else
        {
            const auto duration = now - pressBegin;

            if (duration < 500)
                backButtonPressed = true;
            else
                backButtonLongPressed = true;

            pressBegin = 0;
        }
    }
};
}
