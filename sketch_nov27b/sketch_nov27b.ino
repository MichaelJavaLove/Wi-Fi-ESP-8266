// Wi-Fi розетка на ESP8266
#include <ESP8266WiFi.h>
#include "./DNSServer.h"  
#include <ESP8266WebServer.h>
const byte        DNS_PORT = 53;          // ДНС порт
IPAddress         apIP(10, 10, 10, 1);    // сервер сети
DNSServer         dnsServer;              // объект DNS сервера
ESP8266WebServer  webServer(80);          // объект веб-сервера
int GPIO_2 = 2; //Pin defanition - Relay will be connected to GPIO-0
/*START OF HMTL CODE*/
String style_detials =  //эта строка задает атрибуты веб-страницы
   "<style type=\"text/css\">"
   " body{"
    "  background-color: #a69695;"
    "}"
    "button{"
     " display: inline-block;"
    "}"
    "#buttons{"
     " text-align: center;"
    "}"
    ".controllButtons{"
     " margin-top: 15px;"
      "margin-left: 5px;"
      "background-color: white;"
      "padding: 10px;"
      "border:1px solid black;"
      "border-radius: 10px;"
      "cursor: pointer;"
      "font-size: 14px;"
    "}"
    ".controllButtons:hover{"
     " background-color: orange;"
      "padding: 10px;"
      "border:1px solid black;"
      "border-radius: 10px;"
      "cursor: pointer;"
      "font-size: 14px;"
    "}"
    "@media only screen and (max-width: 700px) {"
     " button{"
      "  display: block;"
      "}"
      "#buttons{"
       " margin-top: 10%;"
        "margin-left: 35%;"
      "}"
       " .controllButtons{"
        "  margin-top: 15px;"
        "margin-left: 5px;"
        "background-color: white;"
        "padding: 15px;"
        "border:1px solid black;"
        "border-radius: 10px;"
        "cursor: pointer;"
        "font-size: 16px;"
      "}"
      ".controllButtons:hover{"
       " background-color: orange;"
        "padding: 15px;"
        "border:1px solid black;"
        "border-radius: 10px;"
        "cursor: pointer;"
        "font-size: 16px;"
      "}"
    "}"
  "</style>";
String Home_Screen = "" //Page 1 - код домашнего экрана
                      "<!DOCTYPE html><html>"
                      "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                      + style_detials +
                      "<div id=\"buttons\">"
                      "<a style=\"text-decoration:none;\" href=\"relay_ON\"> <button id=\"switchLight1\" class=\"controllButtons\">Turn ON</button> </a>"
                      "<a style=\"text-decoration:none;\" href=\"relay_OFF\"><button id=\"switchLight2\" class=\"controllButtons\">Turn OFF</button>  </a>"
                      "</div>"
                      "<body><h1>Welcome - CircuitDigest</h1>"
                      "</body></html>";
String ON_Screen = "" //Page 2 - код экрана включения
                      "<!DOCTYPE html><html>"
                      "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                      + style_detials +
                      "<div id=\"buttons\">"
                      "<a style=\"text-decoration:none;\" href=\"relay_ON\"> <button id=\"switchLight1\" class=\"controllButtons\">Turn ON</button> </a>"
                      "<a style=\"text-decoration:none;\" href=\"relay_OFF\"><button id=\"switchLight2\" class=\"controllButtons\">Turn OFF</button>  </a>"
                      "</div>"
                      "<body><h1>Smart Plug - Turned ON</h1>"
                      "</body></html>";
String OFF_Screen = "" //Page 3 - код экрана выключения
                      "<!DOCTYPE html><html>"
                      "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                      + style_detials +
                      "<div id=\"buttons\">"
                      "<a style=\"text-decoration:none;\" href=\"relay_ON\"> <button id=\"switchLight1\" class=\"controllButtons\">Turn ON</button> </a>"
                      "<a style=\"text-decoration:none;\" href=\"relay_OFF\"><button id=\"switchLight2\" class=\"controllButtons\">Turn OFF</button>  </a>"
                      "</div>"
                      "<body><h1>Smart Plug - Turned OFF</h1>"
                      "</body></html>";
                      
/*END OF HMTL CODE*/                    
                      
void setup() {
 pinMode(LED_BUILTIN, OUTPUT); //LED pin выходит
 pinMode(GPIO_2, OUTPUT); //GPIO pin выходит за Relay 
 digitalWrite(GPIO_2, HIGH); // исходное состояние Relay при включении добавленное

  WiFi.mode(WIFI_AP); //устанавливаем модуль ESP в режим точки доступа
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("ESP_Smart_Plug"); // имя нашей точки доступа
  dnsServer.start(DNS_PORT, "*", apIP);
  webServer.onNotFound([]() {
    webServer.sendHeader("Location", String("http://www.circuitdigest-automation.com/home.html"), true); //Открытие домашней страницы
    webServer.send ( 302, "text/plain", "");
  });
  
  webServer.on("/home.html", []() {
    webServer.send(200, "text/html", Home_Screen);
  });
//Экран ON
  webServer.on("/relay_ON", [](){ //If turn on Button is pressed
       digitalWrite(LED_BUILTIN, LOW); //Turn off LED
       digitalWrite(GPIO_2, LOW); // включить Relay изменил выход на плате D4
       webServer.send(200, "text/html", ON_Screen); //Display this screen
  });
//Экран OFF
  webServer.on("/relay_OFF", [](){ //If turn off Button is pressed
       digitalWrite(LED_BUILTIN, HIGH); //Turn on LED
       digitalWrite(GPIO_2, HIGH); //выключить реле Relay
       webServer.send(200, "text/html", OFF_Screen); //Display this screen
  });
  
  // Добавить кнопки в случае необходимости
  webServer.begin();
}
void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}