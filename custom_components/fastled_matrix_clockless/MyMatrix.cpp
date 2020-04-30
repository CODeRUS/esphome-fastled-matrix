#include "MyMatrix.h"
#include "esphome/core/log.h"

namespace  {

const char *TAG = "MyMatrix";

uint8_t sizeWidth = 16;
uint8_t sizeHeight = 16;
uint8_t matrixType = NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT +
                     NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG;

uint8_t rotation = 3;
uint16_t numLeds = 0;

CRGB* leds = nullptr;

} // namespace


namespace esphome {
namespace fastled_matrix_base {

uint8_t MyMatrix::width()
{
    return sizeWidth;
}

uint8_t MyMatrix::height()
{
    return sizeHeight;
}

void MyMatrix::setCurrentLimit(uint32_t maxCurrent)
{
    FastLED.setMaxPowerInVoltsAndMilliamps(5, maxCurrent);
}

MyMatrix::MyMatrix(CRGB *l, uint8_t w, uint8_t h, uint8_t t)
    : FastLED_NeoMatrix(l, w, h, 1, 1, t)
{
    ESP_LOGCONFIG(TAG, "MyMatrix constructor:");
    ESP_LOGCONFIG(TAG, "  width: %u", w);
    ESP_LOGCONFIG(TAG, "  height: %u", h);
    ESP_LOGCONFIG(TAG, "  type: %u", 0);
    ESP_LOGCONFIG(TAG, "  LEDS: %p", (void *) l);

    sizeWidth = w;
    sizeHeight = h;
    matrixType = t;

    numLeds = sizeWidth * sizeHeight;
    leds = l;
}

void MyMatrix::matrixTest()
{
    for (int16_t i = 0; i < 16; i++) {
        clear();
        drawPixelXY(i, 0, CRGB(CRGB::Red));
        drawPixelXY(0, i, CRGB(CRGB::Green));
        drawPixelXY(i, i, CRGB(CRGB::White));
        show();
        delay(100);
    }

    clear();
    show();
}

void MyMatrix::fill(CRGB color, bool shouldShow)
{
    fill_solid(leds, numLeds, color);
    if (shouldShow) {
        show();
    }
}

void MyMatrix::fillProgress(double progress)
{
    FastLED.clear();

    const uint16_t number = static_cast<uint16_t>(numLeds * progress);
    const uint8_t fullRows = static_cast<uint8_t>(number / width());
    for (uint8_t y = 0; y < fullRows; ++y) {
        for (uint8_t x = 0; x < width(); ++x) {
            drawPixelXY(x, y, CRGB(5, 5, 5));
        }
    }

    const uint8_t remainingProgress = static_cast<uint8_t>(number % width());
    for (uint8_t x = 0; x < remainingProgress; ++x) {
        drawPixelXY(x, fullRows, CRGB(5, 5, 5));
    }

    const String percent = String(static_cast<int>(progress * 100));
    setCursor(0, 5);
    setPassThruColor(CRGB(40, 0, 0));
    print(percent);
    setPassThruColor();
    delay(1);

    FastLED.show();
}

void MyMatrix::setLed(uint16_t index, CRGB color)
{
    leds[index] = color;
}

void MyMatrix::fadeToBlackBy(uint16_t index, uint8_t step)
{
    leds[index].fadeToBlackBy(step);
}

void MyMatrix::clear(bool shouldShow)
{
    FastLED.clear();
    if (shouldShow) {
        delay(1);
        show();
    }
}

uint16_t MyMatrix::getPixelNumberXY(uint8_t x, uint8_t y)
{
    return static_cast<uint16_t>(XY(y, x));
}

void MyMatrix::drawPixelXY(uint8_t x, uint8_t y, CRGB color)
{
    leds[getPixelNumberXY(x, y)] = color;
}

void MyMatrix::drawLineXY(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, CRGB color)
{
    setPassThruColor(color);
    drawLine(y0, x0, y1, x1, 0);
    setPassThruColor();
}

CRGB MyMatrix::getPixColor(uint16_t number)
{
    if (number > numLeds - 1) {
        return 0;
    }
    return leds[number];
}

CRGB MyMatrix::getPixColorXY(uint8_t x, uint8_t y)
{
    return getPixColor(getPixelNumberXY(x, y));
}

void MyMatrix::fillRectXY(uint8_t x, uint8_t y, uint8_t w, uint8_t h, CRGB color)
{
    setPassThruColor(color);
    fillRect(y, x, h, w, 0);
    setPassThruColor();
}

void MyMatrix::fadePixelXY(uint8_t x, uint8_t y, uint8_t step)
{
    const uint16_t pixelNum = getPixelNumberXY(x, y);
    const CRGB color = getPixColor(pixelNum);

    if (!color) {
        return;
    }

    if (color.r >= 30 || color.g >= 30 || color.b >= 30) {
        fadeToBlackBy(pixelNum, step);
    } else {
        setLed(pixelNum, CRGB::Black);
    }
}

}
}
