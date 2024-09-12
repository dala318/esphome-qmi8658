#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

#include "SensorQMI8658.hpp"
// #include <Arduino.h>
// #include <Wire.h>

namespace esphome {
namespace qmi8658 {

class QMI8658Component : public PollingComponent, public i2c::I2CDevice {
 public:
  void setup() override;
  void dump_config() override;
  void update() override;

  float get_setup_priority() const override;

  void set_accel_x_sensor(sensor::Sensor *accel_x_sensor) { accel_x_sensor_ = accel_x_sensor; }
  void set_accel_y_sensor(sensor::Sensor *accel_y_sensor) { accel_y_sensor_ = accel_y_sensor; }
  void set_accel_z_sensor(sensor::Sensor *accel_z_sensor) { accel_z_sensor_ = accel_z_sensor; }
  void set_gyro_x_sensor(sensor::Sensor *gyro_x_sensor) { gyro_x_sensor_ = gyro_x_sensor; }
  void set_gyro_y_sensor(sensor::Sensor *gyro_y_sensor) { gyro_y_sensor_ = gyro_y_sensor; }
  void set_gyro_z_sensor(sensor::Sensor *gyro_z_sensor) { gyro_z_sensor_ = gyro_z_sensor; }
  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }

 protected:
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


//  public:
//   QMI8658() : PollingComponent(200) { }

//   void setup() override {
//     Wire.begin(I2C_SDA, I2C_SCL);
//     //Using WIRE !!
//     if (!QMI.begin(Wire, QMI8658_L_SLAVE_ADDRESS, I2C_SDA, I2C_SCL)) {
//         // printf("Failed to find QMI8658 - check your wiring!\r\n");
//         ESP_LOGE(TAG, "Failed to find QMI8658 - check your wiring!");
//         while (1) {
//             delay(1000);
//         }
//     }
//     // printf("Device ID: %x\r\n",QMI.getChipID());    // Get chip id
//     ESP_LOGI(TAG, "Device ID: %x",QMI.getChipID());
    

//     QMI.configAccelerometer(
//         SensorQMI8658::ACC_RANGE_4G,      // ACC_RANGE_2G / ACC_RANGE_4G / ACC_RANGE_8G / ACC_RANGE_16G
//         SensorQMI8658::ACC_ODR_1000Hz,    // ACC_ODR_1000H / ACC_ODR_500Hz / ACC_ODR_250Hz / ACC_ODR_125Hz / ACC_ODR_62_5Hz / ACC_ODR_31_25Hz / ACC_ODR_LOWPOWER_128Hz / ACC_ODR_LOWPOWER_21Hz / ACC_ODR_LOWPOWER_11Hz / ACC_ODR_LOWPOWER_3Hz    
//         SensorQMI8658::LPF_MODE_0,        //LPF_MODE_0 (2.66% of ODR) / LPF_MODE_1 (3.63% of ODR) / LPF_MODE_2 (5.39% of ODR) / LPF_MODE_3 (13.37% of ODR)
//         true);                            // selfTest enable
//     QMI.configGyroscope(
//         SensorQMI8658::GYR_RANGE_64DPS,   // GYR_RANGE_16DPS / GYR_RANGE_32DPS / GYR_RANGE_64DPS / GYR_RANGE_128DPS / GYR_RANGE_256DPS / GYR_RANGE_512DPS / GYR_RANGE_1024DPS
//         SensorQMI8658::GYR_ODR_896_8Hz,   // GYR_ODR_7174_4Hz / GYR_ODR_3587_2Hz / GYR_ODR_1793_6Hz / GYR_ODR_896_8Hz / GYR_ODR_448_4Hz / GYR_ODR_224_2Hz / GYR_ODR_112_1Hz / GYR_ODR_56_05Hz / GYR_ODR_28_025H
//         SensorQMI8658::LPF_MODE_3,        // LPF_MODE_0 (2.66% of ODR) / LPF_MODE_1 (3.63% of ODR) / LPF_MODE_2 (5.39% of ODR) / LPF_MODE_3 (13.37% of ODR)
//         true);                            // selfTest enable


//     // In 6DOF mode (accelerometer and gyroscope are both enabled),
//     // the output data rate is derived from the nature frequency of gyroscope
//     QMI.enableGyroscope();
//     QMI.enableAccelerometer();
    
//     // QMI.dumpCtrlRegister();               // printf register configuration information

//     // printf("Read data now...\r\n");
//     ESP_LOGI(TAG, "Read data now...");
//   }

//   void update() override {
//     float temperature = QMI.getTemperature_C();
//     temperature_sensor->publish_state(temperature);

//     // int humidity = QMI.humidity();
//     // humidity_sensor->publish_state(humidity);

//     // int vpd = QMI.vpd();
//     // vpd_sensor->publish_state(vpd);

//     // int dew_point = QMI.dew_point();
//     // dew_point_sensor->publish_state(dew_point);



//     if (QMI.getDataReady()) {
//         if (QMI.getAccelerometer(Accel.x, Accel.y, Accel.z)) {
//             // printf("ACCEL:  %f  %f  %f\r\n",Accel.x,Accel.y,Accel.z);
//             ESP_LOGI(TAG, "ACCEL:  %f  %f  %f",Accel.x,Accel.y,Accel.z);
//         }

//         if (QMI.getGyroscope(Gyro.x, Gyro.y, Gyro.z)) {
//             // printf("GYRO:  %f  %f  %f\r\n",Gyro.x,Gyro.y,Gyro.z);
//             ESP_LOGI(TAG, "GYRO:  %f  %f  %f",Gyro.x,Gyro.y,Gyro.z);
//         }
//         // printf("\t\t>      %lu   %.2f ℃\n", QMI.getTimestamp(), QMI.getTemperature_C());
//         ESP_LOGI(TAG, ">      %lu   %.2f ℃", QMI.getTimestamp(), QMI.getTemperature_C());
//         // printf("\r\n");
//     }
//   }

};

}  // namespace qmi8658
}  // namespace esphome
