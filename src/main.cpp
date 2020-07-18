/* 
This code is based on code from tutorial: https://circuits4you.com/2019/03/22/esp8266-weather-station-arduino/
but with significant revisions in backend.
*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <AM2320.h>
#include <SFE_BMP180.h>
#include <BH1750.h>

#include "index.h" // HTML, CSS and JS (AJAX)

const char* ssid = "ssid_name";
const char* password = "wifi_password";

ESP8266WebServer server(80);
AM2320 am2320;
SFE_BMP180 bmp180;
BH1750 bh1750;

void handleRoot(); // func of send main page in root dir
void handleTHPL(); // func of read sensors data and send in JSON
void getAM2320 (float& hum); // func of read data from AM2320 sensor
void getBMP180(double& temp, double& press); // func of read data from BMP180 sensor
void getBH1750(float& lux); // func of read data from BH1750 sensor

void setup() {
    Serial.begin(115200);
    am2320.begin();
    bmp180.begin();
    bh1750.begin();

    WiFi.begin(ssid,password);
    Serial.println("");
    while(WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.on("/readADC", handleTHPL);
    server.begin();
    Serial.println("HTTP server started");
    handleTHPL(); // output send data in serial monitor
}

void loop() {
    server.handleClient(); // send data to client requests  
}

void getAM2320 (float& hum){
    Serial.println("AM2320:");
    if(am2320.measure()){
        //Serial.print("Temperature: "); 
        //Serial.println(am2320.getTemperature());
        Serial.print("Humidity: ");
        hum = am2320.getHumidity();
        Serial.println(hum);
    }else{
        short int errorCode = am2320.getErrorCode();
        switch (errorCode){
            case 1:
                Serial.println("ERR: Sensor is offline");
                break;
            case 2:
                Serial.println("ERR: CRC validation failed");
                break;
        }
    }
}

void getBMP180(double& temp, double& press){
    char status;
    status = bmp180.startTemperature();
    Serial.println("BMP180:");
    if(status != 0){
        delay(status);
        status = bmp180.getTemperature(temp);
        if(status !=0){
            Serial.print("Temperature: ");
            Serial.print(temp,2);
            Serial.println(" *C");

            status = bmp180.startPressure(3);
            if(status != 0){
                delay(status);
                status = bmp180.getPressure(press,temp);
                if(status != 0){
                    Serial.print("Pressure: ");
                    Serial.print(press,2);
                    Serial.println(" mb");
                }else{
                    Serial.println("ERROR retrieving pressure measurement");
                }
            }else{
                Serial.println("ERROR starting pressure measurement");
            }
        }else{
            Serial.println("ERROR retrieving temperature measurement");
        }
    }else{
        Serial.println("ERROR starting temperature measurement");
    }
}

void getBH1750(float& lux){
    Serial.println("BH1750:");
    lux = bh1750.readLightLevel();
    if(lux < 0){
        Serial.println("ERROR connect BH1750!");
    }else{
        Serial.print("Light: ");
        Serial.print(lux);
        Serial.println(" lx");
    }
}

void handleRoot(){
    server.send(200, "text/html", MAIN_page); // send main page from PROGMEM
}

void handleTHPL(){
    double Temp = 0, Press = 0;
    float Hum = 0, Lux = 0;
    getBMP180(Temp,Press);
    getAM2320(Hum);
    getBH1750(Lux);
    String data = "{\"Temperature\":\""+String(Temp)+"\",\"Pressure\":\""+String(Press)+"\",\"Humidity\":\""+String(Hum)+"\",\"Lux\":\""+String(Lux)+"\"}";
    server.send(200,"text/plain",data);
}
