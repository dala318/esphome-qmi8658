#include "qmi8658.h"

#include "esphome/core/log.h"

// #include <Arduino.h>
// #include <Wire.h>
#include "SensorQMI8658.hpp"                    
#include <cmath>

namespace esphome {
namespace qmi8658 {

static const char *TAG = "qmi8658";

void QMI8658Component::setup() {
    // uint8_t sda_pin = this->bus_->sda_pin_;
    // uint8_t scl_pin = this->bus_->scl_pin_;
    // ESP_LOGI(TAG, "SDA: %u   SCL: %u", sda_pin, scl_pin);
 
    // Wire.begin(I2C_SDA, I2C_SCL);
    // if (!qmi8658.begin(Wire, QMI8658_L_SLAVE_ADDRESS)) {
    if (!qmi8658.begin(Wire, QMI8658_L_SLAVE_ADDRESS, this->sda_pin_, this->scl_pin_)) {
        ESP_LOGE(TAG, "Failed to find QMI8658 - check your wiring!");
    }
    ESP_LOGI(TAG, "Device ID: %x",qmi8658.getChipID());
    
    qmi8658.configAccelerometer(
        this->accel_range_,
        this->accel_odr_,
        this->accel_lpf_mode_,
        true);
    qmi8658.configGyroscope(
        this->gyro_range_,
        this->gyro_odr_,
        this->gyro_lpf_mode_,
        true);

    if (this->interrupt_pin_1_ != nullptr)  // Don't seem to work very well? Use interrupt 2
    {
        this->interrupt_pin_1_->setup();
        qmi8658.enableINT(SensorQMI8658::IntPin1);
        qmi8658.enableDataReadyINT();
        ESP_LOGI(TAG, "Interrupt 1 enabled");
    }
    if (this->interrupt_pin_2_ != nullptr)
    {
        this->interrupt_pin_2_->setup();
        qmi8658.enableINT(SensorQMI8658::IntPin2);
        qmi8658.enableDataReadyINT();
        ESP_LOGI(TAG, "Interrupt 2 enabled");
    }

    // TODO: Disable polling when using interrupt?
    // if (this->interrupt_pin_1_ != nullptr || this->interrupt_pin_2_ != nullptr)
    // {}

    if (this->accel_x_sensor_ != nullptr || this->accel_y_sensor_ != nullptr || this->accel_z_sensor_ != nullptr)
    {
        ESP_LOGI(TAG, "Enabling Accelerometer");
        qmi8658.enableAccelerometer();
    }

    if (this->gyro_x_sensor_ != nullptr || this->gyro_y_sensor_ != nullptr || this->gyro_z_sensor_ != nullptr)
    {
        ESP_LOGI(TAG, "Enabling Gyroscope");
        qmi8658.enableGyroscope();
    }

    // TODO: Need to define the log_e log_i, log_d functions to use native ESPHome loggers
    // qmi8658.dumpCtrlRegister();               // printf register configuration information
}

void QMI8658Component::dump_config() {
    ESP_LOGCONFIG(TAG, "QMI8658:");
    LOG_I2C_DEVICE(this);
    if (this->is_failed()) {
        ESP_LOGE(TAG, "Communication with QMI8658 failed!");
    }
    LOG_UPDATE_INTERVAL(this);
    LOG_PIN("  Interrupt pin 1: ", this->interrupt_pin_1_);
    LOG_PIN("  Interrupt pin 2: ", this->interrupt_pin_2_);
    LOG_SENSOR("  ", "Acceleration X", this->accel_x_sensor_);
    LOG_SENSOR("  ", "Acceleration Y", this->accel_y_sensor_);
    LOG_SENSOR("  ", "Acceleration Z", this->accel_z_sensor_);
    LOG_SENSOR("  ", "Gyro X", this->gyro_x_sensor_);
    LOG_SENSOR("  ", "Gyro Y", this->gyro_y_sensor_);
    LOG_SENSOR("  ", "Gyro Z", this->gyro_z_sensor_);
    LOG_SENSOR("  ", "Temperature", this->temperature_sensor_);
}

void QMI8658Component::loop() {
    PollingComponent::loop();

    if (this->interrupt_pin_1_ != nullptr)
    {
        bool interrupt = this->interrupt_pin_1_->digital_read();
        if (interrupt)
            this->update();
    }
    if (this->interrupt_pin_2_ != nullptr)
    {
        bool interrupt = this->interrupt_pin_2_->digital_read();
        if (interrupt)
            this->update();
    }
}

void QMI8658Component::update() {
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
    }
    else{
        ESP_LOGE(TAG, "Data not ready");
    }
}

float QMI8658Component::get_setup_priority() const { return setup_priority::DATA; }

}  // namespace qmi8658
}  // namespace esphome
