import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components.light.types import AddressableLightEffect
from esphome.components.light.effects import register_addressable_effect
from esphome.const import CONF_ID, CONF_NAME
from esphome.core import coroutine

fastled_matrix_ns = cg.esphome_ns.namespace('fastled_matrix')

FastledMatrixEffectsComponent = fastled_matrix_ns.class_('FastledMatrixEffectsComponent', cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(FastledMatrixEffectsComponent),
})

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)


FastledMatrixFireEffect = fastled_matrix_ns.class_('FastledMatrixFireEffect', AddressableLightEffect)

CONF_FIRE_ID = 'fire_id'
CONF_HUE = 'hue'
CONF_SPARKLES = 'sparkles'

@register_addressable_effect('fire', FastledMatrixFireEffect, "Fire", {
    cv.GenerateID(CONF_FIRE_ID): cv.use_id(FastledMatrixEffectsComponent),
    cv.Optional(CONF_HUE, default=0): cv.uint8_t,
    cv.Optional(CONF_SPARKLES, default=True): cv.boolean,
})

def fire_effect_to_code(config, effect_id):
    effect = cg.new_Pvariable(effect_id, config[CONF_NAME])
    cg.add(effect.set_hue(config[CONF_HUE]))
    cg.add(effect.set_sparkles(config[CONF_SPARKLES]))

    yield effect
