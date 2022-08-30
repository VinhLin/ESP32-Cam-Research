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

--------------------------------------------------------------------------------
## [Library cho ESP32-Cam](https://github.com/yoursunny/esp32cam)
- Sử dụng **WifiCam** trong thư mục [examples WifiCam](https://github.com/yoursunny/esp32cam/tree/main/examples/WifiCam)
- [Sử dụng Flash trên ESP-Cam](https://dev.to/azure/esp32cam-flash-on-flash-off-4nk9)


## Test ESP-Cam
- Connect với Wifi của ESP-Cam:
```
login: ESP32-Cam-Wifi
pass: 123456789
```
- Truy cập vào trang web: `http://192.168.4.1/640x480.jpg` để lấy hình ảnh của **ESP32-Cam**











