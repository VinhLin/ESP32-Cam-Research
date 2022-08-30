# ESP32-Cam-Research
Test and Research ESP32-Cam

## ESP32-Cam with PlatformIO
```
 https://docs.platformio.org/en/latest/boards/espressif32/esp32cam.html0
 https://www.survivingwithandroid.com/esp32-cam-platformio-video-streaming-face-recognition/
 https://github.com/espressif/arduino-esp32/tree/master/libraries/ESP32/examples/Camera/CameraWebServer
```

## [Library ESP32-Cam](https://registry.platformio.org/search?q=esp32-cam)

-----------------------------------------------------------------------------------------------
### VC0706-Serial-Camera
```
https://learn.adafruit.com/ttl-serial-camera/arduino-usage
https://registry.platformio.org/libraries/adafruit/Adafruit%20VC0706%20Serial%20Camera%20Library
https://github.com/adafruit/Adafruit-VC0706-Serial-Camera-Library
```

----------------------------------------------------------------------------------------------
## Giải thích cơ cấu hoạt động
- **ESP32-Cam** là một WebServer và đóng vai trò là một **wifi access point** luôn trong trạng thái hoạt động.
```
ssid: ESP32-Cam-Wifi
pass: 123456789
```
- Khi truy cập vào wifi AP này thì mình có thể xem **live video**.
- Mình có một con ESP32-Uno để capture ảnh từ ESP32-Cam. 
> Mục tiêu là con ESP32-Uno này sẽ được gắn với các cảm biến khác, xử lý thêm nhiều tác vụ khác.
> Giả sử có một trigger nào đó xảy ra, mình sẽ chủ động capture ảnh lại và lưu vào SPIFFS của ESP32-Uno.

### Kết quả test
- ESP32-Uno đóng vai trò là một *client*, kết nối với wifi AP của ESP32-Cam.
- Khi cần capture ảnh thì sẽ trỏ đến `http://192.168.4.1/640x480.jpg`
- Và lưu dữ liệu ảnh này vào trong SPIFFS.
- **Đã test thành công**.
