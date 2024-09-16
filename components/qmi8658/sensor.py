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
    DEVICE_CLASS_TEMPERATURE,
    ICON_SCREEN_ROTATION,
    ICON_BRIEFCASE_DOWNLOAD,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
)
from esphome import pins

CONF_INTERRUPT_GROUP = "interrupt_group"
CONF_INTERRUPT_PIN_1 = CONF_INTERRUPT_PIN + "_1"
CONF_INTERRUPT_PIN_2 = CONF_INTERRUPT_PIN + "_2"

CONF_ACCELERATION_RANGE = "acceleration_range"
CONF_ACCELERATION_ODR = "acceleration_odr"
CONF_ACCELERATION_LPF_MODE = "acceleration_lpf_mode"
CONF_GYROSCOPE_RANGE = "gyroscope_range"
CONF_GYROSCOPE_ODR = "gyroscope_odr"
CONF_GYROSCOPE_LPF_MODE = "gyroscope_lpf_mode"

DEPENDENCIES = ["i2c"]

qmi8658_ns = cg.esphome_ns.namespace("qmi8658")


QMI8658Component = qmi8658_ns.class_(
    "QMI8658Component", cg.PollingComponent, i2c.I2CDevice
)

SensorQMI8658_ns = cg.esphome_ns.namespace("SensorQMI8658")

QMI8658AccelRange = SensorQMI8658_ns.enum("AccelRange")
QMI8658AccelRanges = {
    "2G":  QMI8658AccelRange.ACC_RANGE_2G,
    "4G":  QMI8658AccelRange.ACC_RANGE_4G,
    "8G":  QMI8658AccelRange.ACC_RANGE_8G,
    "16G": QMI8658AccelRange.ACC_RANGE_16G,
}

QMI8658AccelODR = SensorQMI8658_ns.enum("AccelOdr")
QMI8658AccelODRs = {
    "1000HZ":   QMI8658AccelODR.ACC_ODR_1000Hz,
    "500HZ":    QMI8658AccelODR.ACC_ODR_500Hz,
    "125HZ":    QMI8658AccelODR.ACC_ODR_125Hz,
    "62.5HZ":   QMI8658AccelODR.ACC_ODR_62_5Hz,
    "31.25HZ":  QMI8658AccelODR.ACC_ODR_31_25Hz,
    "LP_125HZ": QMI8658AccelODR.ACC_ODR_LOWPOWER_128Hz,
    "LP_21HZ":  QMI8658AccelODR.ACC_ODR_LOWPOWER_21Hz,
    "LP_11HZ":  QMI8658AccelODR.ACC_ODR_LOWPOWER_11Hz,
    "LP_3HZ":   QMI8658AccelODR.ACC_ODR_LOWPOWER_3Hz,
}

QMI8658GyroRange = SensorQMI8658_ns.enum("GyroRange")
QMI8658GyroRanges = {
    "16DPS":   QMI8658GyroRange.GYR_RANGE_16DPS,
    "32DPS":   QMI8658GyroRange.GYR_RANGE_32DPS,
    "64DPS":   QMI8658GyroRange.GYR_RANGE_64DPS,
    "128DPS":  QMI8658GyroRange.GYR_RANGE_128DPS,
    "256DPS":  QMI8658GyroRange.GYR_RANGE_256DPS,
    "512DPS":  QMI8658GyroRange.GYR_RANGE_512DPS,
    "1024DPS": QMI8658GyroRange.GYR_RANGE_1024DPS,
}

QMI8658GyroODR = SensorQMI8658_ns.enum("GyroOdr")
QMI8658GyroODRs = {
    "7174.4HZ": QMI8658GyroODR.GYR_ODR_7174_4Hz,
    "3587.2HZ": QMI8658GyroODR.GYR_ODR_3587_2Hz,
    "1793.6HZ": QMI8658GyroODR.GYR_ODR_1793_6Hz,
    "896.8HZ":  QMI8658GyroODR.GYR_ODR_896_8Hz,
    "448.4HZ":  QMI8658GyroODR.GYR_ODR_448_4Hz,
    "224.2HZ":  QMI8658GyroODR.GYR_ODR_224_2Hz,
    "112.1HZ":  QMI8658GyroODR.GYR_ODR_112_1Hz,
    "56.05HZ":  QMI8658GyroODR.GYR_ODR_56_05Hz,
    "28.025HZ": QMI8658GyroODR.GYR_ODR_28_025H,
}

QMI8658LpfMode = SensorQMI8658_ns.enum("LpfMode")
QMI8658LpfModes = {
    "0": QMI8658LpfMode.LPF_MODE_0,
    "1": QMI8658LpfMode.LPF_MODE_1,
    "2": QMI8658LpfMode.LPF_MODE_2,
    "3": QMI8658LpfMode.LPF_MODE_3,
}

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
            cv.Exclusive(CONF_INTERRUPT_PIN_1, CONF_INTERRUPT_GROUP): pins.gpio_input_pin_schema,
            cv.Exclusive(CONF_INTERRUPT_PIN_2, CONF_INTERRUPT_GROUP): pins.gpio_input_pin_schema,
            cv.Optional(CONF_ACCELERATION_RANGE, default="4G"): cv.one_of(
                *QMI8658AccelRanges, upper=True,
            ),
            cv.Optional(CONF_ACCELERATION_ODR, default="1000Hz"): cv.one_of(
                *QMI8658AccelODRs, upper=True,
            ),
            cv.Optional(CONF_ACCELERATION_LPF_MODE, default="0"): cv.one_of(
                *QMI8658LpfModes, upper=True,
            ),
            cv.Optional(CONF_GYROSCOPE_RANGE, default="64DPS"): cv.one_of(
                *QMI8658GyroRanges, upper=True,
            ),
            cv.Optional(CONF_GYROSCOPE_ODR, default="896.8HZ"): cv.one_of(
                *QMI8658GyroODRs, upper=True,
            ),
            cv.Optional(CONF_GYROSCOPE_LPF_MODE, default="3"): cv.one_of(
                *QMI8658LpfModes, upper=True,
            ),
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

    cg.add(var.set_accel_range(QMI8658AccelRanges[config[CONF_ACCELERATION_RANGE]]))
    cg.add(var.set_accel_odr(QMI8658AccelODRs[config[CONF_ACCELERATION_ODR]]))
    cg.add(var.set_accel_lpf_mode(QMI8658LpfModes[config[CONF_ACCELERATION_LPF_MODE]]))

    cg.add(var.set_gyro_range(QMI8658GyroRanges[config[CONF_GYROSCOPE_RANGE]]))
    cg.add(var.set_gyro_odr(QMI8658GyroODRs[config[CONF_GYROSCOPE_ODR]]))
    cg.add(var.set_gyro_lpf_mode(QMI8658LpfModes[config[CONF_GYROSCOPE_LPF_MODE]]))

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
