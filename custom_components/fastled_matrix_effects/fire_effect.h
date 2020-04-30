#pragma once

#include "esphome/core/component.h"
#include "esphome/components/light/addressable_light_effect.h"

namespace esphome {
namespace fastled_matrix {

class FastledMatrixFireEffect : public esphome::light::AddressableLightEffect {
public:
  FastledMatrixFireEffect(const std::string &name);

public:
  void start() override;
  void stop() override;
  void apply(esphome::light::AddressableLight &it, const esphome::light::ESPColor &current_color) override;

  void set_hue(uint8_t h);
  void set_sparkles(boolean s);
};

}
}
