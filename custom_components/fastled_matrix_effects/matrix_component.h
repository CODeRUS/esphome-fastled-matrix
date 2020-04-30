#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace fastled_matrix {

class FastledMatrixEffectsComponent : public esphome::Component {
public:
  void loop() override;
};

}
}
