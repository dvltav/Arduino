#include<stdlib.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#include <PCD8544.h>

//using namespace ArduinoJson::Parser;

#define SSID "KV_2"
#define PASS "vlta??????????????"
#define IP "184.106.153.149" // thingspeak.com
String GET = "GET /channels/22079/fields/1/last";
//String GET = "GET /channels/22079/feed.json?key=0RI7PCVZA33DNGT9&results=1";
SoftwareSerial esp8266(8,9);
StaticJsonBuffer<200> jsonBuffer;
static PCD8544 lcd;


void setup()
{
  Serial.begin(9600);

  lcd.begin(84, 48);
  
  esp8266.begin(9600);
  sendDebug("AT");
  delay(5000);
  
  
  if(esp8266.find("OK")){
    Serial.println("RECEIVED: OK");
    connectWiFi();
  }
}

void loop(){
  
  printTemp(readTemp());
  delay(30000);
  printTemp("--.-");
}

void printTemp(String temp) 
{
 
   // Write a piece of text on the first line...
  lcd.setCursor(0, 0);
  lcd.print("Temperature");
  
    // Write a piece of text on the first line...
  lcd.setCursor(25, 3);
  lcd.print(temp);
  
  lcd.setCursor(0, 5);
  lcd.print("            ");
  
}

String readTemp()
{
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  sendDebug(cmd);
  delay(2000);
  if(esp8266.find("Error")){
    Serial.print("RECEIVED: Error");
    return "s";
  }
  cmd = GET;
  cmd += "\r\n";
  String request = "AT+CIPSEND=" + String(cmd.length());
  sendDebug(request);
 
  if(esp8266.find(">")){
    Serial.print(">");
    //Serial.print(cmd);
    //esp8266.println(cmd);
    sendDebug(cmd);
  }else{
    sendDebug("AT+CIPCLOSE");
  }
  
  unsigned int i = 0; //timeout counter
  int n = 0; // char counter
  char json[200];
  boolean startReading = false;
   Serial.println("while loop....");
 
  
 while(i <60000) {   
    if(esp8266.available()) {
      
      char c = esp8266.read();
      
      
      if (c == ':'){
        Serial.println("MATCH");
        startReading = true;
      }
      
      if (startReading && c!=':' && n <= 6) 
      {
        json[n++]=c;
     // Serial.print(".");
     // Serial.print(c);
      }
      
    }
    i++;
 }//while
    
  
   String tempJson(json);
   tempJson = tempJson.substring(0,4);
  Serial.println("temp = " + tempJson);
  return tempJson;
  
  /**
  JsonObject& root = jsonBuffer.parseObject(json);

  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  
  float temp = root["filed1"];
  Serial.print("---Temp= " );
  Serial.print(temp);

 **/
  
  /**
  if(esp8266.find("OK")){
    Serial.println("RECEIVED: OK");
  }else{
    Serial.println("RECEIVED: Error");
  } **/
}

void sendDebug(String cmd){
  Serial.print("SEND: ");
  Serial.println(cmd);
  lcd.setCursor(0, 5);
  lcd.print(cmd.substring(0,12));
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
