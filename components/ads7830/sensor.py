import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_CHANNEL,
    CONF_ID,
    STATE_CLASS_MEASUREMENT,
    UNIT_VOLT,
    DEVICE_CLASS_VOLTAGE,
)
from . import ads7830_ns, ADS7830Component, CONF_ADS7830_ID

DEPENDENCIES = ["ads7830"]

ADS7830Sensor = ads7830_ns.class_(
    "ADS7830Sensor", sensor.Sensor, cg.PollingComponent
)

CONF_REFERENCE_VOLTAGE = "reference_voltage"

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        ADS7830Sensor,
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=3,
        device_class=DEVICE_CLASS_VOLTAGE,
        state_class=STATE_CLASS_MEASUREMENT,
    )
    .extend(
        {
            cv.GenerateID(CONF_ADS7830_ID): cv.use_id(ADS7830Component),
            cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=7),
            cv.Optional(CONF_REFERENCE_VOLTAGE, default=3.3): cv.float_range(
                min=0.0, max=5.0
            ),
        }
    )
    .extend(cv.polling_component_schema("60s"))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)

    parent = await cg.get_variable(config[CONF_ADS7830_ID])
    cg.add(var.set_parent(parent))
    cg.add(var.set_channel(config[CONF_CHANNEL]))
    cg.add(var.set_reference_voltage(config[CONF_REFERENCE_VOLTAGE]))
