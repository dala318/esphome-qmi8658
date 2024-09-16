import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    CONF_ADDRESS,
    CONF_ACCELERATION_X,
    CONF_ACCELERATION_Y,
    CONF_ACCELERATION_Z,
    CONF_GYROSCOPE_X,
    CONF_GYROSCOPE_Y,
    CONF_GYROSCOPE_Z,
    CONF_INTERRUPT_PIN,
    CONF_TEMPERATURE,
    CONF_ID,
    UNIT_DEGREE_PER_SECOND,
    UNIT_G,
    UNIT_METER_PER_SECOND_SQUARED,
    DEVICE_CLASS_TEMPERATURE,
    ICON_SCREEN_ROTATION,
    ICON_BRIEFCASE_DOWNLOAD,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
)
from esphome import pins

CONF_INTEERUPT_GROUP = "interrupt_group"
CONF_INTERRUPT_PIN_1 = CONF_INTERRUPT_PIN + "_1"
CONF_INTERRUPT_PIN_2 = CONF_INTERRUPT_PIN + "_2"

# CONF_ACCELERATION_RANGE = "acceleration_range"
# CONF_ACCELERATION_ODR = "acceleration_odr"

DEPENDENCIES = ["i2c"]

qmi8658_ns = cg.esphome_ns.namespace("qmi8658")

QMI8658Component = qmi8658_ns.class_(
    "QMI8658Component", cg.PollingComponent, i2c.I2CDevice
)

# QMI8658AccRange = qmi8658_ns.enum("QMI8658AccRange")
# QMI8658AccRanges = {
#     0: QMI8658AccRange.ACC_RANGE_2G,
#     1: QMI8658AccRange.ACC_RANGE_4G,
#     2: QMI8658AccRange.ACC_RANGE_8G,
#     3: QMI8658AccRange.ACC_RANGE_16G,
# }

# QMI8658AccODR = qmi8658_ns.enum("QMI8658AccOdr")
# QMI8658AccODRs = {
#     3: QMI8658AccODR.ACC_ODR_1000Hz,
#     4: QMI8658AccODR.ACC_ODR_500Hz,
#     5: QMI8658AccODR.ACC_ODR_125Hz,
#     6: QMI8658AccODR.ACC_ODR_62_5Hz,
#     7: QMI8658AccODR.ACC_ODR_31_25Hz,
#     12: QMI8658AccODR.ACC_ODR_LOWPOWER_128Hz,
#     13: QMI8658AccODR.ACC_ODR_LOWPOWER_21Hz,
#     14: QMI8658AccODR.ACC_ODR_LOWPOWER_11Hz,
#     15: QMI8658AccODR.ACC_ODR_LOWPOWER_3Hz,
# }


# # QMI8658Datarate = qmi8658_ns.enum("QMI8658Datarate")
# # QMI8658Datarates = {
# #     10: QMI8658Datarate.QMI8658_DATARATE_10_HZ,
# #     50: QMI8658Datarate.QMI8658_DATARATE_50_HZ,
# #     100: QMI8658Datarate.QMI8658_DATARATE_100_HZ,
# #     200: QMI8658Datarate.QMI8658_DATARATE_200_HZ,
# # }


def validate_enum(enum_values, units=None, int=True):
    _units = []
    if units is not None:
        _units = units if isinstance(units, list) else [units]
        _units = [str(x) for x in _units]
    enum_bound = cv.enum(enum_values, int=int)
    def validate_enum_bound(value):
        value = cv.string(value)
        for unit in _units:
            if value.endswith(unit):
                value = value[: -len(unit)]
                break
        return enum_bound(value)
    return validate_enum_bound


acceleration_schema = sensor.sensor_schema(
    unit_of_measurement=UNIT_G,
    icon=ICON_BRIEFCASE_DOWNLOAD,
    accuracy_decimals=1,
    state_class=STATE_CLASS_MEASUREMENT,
)
gyro_schema = sensor.sensor_schema(
    unit_of_measurement=UNIT_DEGREE_PER_SECOND,
    icon=ICON_SCREEN_ROTATION,
    accuracy_decimals=1,
    state_class=STATE_CLASS_MEASUREMENT,
)
temperature_schema = sensor.sensor_schema(
    unit_of_measurement=UNIT_CELSIUS,
    accuracy_decimals=1,
    device_class=DEVICE_CLASS_TEMPERATURE,
    state_class=STATE_CLASS_MEASUREMENT,
)


