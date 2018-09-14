/**
 * @brief Includes
 */
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

#include "index.h"

/**
 * @brief Declarations
 */
ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);
WebSocketsServer webSocket(81);

const char* ssid = "cascaval";
const char* password = "cascava1";

#define LED_PIN 2

/**
 * @brief Starts the ESP in AP and STA mode
 */
void startWiFi() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password, 9);

  WiFi.begin("WifiSSD", "password");
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  //Serial.println(WiFi.localIP());
}

/**
 * @brief Handles web socket events
 */
void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      //Serial.printf("[%u] Disconnected!\n", num);
    break;

    case WStype_CONNECTED: {
      IPAddress ip = webSocket.remoteIP(num);
      //Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
    } break;

    case WStype_TEXT: {
      Serial.print((char)*payload);
    } break;
  }
}
void startWebSocket() {
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  //Serial.println("WebSocket server started");
}

/**
 * @brief Start http server and handles requests
 */
void handleNotFound() {
  server.send(404, "text/plain", "404: File Not Found");
}
void handleRoot() {
  server.send(200, "text/html", MAIN_page);
}
void startServer() {
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
}

/**
 * @brief Defines pin modes and serial speed and starts the web socket and http server
 */
void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);

  startWiFi();
  startWebSocket();
  startServer();
}

/**
 * @brief Calls the client requests handling functions
 */
void loop() {
  webSocket.loop();
  server.handleClient();
}
