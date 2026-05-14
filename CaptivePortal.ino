/*
Author : F-Code101
YouTube : https://www.youtube.com/@F-Code101
Instagram : https://www.instagram.com/f_code101/
Github : https://github.com/fcode101

This code creates a captive portal using an ESP32. 
It sets up a Wi-Fi access point and a web server. 
When a user connects to the access point and tries to access any website, 
they will be redirected to the captive portal page where they can turn an LED on or off. 
The DNS server is used to redirect all requests to the captive portal page.

Install the required libraries and upload the code to your ESP32. 
Then, connect to the "ESP32_Portal" Wi-Fi network using the password

it may work or not depending on the device and its captive portal detection mechanism.
*/

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

DNSServer dnsServer;
WebServer server(80);

const byte DNS_PORT = 53;

IPAddress apIP(192,168,4,1);

// Set your credentials
const char* ssid = "ESP32_Portal";
const char* password = "12345678";

const int ledPin = 2;


// =========================
// HTML PAGE
// =========================
void portalPage() {

String page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>ESP32 Portal</title>

<style>

body{
font-family:Arial;
text-align:center;
background:#f2f2f2;
margin-top:60px;
}

h1{
color:#222;
}

button{
padding:18px 40px;
font-size:28px;
border:none;
border-radius:12px;
margin:15px;
color:white;
cursor:pointer;
box-shadow:0 4px 10px rgba(0,0,0,0.2);
}

.on{
background:#28a745;
}

.off{
background:#dc3545;
}

</style>
</head>

<body>

<h1>ESP32 Captive Portal</h1>

<a href="/on">
<button class="on">ON</button>
</a>

<a href="/off">
<button class="off">OFF</button>
</a>

</body>
</html>
)rawliteral";

server.send(200, "text/html", page);
}

// LED CONTROL
void ledOn() {

digitalWrite(ledPin, HIGH);

portalPage();
}

void ledOff() {

digitalWrite(ledPin, LOW);

portalPage();
}

// CAPTIVE PORTAL REDIRECT
void captivePortal() {

String host = server.hostHeader();

if (!host.equals(apIP.toString())) {

server.sendHeader("Location",
String("http://") + apIP.toString(),
true);

server.send(302, "text/plain", "");

return;
}

portalPage();
}
// SETUP
void setup() {

Serial.begin(115200);

pinMode(ledPin, OUTPUT);

WiFi.mode(WIFI_AP);

WiFi.softAPConfig(apIP, apIP, IPAddress(255,255,255,0));

WiFi.softAP(ssid, password);

Serial.println();
Serial.println("AP Started");
Serial.println(WiFi.softAPIP());


// DNS wildcard redirect to capture all requests and redirect to the portal
dnsServer.start(DNS_PORT, "*", apIP);

// ROUTES
server.on("/", portalPage);
server.on("/on", ledOn);
server.on("/off", ledOff);


// Android
server.on("/generate_204", captivePortal);
server.on("/gen_204", captivePortal);

// Windows
server.on("/fwlink", captivePortal);
server.on("/connecttest.txt", captivePortal);
server.on("/ncsi.txt", captivePortal);

// Apple
server.on("/hotspot-detect.html", captivePortal);

// Generic fallback
server.onNotFound(captivePortal);
server.begin();
Serial.println("Web Server Started");
}

void loop() {

dnsServer.processNextRequest();

server.handleClient();
}

/*
Author : F-Code101
YouTube : https://www.youtube.com/@F-Code101
Instagram : https://www.instagram.com/f_code101/
Github : https://github.com/fcode101
*/
