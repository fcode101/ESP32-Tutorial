#include <WiFi.h>
#include <Preferences.h>

Preferences prefs;

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

uint8_t bssid[6];
int channel = 0;

bool fastConnect()
{
    prefs.begin("wifi", true);

    channel = prefs.getInt("channel", 0);
    prefs.getBytes("bssid", bssid, sizeof(bssid));

    prefs.end();

    if (channel == 0)
    {
        Serial.println("No saved WiFi info");
        return false;
    }

    Serial.println("Using saved BSSID");

    WiFi.begin(
        ssid,
        password,
        channel,
        bssid,
        true
    );

    unsigned long start = millis();

    while (WiFi.status() != WL_CONNECTED &&
           millis() - start < 5000)
    {
        delay(10);
    }

    return WiFi.status() == WL_CONNECTED;
}

void saveWiFiInfo()
{
    prefs.begin("wifi", false);

    prefs.putInt("channel", WiFi.channel());
    prefs.putBytes("bssid", WiFi.BSSID(), 6);

    prefs.end();

    Serial.println("WiFi info saved");
}

bool normalConnect()
{
    Serial.println("Normal connect");

    WiFi.begin(ssid, password);

    unsigned long start = millis();

    while (WiFi.status() != WL_CONNECTED &&
           millis() - start < 10000)
    {
        delay(100);
    }

    return WiFi.status() == WL_CONNECTED;
}

void setup()
{
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);

    unsigned long startTime = millis();

    bool connected = fastConnect();

    if (!connected)
    {
        connected = normalConnect();

        if (connected)
        {
            saveWiFiInfo();
        }
    }

    if (connected)
    {
        Serial.println("Connected!");

        Serial.print("IP: ");
        Serial.println(WiFi.localIP());

        Serial.print("Time: ");
        Serial.print(millis() - startTime);
        Serial.println(" ms");
    }
    else
    {
        Serial.println("WiFi connection failed");
    }
}

void loop()
{
}

/*
The code may contain some errors.
Please feel free to correct and improve it.

Thank you ❤️
*/
