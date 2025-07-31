# 🚀 ESP32 Telegram Bot - Wake On LAN (WoL)

Welcome to the **ESP32 Telegram Bot** project! This bot lets you remotely wake up your PC using *Wake On LAN* technology, check its status, and interact via Telegram — all controlled by your ESP32 board.

---

## ⚙️ Features

- 🔌 **Wake On LAN**: Send a Magic Packet to power on your PC remotely  
- 📡 **Ping**: Check if your PC is currently online or offline  
- 📊 **Status**: View bot status, WiFi info, and uptime  
- 👋 **Start & Help commands**: Get a warm welcome and list of available commands  
- 🔒 Secure connection using TLS certificates  

---

## 🧰 What You Need

- ESP32 development board  
- PC with WoL support enabled and MAC address available  
- WiFi network credentials (SSID and password)  
- Telegram bot token (from [BotFather](https://t.me/BotFather))  
- Arduino IDE or PlatformIO for flashing the ESP32

---

## 📦 Included Files

- `main.ino` — Main program with Telegram bot logic  
- `config.h` — Configuration constants for WiFi, bot token, PC IP, MAC, etc.  
- `README.md` — This document  

---

## ⚡ Setup Instructions

1. **Configure `config.h`**  
   Update your WiFi credentials, Telegram bot token, allowed user ID, PC IP and MAC address.

2. **Enable WoL on your PC**  
   Make sure Wake On LAN is enabled in BIOS and network adapter settings.

3. **Install Arduino Libraries**  
   - WiFi  
   - WiFiMulti  
   - WiFiClientSecure  
   - WiFiUDP  
   - ESPping  
   - WakeOnLan  
   - UniversalTelegramBot  
   - ArduinoJson  

4. **Flash the ESP32**  
   Upload the code using Arduino IDE or PlatformIO.

5. **Use Telegram**  
   Open Telegram, start your bot, and send commands like `/wol`, `/ping`, `/status`, `/help`.

---

## 📋 Available Commands

| Command  | Description                             |
| -------- | --------------------------------------- |
| `/start` | Welcome message and bot introduction |
| `/wol`   | Send Magic Packet to power on your PC |
| `/ping`  | Check if the PC is on or reachable |
| `/status`| Show current bot status and uptime |
| `/help`  | List all available commands |

---

## 💡 How It Works

- ESP32 connects to your WiFi network.  
- It listens for Telegram messages from your allowed user ID only.  
- When you send `/wol`, it sends a Magic Packet to your PC’s MAC address.  
- `/ping` uses ICMP ping to check if your PC is online.  
- `/status` reports bot uptime and WiFi info.  
- Time is synchronized via NTP to keep logs accurate.

---

## 🔐 Security Notes

- Only your Telegram user ID can control the bot (configured in `config.h`).  
- Uses secure TLS connection to Telegram API.  
- Avoid sharing your bot token or WiFi credentials publicly.
