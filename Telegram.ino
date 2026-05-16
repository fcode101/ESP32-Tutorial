/*
Author : F-Code101
YouTube : https://youtube.com/@F-Code101
Instagram : https://www.instagram.com/f_code101/
Github : https://github.com/fcode101

This code is for controlling an LED connected to an ESP32 using a Telegram bot.
Make sure to replace "Your WIFI NAME", "Your WIFI PASSWORD", "Telegram Bot Token", 
and "xxxxxxxxxx" with your chat ID.

this code will turn the LED on when the user sends the command "/on" 
and turn it off when the user sends the command "/off".

Feel free to modify the code to add more features or control other devices!
*/ 

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

const char* ssid = "Your WIFI NAME";
const char* password = "Your WIFI PASSWORD";

// Telegram Bot Token
#define BOTtoken "Telegram Bot Token"

// Your Telegram Chat ID
#define CHAT_ID "xxxxxxxxxx"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

const int ledPin = 2;

unsigned long lastTimeBotRan;
int botRequestDelay = 1000;

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println("WiFi Connected");
  Serial.println(WiFi.localIP());

  client.setInsecure();

}

void handleNewMessages(int numNewMessages) {

  for (int i = 0; i < numNewMessages; i++) {

    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    Serial.println(text);

    if (text == "/start") {

      String welcome = "ESP32 LED Control Bot\n\n";
      welcome += "/on : Turn LED ON\n";
      welcome += "/off : Turn LED OFF";

      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/on") {
      digitalWrite(ledPin, HIGH);
      bot.sendMessage(chat_id, "LED is ON", "");
    }

    if (text == "/off") {
      digitalWrite(ledPin, LOW);
      bot.sendMessage(chat_id, "LED is OFF", "");
    }
  }
}

void loop() {

  if (millis() - lastTimeBotRan > botRequestDelay) {

    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {

      handleNewMessages(numNewMessages);

      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    lastTimeBotRan = millis();
  }
}

/*
Author : F-Code101
YouTube : https://youtube.com/@F-Code101
Instagram : https://www.instagram.com/f_code101/
Github : https://github.com/fcode101
*/