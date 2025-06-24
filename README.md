# 🤖 Telegram Wake-on-LAN Bot with ESP32

This project allows you to turn on a device in your local network using **Wake-on-LAN** via a **Telegram bot**, powered by an **ESP32** board connected to Wi-Fi.

---

## 📦 Features

- `/wol` command to send the Magic Packet
- `/ping` command to check bot responsiveness
- `/start` command to display help message
- User authentication via `chat_id`
- Dynamic Wi-Fi connection management
- Secure HTTPS communication with Telegram API using root certificate

---

## 🔧 Hardware Requirements

- **ESP32** development board
- Wi-Fi network
- A PC configured for **Wake-on-LAN** (BIOS and OS settings enabled)

---

## 📚 Required Libraries

Install these libraries via the **Arduino Library Manager** or manually:

- [`WiFi`](https://www.arduino.cc/en/Reference/WiFi)
- `WiFiMulti` *(part of the ESP32 core)*
- `WiFiClientSecure`
- `WiFiUDP`
- [`WakeOnLan`](https://github.com/a7md0/WakeOnLan)
- [`UniversalTelegramBot`](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot)
- [`ArduinoJson`](https://arduinojson.org/) (version 6 or higher)

> ✅ Make sure to select **ESP32 Dev Module** under **Tools > Board** in the Arduino IDE.

---

## 🔐 Configuration

Ecco la frase corretta in inglese:

Modify the `credentials.h` file appropriately, or create a new file with this name in the project root with the following content:

```cpp
// credentials.h

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASS "your_wifi_password"

#define BOT_TOKEN "123456789:ABCdefGHIjklMNOpqrSTUvwxYZ" // Get this from @BotFather
#define ALLOWED_ID "123456789"  // Telegram chat_id of the authorized user

#define MAC_ADDR {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}  // MAC address of the target PC