CONFIG_SCHEMA = cv.All(
    cv.only_with_arduino,
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(QMI8658Component),
            cv.Optional(CONF_ADDRESS): cv.i2c_address,
            cv.Exclusive(CONF_INTERRUPT_PIN_1, CONF_INTEERUPT_GROUP): pins.gpio_input_pin_schema,
            cv.Exclusive(CONF_INTERRUPT_PIN_2, CONF_INTEERUPT_GROUP): pins.gpio_input_pin_schema,
            # cv.Optional(CONF_ACCELERATION_RANGE, default="4G"): validate_enum(
            #     QMI8658AccRanges, units=["g", "G"]
            # ),
            # cv.Optional(CONF_ACCELERATION_ODR, default="100Hz"): validate_enum(
            #     QMI8658AccODRs, units=["Hz", "hz"]
            # ),
            # # cv.Optional(CONF_RANGE, default="200µT"): validate_enum(
            # #     QMI8658_RANGES, units=["uT", "µT"]
            # # ),
            cv.Optional(CONF_ACCELERATION_X): acceleration_schema,
            cv.Optional(CONF_ACCELERATION_Y): acceleration_schema,
            cv.Optional(CONF_ACCELERATION_Z): acceleration_schema,
            cv.Optional(CONF_GYROSCOPE_X): gyro_schema,
            cv.Optional(CONF_GYROSCOPE_Y): gyro_schema,
            cv.Optional(CONF_GYROSCOPE_Z): gyro_schema,
            cv.Optional(CONF_TEMPERATURE): temperature_schema,
        }
    )
    .extend(cv.polling_component_schema("5s"))
    .extend(i2c.i2c_device_schema(0x6B))
)


# def auto_data_rate(config):
#     interval_sec = config[CONF_UPDATE_INTERVAL].total_milliseconds / 1000
#     interval_hz = 1.0 / interval_sec
#     for datarate in sorted(QMI8658Datarates.keys()):
#         if float(datarate) >= interval_hz:
#             return QMI8658Datarates[datarate]
#     return QMI8658Datarates[200]


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    if CONF_INTERRUPT_PIN_1 in config:
        interrupt_pin = await cg.gpio_pin_expression(config[CONF_INTERRUPT_PIN_1])
        cg.add(var.set_interrupt_pin_1(interrupt_pin))
    if CONF_INTERRUPT_PIN_2 in config:
        interrupt_pin = await cg.gpio_pin_expression(config[CONF_INTERRUPT_PIN_2])
        cg.add(var.set_interrupt_pin_2(interrupt_pin))

    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    # cg.add(var.set_oversampling(config[CONF_OVERSAMPLING]))
    # cg.add(var.set_datarate(auto_data_rate(config)))
    # cg.add(var.set_range(config[CONF_RANGE]))
    if CONF_ACCELERATION_X in config:
        sens = await sensor.new_sensor(config[CONF_ACCELERATION_X])
        cg.add(var.set_accel_x_sensor(sens))
    if CONF_ACCELERATION_Y in config:
        sens = await sensor.new_sensor(config[CONF_ACCELERATION_Y])
        cg.add(var.set_accel_y_sensor(sens))
    if CONF_ACCELERATION_Z in config:
        sens = await sensor.new_sensor(config[CONF_ACCELERATION_Z])
        cg.add(var.set_accel_z_sensor(sens))
    if CONF_GYROSCOPE_X in config:
        sens = await sensor.new_sensor(config[CONF_GYROSCOPE_X])
        cg.add(var.set_gyro_x_sensor(sens))
    if CONF_GYROSCOPE_Y in config:
        sens = await sensor.new_sensor(config[CONF_GYROSCOPE_Y])
        cg.add(var.set_gyro_y_sensor(sens))
    if CONF_GYROSCOPE_Z in config:
        sens = await sensor.new_sensor(config[CONF_GYROSCOPE_Z])
        cg.add(var.set_gyro_z_sensor(sens))
    if CONF_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATURE])
        cg.add(var.set_temperature_sensor(sens))
