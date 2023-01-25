#include <M5StickC.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include "esp_wifi.h"

const char *ssid = "M5Stick_";
const char *password = "12345678";


WiFiServer server(80);

const int pin0 = 0;
const int pin26 = 26;
const int pin36 = 36;
const int pin32 = 32;
const int pin33 = 33;

void setup() {
  M5.begin();
  pinMode(pin0, OUTPUT);
  pinMode(pin26, OUTPUT);
  pinMode(pin36, OUTPUT);
  pinMode(pin32, OUTPUT);
  pinMode(pin33, OUTPUT);


  Serial.begin(9600);
  M5.lcd.println("WIFI ACCESS POINT");
  M5.lcd.printf("Please connect:%s \nThen access to:", ssid);
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();

  M5.lcd.println(myIP);
  server.begin();
}


void loop() {
  WiFiClient client = server.available();

  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') { 
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print("<canvas id=\"canvas\"></canvas>");
            client.print("<style>body {overflow: hidden;user-select: none;}canvas {position: absolute;top: 0;left: 0;z-index: 0;}</style>");
            client.print("<script>const canvas = document.getElementById(\"canvas\");const ctx = canvas.getContext(\"2d\");const pi = Math.PI;const stateObject = {id: \"100\"};var gradient;function fixDpiResizeCanvas() {  const dpr = window.devicePixelRatio || 1;  canvas.style.width = window.innerWidth + \"px\";  canvas.style.height = window.innerHeight + \"px\";  canvas.width = window.innerWidth * dpr;  canvas.height = window.innerHeight * dpr;  ctx.setTransform(dpr, 0, 0, dpr, 0, 0);  gradient = ctx.createLinearGradient(0, canvas.height, canvas.width, 0);  gradient.addColorStop(0, \"#88d8ff\");  gradient.addColorStop(1, \"#d899ff\");}fixDpiResizeCanvas();window.addEventListener(\"resize\", fixDpiResizeCanvas);var positions = {  fixedX: undefined,  fixedY: undefined,  innerX: undefined,  innerY: undefined};var angle = undefined;function touchStart(x, y) {  if (positions.fixedX || positions.fixedY) return;  positions.fixedX = positions.innerX = x;  positions.fixedY = positions.innerY = y;}function touchMove(x, y) {  if (!(positions.fixedX || positions.fixedY)) return;    positions.innerX = x;  positions.innerY = y;  angle = Math.atan2(    positions.innerY - positions.fixedY,    positions.innerX - positions.fixedX  );  if (!(    (x - positions.fixedX) ** 2 +    (y - positions.fixedY) ** 2 < 10000    ))  {    positions.innerX = Math.round(Math.cos(angle) * 100 + positions.fixedX);    positions.innerY = Math.round(Math.sin(angle) * 100 + positions.fixedY);  }}function touchEndOrCancel() {  positions.fixedX  = positions.fixedY  = positions.innerX  = positions.innerY  = angle  = undefined;  window.history.pushState(stateObject, \"Page 1\", \"/000\");}canvas.addEventListener(\"touchstart\", function(e) {  touchStart(e.touches[0].clientX, e.touches[0].clientY);});canvas.addEventListener(\"touchmove\", function(e) {  touchMove(e.touches[0].clientX, e.touches[0].clientY)});canvas.addEventListener(\"touchend\", touchEndOrCancel);canvas.addEventListener(\"touchcancel\", touchEndOrCancel);canvas.addEventListener(\"mousedown\", function (e) {  touchStart(e.offsetX, e.offsetY);});canvas.addEventListener(\"mousemove\", function (e) {  touchMove(e.offsetX, e.offsetY);});canvas.addEventListener(\"mouseup\", touchEndOrCancel);function renderLoop() {  requestAnimationFrame(renderLoop);    ctx.clearRect(0, 0, canvas.width, canvas.height);  ctx.fillStyle = gradient;  ctx.fillRect(0, 0, canvas.width, canvas.height);  var displayAngle = (-angle + 2*pi) % (2*pi);        ctx.fillStyle = \"#0008\";  if (!(positions.fixedX || positions.fixedY)) {    ctx.fillText(\"Touch the screen\", 20, 20);    return;  };    ctx.fillText(    `Angle: ${Math.round((displayAngle * 180) / pi)} degrees (${      Math.round(displayAngle * 100) / 100    } radians)`,    20,    20  );  ctx.fillText(`Raw angle: ${Math.round(angle * 100) / 100} radians (inverted Y axis)`, 20, 50);  ctx.fillText(`Inner joystick: (${positions.innerX},${positions.innerY})`, 20, 80);  ctx.fillText(`Touch start point: (${positions.fixedX},${positions.fixedY}) or (${Math.round(positions.fixedX/window.innerWidth*1000)/1000},${Math.round(positions.fixedY/window.innerHeight*1000)/1000})`, 20, 110);    ctx.beginPath();  ctx.fillStyle = \"#0004\";  ctx.arc(positions.fixedX, positions.fixedY, 100, 0, 2 * pi);  ctx.fill();  ctx.closePath();  ctx.beginPath();  ctx.fillStyle = \"#0008\";  ctx.arc(positions.innerX, positions.innerY, 30, 0, 2 * pi);  ctx.fill();  ctx.closePath();  let angleAbsolute = Math.round((displayAngle * 180) / pi);  let temp  = \"\";  if (angleAbsolute < 22.5 || angleAbsolute > 337.5) {      temp = \"right\";  } else if (angleAbsolute > 22.5 && angleAbsolute < 67.5) {          temp = \"froRig\";  } else if (angleAbsolute > 67.5 && angleAbsolute < 112.5) {          temp = \"front\";  } else if (angleAbsolute > 112.5 && angleAbsolute < 157.5) {          temp = \"froLef\";   } else if (angleAbsolute > 157.5 && angleAbsolute < 202.5) {          temp = \"left\";   } else if (angleAbsolute > 202.5 && angleAbsolute < 247.5) {          temp = \"bacLef\";   } else if (angleAbsolute > 247.5 && angleAbsolute < 292.5) {          temp = \"back\";   } else if (angleAbsolute > 292.5 && angleAbsolute < 337.5) {          temp = \"bacRig\";  }   let differenceY = positions.fixedY - positions.innerY;  let differenceX = positions.fixedX - positions.innerX;  let distance = Math.sqrt(differenceX * differenceX + differenceY * differenceY);  if (distance < 20) {    window.history.pushState(stateObject, \"Page 1\", \"/\" + temp + \"000\");  } else if (distance < 50) {    window.history.pushState(stateObject, \"Page 1\", \"/\" + temp + \"100\");  } else if (distance < 75) {    window.history.pushState(stateObject, \"Page 1\", \"/\" + temp + \"200\");  } else if (distance < 100) {    window.history.pushState(stateObject, \"Page 1\", \"/\" + temp + \"300\");  }}renderLoop();ctx.font = \"20px Helvetica\";ctx.textBaseline = \"top\";</script>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        if (currentLine.endsWith("000")) {
          digitalWrite(pin0, LOW);
          digitalWrite(pin26, LOW);
        } else if (currentLine.endsWith("100")) {
          digitalWrite(pin0, LOW);
          digitalWrite(pin26, HIGH);
        } else if (currentLine.endsWith("200")) {
          digitalWrite(pin0, HIGH);
          digitalWrite(pin26, LOW);
        } else if (currentLine.endsWith("300")) {
          digitalWrite(pin0, HIGH);
          digitalWrite(pin26, HIGH);
        }

        if (currentLine.indexOf("front") != -1) {
          digitalWrite(pin32, LOW);
          digitalWrite(pin33, LOW);
          digitalWrite(pin36, LOW);
        } else if (currentLine.indexOf("froLef") != -1) {
          digitalWrite(pin32, LOW);
          digitalWrite(pin33, LOW);
          digitalWrite(pin36, HIGH);
        } else if (currentLine.indexOf("froRig") != -1) {
          digitalWrite(pin32, LOW);
          digitalWrite(pin33, HIGH);
          digitalWrite(pin36, LOW);
        } else if (currentLine.indexOf("left") != -1) {
          digitalWrite(pin32, LOW);
          digitalWrite(pin33, HIGH);
          digitalWrite(pin36, HIGH);
        } else if (currentLine.indexOf("right") != -1) {
          digitalWrite(pin32, HIGH);
          digitalWrite(pin33, LOW);
          digitalWrite(pin36, LOW);
        } else if (currentLine.indexOf("bacLef") != -1) {
          digitalWrite(pin32, HIGH);
          digitalWrite(pin33, LOW);
          digitalWrite(pin36, HIGH);
        } else if (currentLine.indexOf("bacRig") != -1) {
          digitalWrite(pin32, HIGH);
          digitalWrite(pin33, HIGH);
          digitalWrite(pin36, LOW);
        } else if (currentLine.indexOf("back") != -1) {
          digitalWrite(pin32, HIGH);
          digitalWrite(pin33, HIGH);
          digitalWrite(pin36, HIGH);
        }
      }
    }
    client.stop();
  }
}