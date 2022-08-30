### Thư viện sử dụng
```
ottowinter/ESPAsyncWebServer-esphome@^2.1.0
```
- [Tham khảo](https://techtutorialsx.com/2019/04/26/esp32-arduino-http-server-serving-image-as-attachment/)

### Note
- Đoạn code này sử dụng **ESPAsyncWebServer** để tạo một webserver.
- Khi truy cập vào đường dẫn: `http://192.168.4.1/download`.
- Thì mình có thể tải ảnh đã lưu trong *SPIFFS* của ESP32-Uno.
- Đã test thành công.

### Wifi Access Point
```
ssid: WifiAP_Image
pass: 123456789
```