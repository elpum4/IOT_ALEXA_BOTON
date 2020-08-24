#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#include <DNSServer.h>
#include <ESP8266WebServer.h >
#include <WiFiManager.h>
#include <Espalexa.h>
#include "GFButton.h"
int Luz = 0;
int buttonState = 0;             // the current reading from the input pin
bool ConfigurationPortalRequired = false;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50; 
//BOTON
GFButton boton = GFButton(2);
WiFiManager wifiManager;
int estado = LOW;         // the current state of the output pin

int wifi_reset = 0;

//****


Espalexa alexita;
EspalexaDevice* LuzE;
void FuncionLuz(uint8_t brightness);


void setup() {
  Serial.begin(115200);

  if (WiFi.SSID()==""){
    Serial.println("No tenemos ninguna credencial de punto de acceso, así que consígalas ahora");
    wifiManager.resetSettings();
    if(!wifiManager.autoConnect()){
        Serial.println("Fallo en la conexión (timeout)");
        ESP.reset();
        delay(1000);
        }
 
    Serial.println("Ya estás conectado");  
  }
  else{
    WiFi.mode(WIFI_STA); // Force to station mode because if device was switched off while in access point mode it will start up next time in access point mode.
    Serial.println("Ya estás conectado");
  }
  boton.setHoldHandler(boton_presionado_largo);
  boton.setPressHandler(boton_presionado);
  boton.setReleaseHandler(boton_liberado);
  boton.setDebounceTime(50);
  pinMode(Luz, OUTPUT);
  LuzE = new EspalexaDevice("Luz", FuncionLuz);
  alexita.addDevice(LuzE);
  alexita.begin();
}

void loop() {
  alexita.loop();
  boton.process();
}

void boton_presionado(GFButton & button) {
  
  if (buttonState ==0){
    if (estado == 1){
            LuzE->setValue(0);
            }
        else {
          LuzE->setValue(255);
        }
        
        estado = !estado;
        digitalWrite(Luz,estado);
        Serial.printf("Luz Boton\n");   
      }
buttonState = 1;    
}

 void boton_liberado(GFButton & button) {
  
buttonState = 0; 
  
}

void boton_presionado_largo(GFButton & button) {
  Serial.println("Presionado durante ");
  Serial.print(button.getHoldTime());
  if (boton.getHoldTime()> 4999) {
    Serial.println("Reseteando ");
    wifiManager.resetSettings();
    delay(500);
    if(!wifiManager.autoConnect()){
        Serial.println("Fallo en la conexión (timeout)");
        ESP.reset();
        delay(1000);
        }
 
    Serial.println("Ya estás conectado");  
   }
}


void FuncionLuz(uint8_t brightness) {
  Serial.print("Funcion Luz - ");

  if (brightness) {
    digitalWrite(Luz, 1);
    estado = 1;
    Serial.println(" Encender ");
  }
  else {
    digitalWrite(Luz, 0);
    estado = 0;
    Serial.println(" Apagar ");
  }
}
