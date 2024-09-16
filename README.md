# esphome-qmi8658

Using [SensorLib](https://github.com/lewisxhe/SensorLib) by @lewisxhe

Add the following to your ESPHome config

(Check out the attached `led8x8-qmi8658.yaml` for a more complete config using the [ESP32-S3-Matrix](https://www.waveshare.com/wiki/ESP32-S3-Matrix) )

```yaml
esphome:
  platformio_options:
    board_build.flash_mode: dio
  libraries:
    - "Wire"
    - "SPI"
    - "SensorLib"

external_components:
- source:
      source: github://dala318/esphome-qmi8658

i2c:
  sda: GPIO11
  scl: GPIO12

sensor:
  - platform: qmi8658
    address: 0x6B
    # interrupt_pin_1: GPIO10    # Don't work very well for me, use IRQ2 below
    interrupt_pin_2: GPIO13
    acceleration_range: 4G       # Optional
    acceleration_odr: 100hz      # Optional
    acceleration_lpf_mode: "0"   # Optional
    gyroscope_range: "64DPS"     # Optional
    gyroscope_odr: "896.8HZ"     # Optional
    gyroscope_lpf_mode: "3"      # Optional
    acceleration_x:
      name: "QMI8658 Acceleration X"
    acceleration_y:
      name: "QMI8658 Acceleration Y"
    acceleration_z:
      name: "QMI8658 Acceleration Z"
    gyroscope_x:
      name: "QMI8658 Gyro X"
    gyroscope_y:
      name: "QMI8658 Gyro Y"
    gyroscope_z:
      name: "QMI8658 Gyro Z"
    temperature:
      name: "QMI8658 Temperature"
      filters:
        - offset: 34.0
    update_interval: 5s

```
