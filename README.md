# ESP32-Cam-Research
Test and Research ESP32-Cam

## ESP32-Cam with PlatformIO
```
 https://docs.platformio.org/en/latest/boards/espressif32/esp32cam.html0
 https://www.survivingwithandroid.com/esp32-cam-platformio-video-streaming-face-recognition/
 https://github.com/espressif/arduino-esp32/tree/master/libraries/ESP32/examples/Camera/CameraWebServer
```

## [Library ESP32-Cam](https://registry.platformio.org/search?q=esp32-cam)

------------------------------------------------------------------------------------------------
## Note
- Khi dùng **ESP32-Cam** dùng các library cho **PlatformIO**.
- Lúc build ra thì nó bị lỗi.
- Khi search thì nguyên nhân là phiên bản **arduino-esp32** thấp, cần nâng cấp.

### [Tham khảo cách khắc phục](https://community.platformio.org/t/how-to-use-the-newest-version-of-espressif-arduino-esp32/24649)
- Mình thử như sau: Trước hết là update **Platform của ESP32** lên **version 5.0.0**
- Vô **Pio HOME** -> **Platforms** -> **Updates** -> Update Espressif 32.
- Sau khi trong project ESP32-Cam, mình sửa file **platformio.ini** như sau:
```
[env:esp32cam]
platform = https://github.com/platformio/platform-espressif32.git#feature/arduino-idf-master
board = esp32cam
framework = arduino
platform_packages =
   framework-arduinoespressif32 @ https://github.com/espressif/arduino-esp32.git#2.0.1
lib_deps = 
    yoursunny/esp32cam@^0.0.20220612
    ottowinter/ESPAsyncWebServer-esphome@^2.1.0
```







