#pragma once

#if defined(ESP8266)
#define FASTLED_ESP8266_RAW_PIN_ORDER
//#define FASTLED_INTERRUPT_RETRY_COUNT 0
//#define FASTLED_ALLOW_INTERRUPTS 0
#endif
//#define FASTLED_USE_PROGMEM 1

#include <Arduino.h>
#include <FastLED.h>
#include <Adafruit_GFX.h>
#include <FastLED_NeoMatrix.h>

namespace esphome {
namespace fastled_matrix_base {

class MyMatrix : public FastLED_NeoMatrix
{
public:
    MyMatrix(CRGB *leds, uint8_t w, uint8_t h, uint8_t matrixType);

    uint8_t width();
    uint8_t height();

    void setCurrentLimit(uint32_t maxCurrent);
    
    void matrixTest();

    void clear(bool shouldShow = false);
    void fill(CRGB color, bool shouldShow = false);
    void fillProgress(double progress);
    void setLed(uint16_t index, CRGB color);
    void fadeToBlackBy(uint16_t index, uint8_t step);

    uint16_t getPixelNumberXY(uint8_t x, uint8_t y);

    void drawPixelXY(uint8_t x, uint8_t y, CRGB color);
    void drawLineXY(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, CRGB color);

    CRGB getPixColor(uint16_t number);
    CRGB getPixColorXY(uint8_t x, uint8_t y);

    void fillRectXY(uint8_t x, uint8_t y, uint8_t w, uint8_t h, CRGB color);

    void fadePixelXY(uint8_t x, uint8_t y, uint8_t step);
};

}
}
