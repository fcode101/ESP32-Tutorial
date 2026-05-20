/*
Author : F-Code101
YouTube : https://youtube.com/@F-Code101
Instagram : https://www.instagram.com/f_code101/
Github : https://github.com/fcode101


Use PlatformIO to upload the code to your ESP32.
Make sure to install the LittleFS plugin in PlatformIO and upload the song,
html page and Cover image in the "data" folder to the ESP32's filesystem.

The HTML code is given below, create a new html file and place it in "data" folder

*/

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

const char *ssid = "ESP32-Music";
const char *password = "12345678";

AsyncWebServer server(80);

void setup()
{

  Serial.begin(115200);

  if (!LittleFS.begin(true))
  {
    Serial.println("LittleFS Mount Failed");
    return;
  }

  Serial.println("LittleFS Ready");

  // Start Access Point
  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.println("WiFi Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/index.html", "text/html"); });

  server.on("/song.mp3", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/song.mp3", "audio/mpeg"); });

  server.serveStatic("/", LittleFS, "/");

  server.begin();

  Serial.println("Server Started");
}

void loop()
{
}

/*
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<title>ESP32 Music</title>

<link href="https://fonts.googleapis.com/css2?family=Poppins:wght@300;400;600;700&display=swap" rel="stylesheet">

<style>

*{
  margin:0;
  padding:0;
  box-sizing:border-box;
  font-family:'Poppins',sans-serif;
}

body{
  height:100vh;
  background:linear-gradient(135deg,#0f0f0f,#1a1a1a,#000);
  display:flex;
  justify-content:center;
  align-items:center;
  overflow:hidden;
  color:white;
}

.background{
  position:absolute;
  width:500px;
  height:500px;
  background:#1db954;
  filter:blur(180px);
  opacity:0.25;
  border-radius:50%;
  animation:float 6s ease-in-out infinite;
}

@keyframes float{
  0%{transform:translateY(0px);}
  50%{transform:translateY(-25px);}
  100%{transform:translateY(0px);}
}

.player{
  position:relative;
  width:350px;
  padding:30px;
  border-radius:30px;
  background:rgba(255,255,255,0.08);
  backdrop-filter:blur(20px);
  border:1px solid rgba(255,255,255,0.1);
  box-shadow:0 8px 40px rgba(0,0,0,0.5);
  text-align:center;
}

.cover{
  width:220px;
  height:220px;
  margin:auto;
  border-radius:25px;
  overflow:hidden;
  box-shadow:0 10px 30px rgba(0,0,0,0.6);
}

.cover img{
  width:100%;
  height:100%;
  object-fit:cover;
}

.title{
  margin-top:25px;
  font-size:24px;
  font-weight:700;
}

.artist{
  margin-top:8px;
  color:#b3b3b3;
  font-size:14px;
}

.controls{
  display:flex;
  justify-content:center;
  align-items:center;
  gap:20px;
  margin-top:30px;
}

.btn{
  width:65px;
  height:65px;
  border:none;
  border-radius:50%;
  background:#1db954;
  color:white;
  font-size:24px;
  cursor:pointer;
  transition:0.25s;
  box-shadow:0 5px 20px rgba(29,185,84,0.4);
}

.btn:hover{
  transform:scale(1.1);
}

.small-btn{
  width:50px;
  height:50px;
  background:#222;
  box-shadow:none;
}

.progress-container{
  margin-top:30px;
}

.progress{
  width:100%;
  height:6px;
  background:#333;
  border-radius:20px;
  overflow:hidden;
}

.progress-bar{
  width:0%;
  height:100%;
  background:#1db954;
}

.time{
  display:flex;
  justify-content:space-between;
  margin-top:8px;
  font-size:12px;
  color:#aaa;
}

.bottom{
  margin-top:25px;
  font-size:13px;
  color:#888;
}

.glow{
  position:absolute;
  inset:0;
  border-radius:30px;
  border:1px solid rgba(255,255,255,0.08);
  pointer-events:none;
}

</style>
</head>

<body>

<div class="background"></div>

<div class="player">

<div class="glow"></div>

<div class="cover">
<img src="/cover.jpg" alt="Album Cover">
</div>

<div class="title">
F-Code 101 Beats
</div>

<div class="artist">
Powered by ESP32
</div>

<div class="controls">

<button class="btn small-btn">
⏮
</button>

<button class="btn" onclick="togglePlay()" id="playBtn">
▶
</button>

<button class="btn small-btn">
⏭
</button>

</div>

<div class="progress-container">

<div class="progress">
<div class="progress-bar" id="progressBar"></div>
</div>

<div class="time">
<span id="current">0:00</span>
<span id="duration">0:00</span>
</div>

</div>

<div class="bottom">
Don't use Spotify. Use ESP32.
</div>

<audio id="audio">
<source src="/song.mp3" type="audio/mpeg">
</audio>

</div>

<script>

const audio = document.getElementById("audio");
const playBtn = document.getElementById("playBtn");
const progressBar = document.getElementById("progressBar");

function togglePlay(){

  if(audio.paused){
    audio.play();
    playBtn.innerHTML = "❚❚";
  }else{
    audio.pause();
    playBtn.innerHTML = "▶";
  }

}

audio.addEventListener("timeupdate",()=>{

  const progress = (audio.currentTime / audio.duration) * 100;
  progressBar.style.width = progress + "%";

  document.getElementById("current").innerHTML =
    formatTime(audio.currentTime);

  document.getElementById("duration").innerHTML =
    formatTime(audio.duration);

});

function formatTime(time){

  if(isNaN(time)) return "0:00";

  let mins = Math.floor(time / 60);
  let secs = Math.floor(time % 60);

  if(secs < 10) secs = "0" + secs;

  return mins + ":" + secs;
}

</script>

</body>
</html>

*/

/*
Author : F-Code101
YouTube : https://youtube.com/@F-Code101
Instagram : https://www.instagram.com/f_code101/
Github : https://github.com/fcode101
*/