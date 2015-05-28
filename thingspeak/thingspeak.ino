#include<stdlib.h>
#include <SoftwareSerial.h>
//#include <OneWire.h>
//#include <DallasTemperature.h>
#define ONE_WIRE_BUS 8
//OneWire oneWire(ONE_WIRE_BUS);
//DallasTemperature sensors(&oneWire);</p><p>
#define SSID "KV_2"
#define PASS "vlta???????????"
#define IP "184.106.153.149" // thingspeak.com
String GET = "GET /update?key=0RI7PCVZA33DNGT9&field1=";
//SoftwareSerial monitor(10, 11); // RX, TX
SoftwareSerial esp8266(2,3);
int temp = 0;
void setup()
{
  Serial.begin(9600);
  esp8266.begin(9600);
  //sensors.begin();
  sendDebug("AT");
  delay(5000);
  
  
  if(esp8266.find("OK")){
    Serial.println("RECEIVED: OK");
    connectWiFi();
  }
}

void loop(){
  /**
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  tempC = DallasTemperature::toFahrenheit(tempC);
  char buffer[10];
  String tempF = dtostrf(tempC, 4, 1, buffer);
  updateTemp(tempF);
  **/
  
  updateTemp( String(temp++));
  delay(30000);
}

void updateTemp(String tenmpF){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  sendDebug(cmd);
  delay(2000);
  if(esp8266.find("Error")){
    Serial.print("RECEIVED: Error");
    return;
  }
  cmd = GET;
  cmd += tenmpF;
  cmd += "\r\n";
  String request = "AT+CIPSEND=" + String(cmd.length());
  sendDebug(request);
  
  if(esp8266.find(">")){
    Serial.print(">");
    Serial.print(cmd);
    //esp8266.println(cmd);
    sendDebug(cmd);
  }else{
    sendDebug("AT+CIPCLOSE");
  }
  if(esp8266.find("OK")){
    Serial.println("RECEIVED: OK");
  }else{
    Serial.println("RECEIVED: Error");
  }
}
void sendDebug(String cmd){
  Serial.print("SEND: ");
  Serial.println(cmd);
  esp8266.println(cmd);
} 
 
boolean connectWiFi(){
  esp8266.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  
  sendDebug(cmd);
  delay(5000);
    
  if(esp8266.find("OK")){
    Serial.println("RECEIVED: OK");
    return true;
  }else{
    Serial.println("RECEIVED: Error");
    return false;
  }
}
