#include "LEDControlModule.h"

namespace emsesp {

LEDControlModule::LEDControlModule()
    : strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800), ledConfigs(NUM_LEDS) {
    strip.begin();
    strip.show(); // Initialize all LEDs to off
}

void LEDControlModule::start(emsesp::EMSESP* emsesp_main, bool test_mode) {
    updateLEDs();
    EMSESP::logger().info("LEDControlModule started");
}

void LEDControlModule::loop() {
    handleBlink();
}

void LEDControlModule::configureLED(size_t index, uint8_t r, uint8_t g, uint8_t b, bool blink) {
    if (index >= NUM_LEDS) {
        EMSESP::logger().info("Invalid LED index: %d", index);
        return;
    }

    ledConfigs[index].r = r;
    ledConfigs[index].g = g;
    ledConfigs[index].b = b;
    ledConfigs[index].blink = blink;

    updateLEDs();
}

void LEDControlModule::list(JsonObject output) {
    for (size_t i = 0; i < NUM_LEDS; ++i) {
        char ledKey[8];
        snprintf(ledKey, sizeof(ledKey), "led%d", static_cast<int>(i + 1));

        JsonObject led = output.createNestedObject(ledKey);
        led["r"] = ledConfigs[i].r;
        led["g"] = ledConfigs[i].g;
        led["b"] = ledConfigs[i].b;
        led["blink"] = ledConfigs[i].blink;
    }
}

void LEDControlModule::setLED(size_t index, uint8_t r, uint8_t g, uint8_t b) {
    strip.setPixelColor(index, strip.Color(r, g, b));
}

void LEDControlModule::updateLEDs() {
    for (size_t i = 0; i < NUM_LEDS; ++i) {
        setLED(i, ledConfigs[i].r, ledConfigs[i].g, ledConfigs[i].b);
    }
    strip.show();
}

void LEDControlModule::handleBlink() {
    unsigned long currentTime = millis();
    if (currentTime - lastBlinkTime >= 500) { // 1 Hz (500 ms on/off)
        lastBlinkTime = currentTime;
        blinkState = !blinkState;

        for (size_t i = 0; i < NUM_LEDS; ++i) {
            if (ledConfigs[i].blink) {
                if (blinkState) {
                    setLED(i, ledConfigs[i].r, ledConfigs[i].g, ledConfigs[i].b);
                } else {
                    setLED(i, 0, 0, 0); // Turn off the LED
                }
            }
        }
        strip.show();
    }
}

} // namespace emsesp
