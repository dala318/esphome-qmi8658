#include "esphome.h"
// #include "esphome/components/i2c/i2c.h"

static const char *TAG = "qmi8658";

// An attempt to use native ESP_LOG in SensorQMI8658
// #ifndef log_e
// #define log_e(fmt, ...)     ESP_LOGE(TAG, fmt, ##__VA_ARGS__)
// #endif
// #ifndef log_i
// #define log_i(fmt, ...)     ESP_LOGI(TAG, fmt, ##__VA_ARGS__)
// #endif
// #ifndef log_d
// #define log_d(fmt, ...)     ESP_LOGD(TAG, fmt, ##__VA_ARGS__)
// #endif

#include "SensorQMI8658.hpp"

#define I2C_SDA                     (11)
#define I2C_SCL                     (12)

// Some attempt to pass read/write handles to qmi8658.begin()
// static int read_i2c(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint8_t len)
//   {return 0;}
// static int write_i2c(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint8_t len)
//   {return 0;}


// class QMI8658 : public PollingComponent, public i2c::I2CDevice {
class QMI8658 : public PollingComponent {
    public:
        SensorQMI8658 qmi8658;
        IMUdata accel_data;
        IMUdata gyro_data;

        Sensor *temperature_sensor = new Sensor();
        Sensor *accel_x_sensor = new Sensor();

        // To prevent self-heating, the datasheet warns to do
        // maximum two measurements per second at 12-bit accuracy.
        // Stay on the safe side by polling every five seconds,
        // because the calls to vpd() and dew_point() also measure
        // the temperature.
        QMI8658() : PollingComponent(1000) { }

        void setup() override {
            ESP_LOGI(TAG, "Setup component");
            // Wire.begin();
            // if(!qmi8658.begin(QMI8658_L_SLAVE_ADDRESS)) {
            // if(!qmi8658.begin(QMI8658_L_SLAVE_ADDRESS, read_i2c, write_i2c)) {
            // if(!qmi8658.begin(Wire, QMI8658_L_SLAVE_ADDRESS)) {
            if(!qmi8658.begin(Wire, QMI8658_L_SLAVE_ADDRESS, I2C_SDA, I2C_SCL)) {
                ESP_LOGE(TAG, "Sensor is not connected");
            }
            ESP_LOGI(TAG, "Device ID: %x", qmi8658.getChipID());
        
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
        }

    void update() override {
        float temperature = qmi8658.getTemperature_C();
        temperature_sensor->publish_state(temperature);

        if (qmi8658.getDataReady()) {
            if (qmi8658.getAccelerometer(accel_data.x, accel_data.y, accel_data.z)) {
                ESP_LOGD(TAG, "ACCEL:  %f  %f  %f", accel_data.x, accel_data.y, accel_data.z);
                accel_x_sensor->publish_state(accel_data.x);
            }

            if (qmi8658.getGyroscope(gyro_data.x, gyro_data.y, gyro_data.z)) {
                ESP_LOGD(TAG, "GYRO:  %f  %f  %f", gyro_data.x, gyro_data.y, gyro_data.z);
            }
            ESP_LOGD(TAG, "TIME:  %lu   %.2f ℃", qmi8658.getTimestamp(), temperature);
        }
        else{
            ESP_LOGE(TAG, "Data not ready");
        }
    }
};