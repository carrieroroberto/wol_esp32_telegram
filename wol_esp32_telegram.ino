// Include required libraries
#include <WiFi.h>                  // Core WiFi support
#include <WiFiMulti.h>            // Allows connecting to multiple WiFi networks
#include <WiFiClientSecure.h>     // Secure HTTPS client
#include <WiFiUdp.h>              // UDP support
#include <ESPping.h>              // ICMP Ping support
#include "WakeOnLan.h"            // WoL packet sender
#include "UniversalTelegramBot.h" // Telegram Bot API
#include "ArduinoJson.h"          // JSON parsing
#include "config.h"               // Project-specific configuration

// Instantiate required objects
WiFiMulti wifiMulti;                            // Manages multiple WiFi credentials
WiFiClientSecure secured_client;                // Secure HTTPS connection to Telegram
WiFiUDP UDP;                                    // UDP client (used for WoL)
WakeOnLan WOL(UDP);                             // Wake-on-LAN packet sender
UniversalTelegramBot bot(BOT_TOKEN, secured_client);  // Telegram Bot instance

// Timing constants
const unsigned long BOT_MTBS = 1000;  // Milliseconds between bot polling
unsigned long bot_lasttime = 0;       // Last bot update check
unsigned long startMillis;            // Bot start time for uptime tracking

void setup() {
  Serial.begin(115200); // Start serial communication

  // Connect to WiFi using stored credentials
  wifiMulti.addAP(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  while ((wifiMulti.run() != WL_CONNECTED)) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected to WiFi");
  Serial.println(WiFi.localIP());  // Print local IP address

  // Set up HTTPS certificate for Telegram
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  // Calculate broadcast address for WoL
  WOL.calculateBroadcastAddress(WiFi.localIP(), WiFi.subnetMask());

  // Synchronize time from NTP server (important for HTTPS)
  configTime(0, 0, "pool.ntp.org");
  time_t now = time(nullptr);
  while (now < 24 * 3600) { // Wait until time is valid
    delay(150);
    now = time(nullptr);
  }

  // Store boot timestamp for uptime reporting
  startMillis = millis();
}

void loop() {
  // Check for new Telegram messages at defined interval
  if (millis() - bot_lasttime > BOT_MTBS) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      for (int i = 0; i < numNewMessages; i++) {
        if (bot.messages[i].from_id != ALLOWED_ID)
          continue; // Ignore unauthorized users

        String chat_id = bot.messages[i].chat_id;
        String text = bot.messages[i].text;
        String from_name = bot.messages[i].from_name;
        if (from_name == "") from_name = "Guest";

        // Handle /start command
        if (text == "/start") {
          String welcome = "👋🏻 Hi, I'm *WoL*, your personal *Wake-on-LAN* bot!\n\n";
          welcome += "You can use the following *commands*:\n";
          welcome += "🚀 /wol - Turn on your PC using Magic Packet\n";
          welcome += "💖 /ping - Check if the PC is on\n";
          welcome += "📊 /status - Show bot status\n";
          welcome += "✨ /start - Show this welcome message\n";
          bot.sendMessage(chat_id, welcome, "Markdown");
        }

        // Handle /wol command (send magic packet if PC is off)
        else if (text == "/wol") {
          if (Ping.ping(PC_IP)) {
            bot.sendMessage(chat_id, "✅ The PC is *already on*", "Markdown");
          } else {
            WOL.sendMagicPacket(MAC_ADDR);
            bot.sendMessage(chat_id, "🚀 *Magic Packet sent!* Please wait while the PC powers on...", "Markdown");
            delay(30000); // Wait 30 seconds before checking again
            if (Ping.ping(PC_IP)) {
              bot.sendMessage(chat_id, "✅ The PC *successfully turned on*", "Markdown");
            } else {
              bot.sendMessage(chat_id, "❌ The PC *did not turn on* (after 30 seconds). Please check for issues", "Markdown");
            }
          }
        }

        // Handle /ping command (check PC availability)
        else if (text == "/ping") {
          if (Ping.ping(PC_IP)) {
            bot.sendMessage(chat_id, "✅ The PC is *online*", "Markdown");
          } else {
            bot.sendMessage(chat_id, "❌ The PC is *offline* or *not reachable*", "Markdown");
          }
        }

        // Handle /status command (show system status)
        else if (text == "/status") {
          unsigned long uptime_ms = millis() - startMillis;
          unsigned long seconds = uptime_ms / 1000;
          unsigned long minutes = seconds / 60;
          unsigned long hours = minutes / 60;
          seconds = seconds % 60;
          minutes = minutes % 60;

          String status = "📊 Bot Status:\n\n";
          status += "🟢 *Online*\n";
          status += "🌐 Local IP: " + WiFi.localIP().toString() + "\n";
          status += "📶 SSID: " + String(WiFi.SSID()) + "\n";
          status += "📈 Uptime: " + String(hours) + "h " + String(minutes) + "m " + String(seconds) + "s\n";
          bot.sendMessage(chat_id, status, "Markdown");
        }
      }
      // Check if new messages have arrived during processing
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    // Update last polling timestamp
    bot_lasttime = millis();
  }

  delay(10); // Small delay to prevent CPU overuse
}
