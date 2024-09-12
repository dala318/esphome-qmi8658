#include "qmi8658.h"

#include "esphome/core/log.h"

// #include <Arduino.h>
// #include <Wire.h>
#include "SensorQMI8658.hpp"                    
#include <cmath>

// #define I2C_SDA       11
// #define I2C_SCL       12

namespace esphome {
namespace qmi8658 {

static const char *TAG = "qmi8658";

void QMI8658Component::setup() {
    qmi8658 = SensorQMI8658();

    // Wire.begin(I2C_SDA, I2C_SCL);
    //Using WIRE !!
    // if (!qmi8658.begin(Wire, QMI8658_L_SLAVE_ADDRESS, I2C_SDA, I2C_SCL)) {
    if (!qmi8658.begin(QMI8658_L_SLAVE_ADDRESS)) {
        ESP_LOGE(TAG, "Failed to find QMI8658 - check your wiring!");
        while (1) {
            delay(1000);
        }
    }
    ESP_LOGI(TAG, "Device ID: %x",qmi8658.getChipID());
    
    qmi8658.configAccelerometer(
        SensorQMI8658::ACC_RANGE_4G,      // ACC_RANGE_2G / ACC_RANGE_4G / ACC_RANGE_8G / ACC_RANGE_16G
        SensorQMI8658::ACC_ODR_1000Hz,    // ACC_ODR_1000H / ACC_ODR_500Hz / ACC_ODR_250Hz / ACC_ODR_125Hz / ACC_ODR_62_5Hz / ACC_ODR_31_25Hz / ACC_ODR_LOWPOWER_128Hz / ACC_ODR_LOWPOWER_21Hz / ACC_ODR_LOWPOWER_11Hz / ACC_ODR_LOWPOWER_3Hz    
        SensorQMI8658::LPF_MODE_0,        //LPF_MODE_0 (2.66% of ODR) / LPF_MODE_1 (3.63% of ODR) / LPF_MODE_2 (5.39% of ODR) / LPF_MODE_3 (13.37% of ODR)
        true);                            // selfTest enable
    qmi8658.configGyroscope(
        SensorQMI8658::GYR_RANGE_64DPS,   // GYR_RANGE_16DPS / GYR_RANGE_32DPS / GYR_RANGE_64DPS / GYR_RANGE_128DPS / GYR_RANGE_256DPS / GYR_RANGE_512DPS / GYR_RANGE_1024DPS
        SensorQMI8658::GYR_ODR_896_8Hz,   // GYR_ODR_7174_4Hz / GYR_ODR_3587_2Hz / GYR_ODR_1793_6Hz / GYR_ODR_896_8Hz / GYR_ODR_448_4Hz / GYR_ODR_224_2Hz / GYR_ODR_112_1Hz / GYR_ODR_56_05Hz / GYR_ODR_28_025H
        SensorQMI8658::LPF_MODE_3,        // LPF_MODE_0 (2.66% of ODR) / LPF_MODE_1 (3.63% of ODR) / LPF_MODE_2 (5.39% of ODR) / LPF_MODE_3 (13.37% of ODR)
        true);                            // selfTest enable

    // In 6DOF mode (accelerometer and gyroscope are both enabled),
    // the output data rate is derived from the nature frequency of gyroscope
    qmi8658.enableGyroscope();
    qmi8658.enableAccelerometer();
    
    // qmi8658.dumpCtrlRegister();               // printf register configuration information
    ESP_LOGI(TAG, "Read data now...");
}

void QMI8658Component::dump_config() {
  ESP_LOGCONFIG(TAG, "QMI8658:");
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with QMI8658 failed!");
  }
  LOG_UPDATE_INTERVAL(this);
  LOG_SENSOR("  ", "Acceleration X", this->accel_x_sensor_);
  LOG_SENSOR("  ", "Acceleration Y", this->accel_y_sensor_);
  LOG_SENSOR("  ", "Acceleration Z", this->accel_z_sensor_);
  LOG_SENSOR("  ", "Gyro X", this->gyro_x_sensor_);
  LOG_SENSOR("  ", "Gyro Y", this->gyro_y_sensor_);
  LOG_SENSOR("  ", "Gyro Z", this->gyro_z_sensor_);
  LOG_SENSOR("  ", "Temperature", this->temperature_sensor_);
}

void QMI8658Component::update() {
    // float temperature = qmi8658.getTemperature_C();
    // if (this->temperature_sensor_ != nullptr)
    //     temperature_sensor_->publish_state(temperature);

    if (qmi8658.getDataReady()) {
        float temperature = qmi8658.getTemperature_C();
        ESP_LOGD(TAG, ">      %lu   %.2f ℃", qmi8658.getTimestamp(), temperature);
        if (this->temperature_sensor_ != nullptr)
            temperature_sensor_->publish_state(temperature);

        if (qmi8658.getAccelerometer(accel_data.x, accel_data.y, accel_data.z)) {
            ESP_LOGD(TAG, "ACCEL:  %f  %f  %f", accel_data.x, accel_data.y, accel_data.z);
            if (this->accel_x_sensor_ != nullptr)
                accel_x_sensor_->publish_state(accel_data.x);
            if (this->accel_y_sensor_ != nullptr)
                accel_y_sensor_->publish_state(accel_data.y);
            if (this->accel_z_sensor_ != nullptr)
                accel_z_sensor_->publish_state(accel_data.z);
        }

        if (qmi8658.getGyroscope(gyro_data.x, gyro_data.y, gyro_data.z)) {
            ESP_LOGD(TAG, "GYRO:  %f  %f  %f", gyro_data.x, gyro_data.y, gyro_data.z);
            if (this->gyro_x_sensor_ != nullptr)
                this->gyro_x_sensor_->publish_state(gyro_data.x);
            if (this->gyro_y_sensor_ != nullptr)
                this->gyro_y_sensor_->publish_state(gyro_data.y);
            if (this->gyro_z_sensor_ != nullptr)
                this->gyro_z_sensor_->publish_state(gyro_data.z);
        }

        // ESP_LOGD(TAG, ">      %lu   %.2f ℃", qmi8658.getTimestamp(), qmi8658.getTemperature_C());
    }
    else{
        ESP_LOGE(TAG, "Data not ready");
    }
}

float QMI8658Component::get_setup_priority() const { return setup_priority::DATA; }

}  // namespace qmi8658
}  // namespace esphome
