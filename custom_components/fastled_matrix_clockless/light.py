import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import light
from esphome.const import CONF_OUTPUT_ID, CONF_NUM_LEDS, CONF_RGB_ORDER, CONF_MAX_REFRESH_RATE
from esphome.const import CONF_CHIPSET, CONF_NUM_LEDS, CONF_PIN, CONF_RGB_ORDER
from esphome.const import CONF_WIDTH, CONF_ROTATION, CONF_TYPE, CONF_MAX_CURRENT
from esphome.core import coroutine

fastled_matrix_base_ns = cg.esphome_ns.namespace('fastled_matrix_base')
FastLEDMatrixOutput = fastled_matrix_base_ns.class_('FastLEDMatrixOutput', light.AddressableLight)

RGB_ORDERS = [
    'RGB',
    'RBG',
    'GRB',
    'GBR',
    'BRG',
    'BGR',
]

CHIPSETS = [
    'NEOPIXEL',
    'TM1829',
    'TM1809',
    'TM1804',
    'TM1803',
    'UCS1903',
    'UCS1903B',
    'UCS1904',
    'UCS2903',
    'WS2812',
    'WS2852',
    'WS2812B',
    'SK6812',
    'SK6822',
    'APA106',
    'PL9823',
    'WS2811',
    'WS2813',
    'APA104',
    'WS2811_400',
    'GW6205',
    'GW6205_400',
    'LPD1886',
    'LPD1886_8BIT',
]

MATRIX_TYPE = {
    'NEO_MATRIX_TOP'         : 0x00, # Pixel 0 is at top of matrix
    'NEO_MATRIX_BOTTOM'      : 0x01, # Pixel 0 is at bottom of matrix
    'NEO_MATRIX_LEFT'        : 0x00, # Pixel 0 is at left of matrix
    'NEO_MATRIX_RIGHT'       : 0x02, # Pixel 0 is at right of matrix
    'NEO_MATRIX_CORNER'      : 0x03, # Bitmask for pixel 0 matrix corner
    'NEO_MATRIX_ROWS'        : 0x00, # Matrix is row major (horizontal)
    'NEO_MATRIX_COLUMNS'     : 0x04, # Matrix is column major (vertical)
    'NEO_MATRIX_AXIS'        : 0x04, # Bitmask for row/column layout
    'NEO_MATRIX_PROGRESSIVE' : 0x00, # Same pixel order across each line
    'NEO_MATRIX_ZIGZAG'      : 0x08, # Pixel order reverses between lines
    'NEO_MATRIX_SEQUENCE'    : 0x08, # Bitmask for pixel line order
    'NEO_TILE_TOP'           : 0x00, # First tile is at top of matrix
    'NEO_TILE_BOTTOM'        : 0x10, # First tile is at bottom of matrix
    'NEO_TILE_LEFT'          : 0x00, # First tile is at left of matrix
    'NEO_TILE_RIGHT'         : 0x20, # First tile is at right of matrix
    'NEO_TILE_CORNER'        : 0x30, # Bitmask for first tile corner
    'NEO_TILE_ROWS'          : 0x00, # Tiles ordered in rows
    'NEO_TILE_COLUMNS'       : 0x40, # Tiles ordered in columns
    'NEO_TILE_AXIS'          : 0x40, # Bitmask for tile H/V orientation
    'NEO_TILE_PROGRESSIVE'   : 0x00, # Same tile order across each line
    'NEO_TILE_ZIGZAG'        : 0x80, # Tile order reverses between lines
    'NEO_TILE_SEQUENCE'      : 0x80, # Bitmask for tile line order
}

MATRIX_TYPES = list(MATRIX_TYPE.keys())

CONF_HEIGHT='height'
CONF_MAX_BRIGHTNESS='max_brightness'

@coroutine
def new_fastled_matrix(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    yield cg.register_component(var, config)

    if CONF_MAX_REFRESH_RATE in config:
        cg.add(var.set_max_refresh_rate(config[CONF_MAX_REFRESH_RATE]))

    yield light.register_light(var, config)
    cg.add_library('FastLED', '3.2.9')
    cg.add_library('https://github.com/marcmerlin/FastLED_NeoMatrix.git', '')
    cg.add_library('https://github.com/marcmerlin/Framebuffer_GFX.git', '')
    cg.add_library('Adafruit GFX Library', '')
    cg.add_library('Adafruit BusIO', '')    
    cg.add_library('SPI', '')
    cg.add_library('Wire', '')
    yield var

def validate(value):
    if value[CONF_CHIPSET] == 'NEOPIXEL' and CONF_RGB_ORDER in value:
        raise cv.Invalid("NEOPIXEL doesn't support RGB order")
    return value


FASTLED_SCHEMA = light.ADDRESSABLE_LIGHT_SCHEMA.extend({
    cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(FastLEDMatrixOutput),
    cv.Required(CONF_NUM_LEDS): cv.positive_not_null_int,
    cv.Optional(CONF_RGB_ORDER): cv.one_of(*RGB_ORDERS, upper=True),
    cv.Optional(CONF_MAX_REFRESH_RATE): cv.positive_time_period_microseconds,
}).extend(cv.COMPONENT_SCHEMA)

CONFIG_SCHEMA = cv.All(FASTLED_SCHEMA.extend({
    cv.Required(CONF_CHIPSET): cv.one_of(*CHIPSETS, upper=True),
    cv.Required(CONF_TYPE): cv.ensure_list(cv.one_of(*MATRIX_TYPES, upper=True)),
    cv.Required(CONF_PIN): pins.output_pin,
    cv.Optional(CONF_WIDTH, default=16): cv.uint8_t,
    cv.Optional(CONF_HEIGHT, default=16): cv.uint8_t,
    cv.Optional(CONF_ROTATION, default=0): cv.uint8_t,
    cv.Optional(CONF_MAX_CURRENT, default=1000): cv.uint32_t,
    cv.Optional(CONF_MAX_BRIGHTNESS, default=255): cv.uint8_t,
}), validate)


def to_code(config):
    var = yield new_fastled_matrix(config)

    rgb_order = None
    if CONF_RGB_ORDER in config:
        rgb_order = cg.RawExpression(config[CONF_RGB_ORDER])
    template_args = cg.TemplateArguments(cg.RawExpression(config[CONF_CHIPSET]),
                                         config[CONF_PIN], rgb_order)
    cg.add(var.add_leds(template_args, config[CONF_NUM_LEDS]))

    cg.add(var.set_matrix(
        config[CONF_WIDTH],
        config[CONF_HEIGHT],
        sum([MATRIX_TYPE[i] for i in config[CONF_TYPE]])
    ))
    cg.add(var.set_max_current(config[CONF_MAX_CURRENT]))
    cg.add(var.set_max_brightness(config[CONF_MAX_BRIGHTNESS]))
    cg.add(var.set_rotation(config[CONF_ROTATION]))
