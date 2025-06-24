#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include "WakeOnLan.h"
#include "UniversalTelegramBot.h"
#include "ArduinoJson.h"
#include "credentials.h"

WiFiMulti wifiMulti;
WiFiClientSecure secured_client;
WiFiUDP UDP;
WakeOnLan WOL(UDP);
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
const unsigned long BOT_MTBS = 1000;
unsigned long bot_lasttime = 0;

void sendWOL() {
  WOL.sendMagicPacket(MAC_ADDR);
}

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    if (bot.messages[i].from_id != ALLOWED_ID)
      continue;

    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if (text == "/wol") {
      sendWOL();
      bot.sendMessage(chat_id, "Magic Packet sent!", "");
    } else if (text == "/ping") {
      bot.sendMessage(chat_id, "Pong!", "");
    } else if (text == "/start") {
      String welcome = "Welcome to **WoL Bot**, " + from_name + "!\n";
      welcome += "/wol : Send the Magic Packet\n";
      welcome += "/ping : Check the bot status\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}

void setup() {
  wifiMulti.addAP(WIFI_SSID, WIFI_PASS);

  while ((wifiMulti.run() != WL_CONNECTED)) {
    delay(500);
  }

  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  WOL.calculateBroadcastAddress(WiFi.localIP(), WiFi.subnetMask());

  configTime(0, 0, "pool.ntp.org");
  time_t now = time(nullptr);
  while (now < 24 * 3600) {
    delay(150);
    now = time(nullptr);
  }
}

void loop() {
  if (millis() - bot_lasttime > BOT_MTBS) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    bot_lasttime = millis();
  }
  delay(10);
}
