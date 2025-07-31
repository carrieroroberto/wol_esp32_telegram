# 🔌 Wake-on-LAN Telegram Bot for ESP32

<div align="center">
  <img src="https://img.shields.io/badge/platform-ESP32-blue.svg" />
  <img src="https://img.shields.io/badge/telegram-bot-blue?logo=telegram" />
  <img src="https://img.shields.io/badge/status-online-brightgreen" />
</div>

---

## ❓ What is this?

This is a lightweight and secure project for your **ESP32** that lets you control your PC (or other WoL-compatible devices) remotely using a **Telegram bot**.

With simple commands, you can:
- ✅ Check if your PC is online
- 🚀 Wake it up with a Magic Packet
- 📊 Monitor the bot’s status
- 💬 Interact from anywhere using Telegram

---

## 🎯 Features

- 🔒 Works with authorized users only
- 📡 Sends Magic Packet via Wake-on-LAN (UDP)
- 🌐 Supports HTTPS with proper time sync
- 💬 Clean and interactive Telegram messages
- 💻 Checks device availability using ICMP Ping
- 🛠️ Easy to configure via `config.h`

---

## 🚀 How to Use

### 1. ⚙️ Setup

- Clone the repository
- Open with [Arduino IDE](https://www.arduino.cc/en/software)
- Install the required libraries (see below)
- Modify `config.h` with your values:
  ```cpp
  #define WIFI_SSID      "YourWiFiSSID"
  #define WIFI_PASS      "YourWiFiPassword"
  #define BOT_TOKEN      "123456:ABC-YOUR-BOT-TOKEN"
  #define ALLOWED_ID     123456789  // Your Telegram user ID
  #define PC_IP          IPAddress(192, 168, 1, 100)
  #define MAC_ADDR       {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}
