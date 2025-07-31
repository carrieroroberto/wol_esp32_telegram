#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include <ESPping.h>
#include "WakeOnLan.h"
#include "UniversalTelegramBot.h"
#include "ArduinoJson.h"
#include "config.h"

WiFiMulti wifiMulti;
WiFiClientSecure secured_client;
WiFiUDP UDP;
WakeOnLan WOL(UDP);
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
const unsigned long BOT_MTBS = 1000;
unsigned long bot_lasttime = 0;
unsigned long startMillis;

void setup() {
  Serial.begin(115200);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASS);

  Serial.print("Connessione WiFi...");
  while ((wifiMulti.run() != WL_CONNECTED)) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnesso a WiFi.");
  Serial.println(WiFi.localIP());

  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  WOL.calculateBroadcastAddress(WiFi.localIP(), WiFi.subnetMask());

  configTime(0, 0, "pool.ntp.org");
  time_t now = time(nullptr);
  while (now < 24 * 3600) {
    delay(150);
    now = time(nullptr);
  }

  startMillis = millis();
}

void loop() {
  if (millis() - bot_lasttime > BOT_MTBS) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      for (int i = 0; i < numNewMessages; i++) {
        if (bot.messages[i].from_id != ALLOWED_ID)
          continue;

        String chat_id = bot.messages[i].chat_id;
        String text = bot.messages[i].text;
        String from_name = bot.messages[i].from_name;
        if (from_name == "") from_name = "Guest";

        if (text == "/start") {
          String welcome = "👋🏻 Ciao, sono *WoL*, il tuo bot personale di *WakeOnLan*!\n\n";
          welcome += "Puoi utilizzare i seguenti *comandi*:\n";
          welcome += "🚀 /wol - Accendi il PC con Magic Packet\n";
          welcome += "💖 /ping - Verifica se il PC è acceso\n";
          welcome += "📊 /status - Visualizza lo stato del bot\n";
          welcome += "✨ /start - Mostra il messaggio di benvenuto\n";
          bot.sendMessage(chat_id, welcome, "Markdown");
        }
        else if (text == "/wol") {
          if(Ping.ping(PC_IP)) {
            bot.sendMessage(chat_id, "✅ Il PC è *già acceso*", "Markdown");
          }
          else {
            WOL.sendMagicPacket(MAC_ADDR);
            bot.sendMessage(chat_id, "🚀 *Magic Packet inviato!* Attendi che il PC si accenda", "Markdown");
            delay(30000);
            if (Ping.ping(PC_IP)) {
              bot.sendMessage(chat_id, "✅ Il PC si è *acceso con successo*", "Markdown");
            } else {
              bot.sendMessage(chat_id, "❌ Il PC *non si è acceso* (dopo 30 secondi). Verifica eventuali problemi", "Markdown");
            }
          }
        } 
        else if (text == "/ping") {
          if (Ping.ping(PC_IP)) {
            bot.sendMessage(chat_id, "✅ Il PC *è acceso*", "Markdown");
          } else {
            bot.sendMessage(chat_id, "❌ Il PC *è spento* o *non raggiungibile*", "Markdown");
          }
        } 
        else if (text == "/status") {
          unsigned long uptime_ms = millis() - startMillis;
          unsigned long seconds = uptime_ms / 1000;
          unsigned long minutes = seconds / 60;
          unsigned long hours = minutes / 60;
          seconds = seconds % 60;
          minutes = minutes % 60;

          String status = "📊 Stato del bot:\n\n";
          status += "🟢 *Online*\n";
          status += "🌐 IP locale: " + WiFi.localIP().toString() + "\n";
          status += "📶 SSID: " + String(WiFi.SSID()) + "\n";
          status += "📈 Uptime: " + String(hours) + "h " + String(minutes) + "m " + String(seconds) + "s\n";
          bot.sendMessage(chat_id, status, "Markdown");
        }
      }
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    bot_lasttime = millis();
  }
  delay(10);
}