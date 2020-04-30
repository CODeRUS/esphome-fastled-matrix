#include "fastled_matrix.h"

#include "esphome/core/log.h"

namespace esphome {
namespace fastled_matrix_base {


const char *TAG = "fastled_matrix";

void FastLEDMatrixOutput::setup() {
  ESP_LOGCONFIG(TAG, "Setting up FastLED matrix...");
  this->controller_->init();
  this->controller_->setLeds(this->leds_, this->num_leds_);
  this->effect_data_ = new uint8_t[this->num_leds_];
  if (!this->max_refresh_rate_.has_value()) {
    this->set_max_refresh_rate(this->controller_->getMaxRefreshRate());
  }
}

void FastLEDMatrixOutput::dump_config() {
  ESP_LOGCONFIG(TAG, "FastLED matrix:");
  ESP_LOGCONFIG(TAG, "  Num LEDs: %u", this->num_leds_);
  ESP_LOGCONFIG(TAG, "  Max refresh rate: %u", *this->max_refresh_rate_);
}

void FastLEDMatrixOutput::loop() {
  if (!this->should_show_())
    return;

  uint32_t now = micros();
  // protect from refreshing too often
  if (*this->max_refresh_rate_ != 0 && (now - this->last_refresh_) < *this->max_refresh_rate_) {
    return;
  }
  this->last_refresh_ = now;
  this->mark_shown_();

  ESP_LOGVV(TAG, "Writing RGB values to bus...");
  this->controller_->showLeds();
}

void FastLEDMatrixOutput::set_matrix(uint8_t width, uint8_t height, uint8_t matrixType)
{
  if (matrix_) {
    return;  
  }
  matrix_ = new MyMatrix(leds_, width, height, matrixType);
}

void FastLEDMatrixOutput::set_max_brightness(uint8_t maxBrightness)
{
  ESP_LOGCONFIG(TAG, "  Settings max brightness to: %u", maxBrightness);
  FastLED.setBrightness(maxBrightness);
}

void FastLEDMatrixOutput::set_max_current(uint32_t maxCurrent)
{
  ESP_LOGCONFIG(TAG, "  Settings max current to: %u", maxCurrent);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, maxCurrent);
}

void FastLEDMatrixOutput::set_rotation(uint8_t rotation)
{
  if (!matrix_) {
    return;
  }
  ESP_LOGCONFIG(TAG, "  Settings rotation to: %u", rotation);
  matrix_->setRotation(rotation);
}

}  // namespace fastled_base
}  // namespace esphome