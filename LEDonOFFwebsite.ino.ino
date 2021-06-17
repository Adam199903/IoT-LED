#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

const char* ssid = "SKYA18AA";
const char* password = "SQEVETCR";

const int ledPin = 13;
String ledState;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(ledPin)){
    ledState = "ON";
    }
    else{
    ledState = "OFF";
    }
    Serial.print(ledState);
    return ledState;
   }
  return String();  
}


void setup(){
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  //intalize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("Error with External Files");
    return;
  }

  //connect to wifi
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("connecting to wifi...");
  }

  //get the esp32s IP adress
  Serial.println(WiFi.localIP());

  //load html page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request -> send(SPIFFS, "/index.html", String(), false, processor);
  });

  //load style.css file we made
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request -> send(SPIFFS, "/style.html","text/css");
  });

 //code to handle the on and off buttons on website
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, HIGH);
    request -> send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, LOW);
    request->send(SPIFFS, "/index.html", String(),false, processor);
  });

  server.begin();

}

void loop(){}
