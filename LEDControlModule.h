#ifndef LEDCONTROLMODULE_H
#define LEDCONTROLMODULE_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <memory>
#include <vector>
#include <emsesp.h>


#define LED_PIN 6

namespace emsesp {

class LEDControlModule {
public:
    class LEDConfig {
    public:
        LEDConfig(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, bool blink = false)
            : r(r), g(g), b(b), blink(blink) {}

        uint8_t r;
        uint8_t g;
        uint8_t b;
        bool blink;
    };

    LEDControlModule();

    void start(emsesp::EMSESP *emsesp_main, bool test_mode = false);
    void loop();
    void configureLED(size_t index, uint8_t r, uint8_t g, uint8_t b, bool blink);
    void list(JsonObject output);

private:
    static constexpr int NUM_LEDS = 8;

    Adafruit_NeoPixel strip;
    std::vector<LEDConfig> ledConfigs;
    unsigned long lastBlinkTime = 0;
    bool blinkState = false;

    void setLED(size_t index, uint8_t r, uint8_t g, uint8_t b);
    void updateLEDs();
    void handleBlink();
};

} // namespace emsesp

#endif