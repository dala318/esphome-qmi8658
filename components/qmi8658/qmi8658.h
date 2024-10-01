#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

#include "SensorQMI8658.hpp"

namespace esphome {
namespace qmi8658 {

class QMI8658Component : public PollingComponent, public i2c::I2CDevice {
    public:
        void setup() override;
        void dump_config() override;
        void update() override;
        void loop() override;

        float get_setup_priority() const override;

        void set_sda_pin(uint8_t sda_pin) { sda_pin_ = sda_pin; }
        void set_scl_pin(uint8_t scl_pin) { scl_pin_ = scl_pin; }

        void set_accel_range(SensorQMI8658::AccelRange accel_range) { accel_range_ = accel_range; }
        void set_accel_odr(SensorQMI8658::AccelODR accel_odr) { accel_odr_ = accel_odr; }
        void set_accel_lpf_mode(SensorQMI8658::LpfMode accel_lpf_mode) { accel_lpf_mode_ = accel_lpf_mode; }

        void set_gyro_range(SensorQMI8658::GyroRange gyro_range) { gyro_range_ = gyro_range; }
        void set_gyro_odr(SensorQMI8658::GyroODR gyro_odr) { gyro_odr_ = gyro_odr; }
        void set_gyro_lpf_mode(SensorQMI8658::LpfMode gyro_lpf_mode) { gyro_lpf_mode_ = gyro_lpf_mode; }

        void set_interrupt_pin_1(GPIOPin *interrupt_pin) { interrupt_pin_1_ = interrupt_pin; }
        void set_interrupt_pin_2(GPIOPin *interrupt_pin) { interrupt_pin_2_ = interrupt_pin; }
        void set_accel_x_sensor(sensor::Sensor *accel_x_sensor) { accel_x_sensor_ = accel_x_sensor; }
        void set_accel_y_sensor(sensor::Sensor *accel_y_sensor) { accel_y_sensor_ = accel_y_sensor; }
        void set_accel_z_sensor(sensor::Sensor *accel_z_sensor) { accel_z_sensor_ = accel_z_sensor; }
        void set_gyro_x_sensor(sensor::Sensor *gyro_x_sensor) { gyro_x_sensor_ = gyro_x_sensor; }
        void set_gyro_y_sensor(sensor::Sensor *gyro_y_sensor) { gyro_y_sensor_ = gyro_y_sensor; }
        void set_gyro_z_sensor(sensor::Sensor *gyro_z_sensor) { gyro_z_sensor_ = gyro_z_sensor; }
        void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }

    protected:
        uint8_t sda_pin_;
        uint8_t scl_pin_;

        SensorQMI8658::AccelRange accel_range_;
        SensorQMI8658::AccelODR accel_odr_;
        SensorQMI8658::LpfMode accel_lpf_mode_;

        SensorQMI8658::GyroRange gyro_range_;
        SensorQMI8658::GyroODR gyro_odr_;
        SensorQMI8658::LpfMode gyro_lpf_mode_;

        GPIOPin *interrupt_pin_1_{nullptr};
        GPIOPin *interrupt_pin_2_{nullptr};
        sensor::Sensor *accel_x_sensor_{nullptr};
        sensor::Sensor *accel_y_sensor_{nullptr};
        sensor::Sensor *accel_z_sensor_{nullptr};
        sensor::Sensor *gyro_x_sensor_{nullptr};
        sensor::Sensor *gyro_y_sensor_{nullptr};
        sensor::Sensor *gyro_z_sensor_{nullptr};
        sensor::Sensor *temperature_sensor_{nullptr};

        SensorQMI8658 qmi8658;
        IMUdata accel_data;
        IMUdata gyro_data;
};

}  // namespace qmi8658
}  // namespace esphome
