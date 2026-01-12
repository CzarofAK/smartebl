import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

CODEOWNERS = ["@smartebl"]
DEPENDENCIES = ["uart"]
MULTI_CONF = True

CONF_LIN_BUS_ID = "lin_bus_id"
CONF_SLEEP_PIN = "sleep_pin"
CONF_WAKE_PIN = "wake_pin"
CONF_NODE_ADDRESS = "node_address"
CONF_IS_MASTER = "is_master"

lin_bus_ns = cg.esphome_ns.namespace("lin_bus")
LINBusComponent = lin_bus_ns.class_(
    "LINBusComponent", cg.Component, uart.UARTDevice
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(LINBusComponent),
            cv.Optional(CONF_SLEEP_PIN): cv.int_,
            cv.Optional(CONF_WAKE_PIN): cv.int_,
            cv.Optional(CONF_NODE_ADDRESS, default=0x00): cv.hex_uint8_t,
            cv.Optional(CONF_IS_MASTER, default=True): cv.boolean,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_SLEEP_PIN in config:
        cg.add(var.set_sleep_pin(config[CONF_SLEEP_PIN]))
    if CONF_WAKE_PIN in config:
        cg.add(var.set_wake_pin(config[CONF_WAKE_PIN]))

    cg.add(var.set_node_address(config[CONF_NODE_ADDRESS]))
    cg.add(var.set_is_master(config[CONF_IS_MASTER]))
