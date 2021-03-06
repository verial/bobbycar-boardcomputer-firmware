#pragma once

#include <algorithm>
#include <utility>

#include <ArduinoOTA.h>
#include <WString.h>
#include <WiFi.h>

#include "display.h"
#include "globals.h"

// macros are a shit piece of software
#define STRING2(s) #s
#define STRING(s) STRING2(s)

namespace {
bool currentlyReverseBeeping;
bool reverseBeepToggle;
unsigned int lastReverseBeepToggle;

template<typename ...T>
class makeComponent : public T...
{};

template <typename T1, typename T2, typename ...T3>
class makeComponentArgs : public T1, public T2, public T3...
{
public:
    template<typename ...T>
    makeComponentArgs(T&& ...args) :
        T2{std::forward<T>(args)...}
    {
    }
};

template<typename T>
T scaleBetween(T x, T in_min, T in_max, T out_min, T out_max) {
    if (x < std::min(in_min, in_max))
        x = std::min(in_min, in_max);
    else if (x > std::max(in_min, in_max))
        x = std::max(in_min, in_max);

    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
};

float convertToKmh(float val)
{
    return val /* / settings.controllerHardware.numMagnetPoles */ / 60.f * settings.controllerHardware.wheelDiameter / 1000.f * 3.14159265359f * 3.6f;
}

float convertFromKmh(float val)
{
    return val /* * settings.controllerHardware.numMagnetPoles */ * 60.f / settings.controllerHardware.wheelDiameter * 1000.f / 3.14159265359f / 3.6f;
}

float convertToInch(float val)
{
    return val / 25.4f;
}

float convertFromInch(float val)
{
    return val * 25.4f;
}

float fixCurrent(int16_t value)
{
    return -value/50.;
}

float fixBatVoltage(int16_t value)
{
    return value/100.;
}

float fixBoardTemp(int16_t value)
{
    return value/10.;
}

String hallString(const MotorFeedback &motor)
{
    return String{} + (motor.hallA ? '1' : '0') + (motor.hallB ? '1' : '0') + (motor.hallC ? '1' : '0');
}

template<typename T>
String toString(T value)
{
    return String{} + value;
}

template<>
String toString<bool>(bool value)
{
    return value ? "true" : "false";
}

template<>
String toString<ControlType>(ControlType value)
{
    switch (value)
    {
    case ControlType::Commutation: return "Commutation";
    case ControlType::Sinusoidal: return "Sinusoidal";
    case ControlType::FieldOrientedControl: return "FieldOrientedControl";
    }
    return String("Unknown: ") + int(value);
}

template<>
String toString<ControlMode>(ControlMode value)
{
    switch (value)
    {
    case ControlMode::OpenMode: return "OpenMode";
    case ControlMode::Voltage: return "Voltage";
    case ControlMode::Speed: return "Speed";
    case ControlMode::Torque: return "Torque";
    }
    return String("Unknown: ") + int(value);
}

template<>
String toString<wl_status_t>(wl_status_t value)
{
    switch (value)
    {
    case WL_NO_SHIELD: return "WL_NO_SHIELD";
    case WL_IDLE_STATUS: return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL: return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED: return "WL_SCAN_COMPLETED";
    case WL_CONNECTED: return "WL_CONNECTED";
    case WL_CONNECT_FAILED: return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED: return "WL_DISCONNECTED";
    }

    return String("Unknown: ") + int(value);
}

template<>
String toString<ota_error_t>(ota_error_t value)
{
    switch (value)
    {
    case OTA_AUTH_ERROR: return "OTA_AUTH_ERROR";
    case OTA_BEGIN_ERROR: return "OTA_BEGIN_ERROR";
    case OTA_CONNECT_ERROR: return "OTA_CONNECT_ERROR";
    case OTA_RECEIVE_ERROR: return "OTA_RECEIVE_ERROR";
    case OTA_END_ERROR: return "OTA_END_ERROR";
    }

    return String("Unknown: ") + int(value);
}

std::array<std::reference_wrapper<MotorState>, 2> motorsInController(Controller &controller)
{
    return {std::ref(controller.command.left), std::ref(controller.command.right)};
}

std::array<std::reference_wrapper<const MotorState>, 2> motorsInController(const Controller &controller)
{
    return {std::ref(controller.command.left), std::ref(controller.command.right)};
}

std::array<std::reference_wrapper<MotorFeedback>, 2> motorFeedbacksInController(Controller &controller)
{
    return {std::ref(controller.feedback.left), std::ref(controller.feedback.right)};
}

std::array<std::reference_wrapper<const MotorFeedback>, 2> motorFeedbacksInController(const Controller &controller)
{
    return {std::ref(controller.feedback.left), std::ref(controller.feedback.right)};
}

std::array<std::reference_wrapper<MotorState>, 4> motors()
{
    return {
        std::ref(controllers.front.command.left), std::ref(controllers.front.command.right),
        std::ref(controllers.back.command.left), std::ref(controllers.back.command.right)
    };
}

void fixCommonParams()
{
    for (MotorState &motor : motors())
    {
        motor.iMotMax = settings.limits.iMotMax;
        motor.iDcMax = settings.limits.iDcMax;
        motor.nMotMax = settings.limits.nMotMax;
        motor.fieldWeakMax = settings.limits.fieldWeakMax;
        motor.phaseAdvMax = settings.limits.phaseAdvMax;
    }

    if (settings.reverseBeep)
    {
        const auto x = motors();
        const auto shouldBeep = std::all_of(std::begin(x), std::end(x), [](const MotorState &motor){ return motor.pwm < 0; });

        if (shouldBeep != currentlyReverseBeeping)
        {
            if (shouldBeep)
            {
                reverseBeepToggle = true;
                lastReverseBeepToggle = millis();
                for (auto &controller : controllers)
                    controller.command.buzzer = {.freq=settings.reverseBeepFreq0, .pattern=0};
            }
            else
                for (auto &controller : controllers)
                    controller.command.buzzer = {};

            currentlyReverseBeeping = shouldBeep;
        }
        else if (shouldBeep && millis() - lastReverseBeepToggle >= (reverseBeepToggle?settings.reverseBeepDuration0:settings.reverseBeepDuration1))
        {
            reverseBeepToggle = !reverseBeepToggle;

            for (auto &controller : controllers)
                controller.command.buzzer = {.freq=uint8_t(reverseBeepToggle?settings.reverseBeepFreq0:settings.reverseBeepFreq1), .pattern=0};

            lastReverseBeepToggle = millis();
        }
    }
    else if (currentlyReverseBeeping)
    {
        for (auto &controller : controllers)
            controller.command.buzzer = {};
        currentlyReverseBeeping = false;
    }

    controllers.front.command.left.enable = settings.controllerHardware.enableFrontLeft;
    controllers.front.command.right.enable = settings.controllerHardware.enableFrontRight;
    controllers.back.command.left.enable = settings.controllerHardware.enableBackLeft;
    controllers.back.command.right.enable = settings.controllerHardware.enableBackRight;

    if (settings.controllerHardware.invertFrontLeft)
        controllers.front.command.left.pwm = -controllers.front.command.left.pwm;
    if (settings.controllerHardware.invertFrontRight)
        controllers.front.command.right.pwm = -controllers.front.command.right.pwm;
    if (settings.controllerHardware.invertBackLeft)
        controllers.back.command.left.pwm = -controllers.back.command.left.pwm;
    if (settings.controllerHardware.invertBackRight)
        controllers.back.command.right.pwm = -controllers.back.command.right.pwm;
}

void sendCommands()
{
    for (Controller &controller : controllers)
    {
        controller.command.start = Command::VALID_HEADER;
        controller.command.checksum = calculateChecksum(controller.command);
        controller.serial.get().write((uint8_t *) &controller.command, sizeof(controller.command));
    }
}

template<typename T, typename... Args>
void switchScreen(Args&&... args);

void updateSwapFrontBack()
{
    controllers.front.serial = settings.controllerHardware.swapFrontBack ? Serial2 : Serial1;
    controllers.back.serial = settings.controllerHardware.swapFrontBack ? Serial1 : Serial2;
}

void loadSettings()
{
    settingsPersister.load(settings);
}

void saveSettings()
{
    settingsPersister.save(settings);
}

void updateAccumulators()
{
    avgSpeed = 0.f;
    sumCurrent = 0.f;
    uint8_t count{0};

    for (const Controller &controller : controllers)
    {
        if (!controller.feedbackValid)
            continue;

        avgSpeed +=
                controller.feedback.left.speed * (controller.invertLeft ? -1 : 1) +
                controller.feedback.right.speed * (controller.invertRight ? -1 : 1);

        sumCurrent +=
                controller.feedback.left.current +
                controller.feedback.right.current;

        count +=2;
    }

    if (count)
        avgSpeed /= count;

    sumCurrent = fixCurrent(sumCurrent);

    avgSpeedKmh = convertToKmh(avgSpeed);
}

void readPotis()
{
    const auto sampleMultipleTimes = [](int pin){
        analogRead(pin);
        double sum{};
        for (int i = 0; i < settings.boardcomputerHardware.sampleCount; i++)
            sum += analogRead(pin);
        return sum/settings.boardcomputerHardware.sampleCount;
    };

    raw_gas = sampleMultipleTimes(PINS_GAS);
    gas = scaleBetween<float>(raw_gas, settings.boardcomputerHardware.gasMin, settings.boardcomputerHardware.gasMax, 0., 1000.);

    raw_brems = sampleMultipleTimes(PINS_BREMS);
    brems = scaleBetween<float>(raw_brems, settings.boardcomputerHardware.bremsMin, settings.boardcomputerHardware.bremsMax, 0., 1000.);

#ifdef FEATURE_GAMETRAK
    raw_gametrakX = sampleMultipleTimes(PINS_GAMETRAKX);
    gametrakX = scaleBetween<float>(raw_gametrakX, settings.boardcomputerHardware.gametrakXMin, settings.boardcomputerHardware.gametrakXMax, 0., 1000.);

    raw_gametrakY = sampleMultipleTimes(PINS_GAMETRAKY);
    gametrakY = scaleBetween<float>(raw_gametrakY, settings.boardcomputerHardware.gametrakYMin, settings.boardcomputerHardware.gametrakYMax, 0., 1000.);

    raw_gametrakDist = sampleMultipleTimes(PINS_GAMETRAKDIST);
    gametrakDist = scaleBetween<float>(raw_gametrakDist, settings.boardcomputerHardware.gametrakDistMin, settings.boardcomputerHardware.gametrakDistMax, 0., 1000.);
#endif
}
}
