/* ====== ESP8266 Demo ======
 *   Print out analog values
 * ==========================
 *
 * Change SSID and PASS to match your WiFi settings.
 * The IP address is displayed to soft serial upon successful connection.
 *
 * Ray Wang @ Rayshobby LLC
 * http://rayshobby.net/?p=9734
 */

// comment this part out if not using LCD debug
#include <SoftwareSerial.h>
//SoftwareSerial dbg(7, 8); // using pin 7, 8 for software serial
SoftwareSerial esp8266(2,3);

enum {WIFI_ERROR_NONE=0, WIFI_ERROR_AT, WIFI_ERROR_RST, WIFI_ERROR_SSIDPWD, WIFI_ERROR_SERVER, WIFI_ERROR_UNKNOWN};

#define BUFFER_SIZE 128

#define SSID  "KV_2"   // change this to match your WiFi SSID
#define PASS  "vlta??????????????"  // change this to match your WiFi password
#define PORT  "8080"      // using port 8080 by default

char buffer[BUFFER_SIZE];

void setup() {

  Serial.begin(9600);
  Serial.setTimeout(5000);
  esp8266.begin(9600); // your esp's baud rate might be different
  
  Serial.begin(9600);
  Serial.println("begin.");
    
  byte err = setupWiFi();
  if (err) {
    // error, print error code
    Serial.print("setup error:");
    Serial.println((int)err);
  } else {
    // success, print IP
    Serial.print("ip addr:");
    char *ip = getIP();
    if (ip) {
      Serial.println(ip);
    }
    else {
      Serial.println("none");
    }
    maxTimeout();
  }
}

bool maxTimeout() {
  // send AT command
  esp8266.println("AT+CIPSTO=0");
  if(esp8266.find("OK")) {
    return true;
  } else {
    Serial.println("maxtimeout=false.");
    return false;
  }
}

char* getIP() {
  // send AT command
  esp8266.println("AT+CIFSR");

  // the response from the module is:
  // AT+CIFSR\n\n
  // 192.168.x.x\n 
  // so read util \n three times
  esp8266.readBytesUntil('\n', buffer, BUFFER_SIZE);  
  esp8266.readBytesUntil('\n', buffer, BUFFER_SIZE);  
  esp8266.readBytesUntil('\n', buffer, BUFFER_SIZE);  
  buffer[strlen(buffer)-1]=0;
  return buffer;
}

void loop() {
  Serial.println("loop.");
  int ch_id, packet_len;
  char *pb;  
  esp8266.readBytesUntil('\n', buffer, BUFFER_SIZE);
  if(strncmp(buffer, "+IPD,", 5)==0) {
    // request: +IPD,ch,len:data
    sscanf(buffer+5, "%d,%d", &ch_id, &packet_len);
    if (packet_len > 0) {
      // read serial until packet_len character received
      // start from :
      pb = buffer+5;
      while(*pb!=':') pb++;
      pb++;
      if (strncmp(pb, "GET /", 5) == 0) {
        serve_homepage(ch_id);
      }
    }
  }
}



void serve_homepage(int ch_id) {
  String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\nRefresh: 5\r\n";

  String content="";
  // output the value of each analog input pin
  for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
    int sensorReading = analogRead(analogChannel);
    content += "analog input ";
    content += analogChannel;
    content += " is ";
    content += sensorReading;
    content += "<br />\n";       
  }

  header += "Content-Length:";
  header += (int)(content.length());
  header += "\r\n\r\n";
  esp8266.print("AT+CIPSEND=");
  esp8266.print(ch_id);
  esp8266.print(",");
  esp8266.println(header.length()+content.length());
  if (esp8266.find(">")) {
    esp8266.print(header);
    esp8266.print(content);
    delay(20);
  }
  /*Serial.print("AT+CIPCLOSE=");
  Serial.println(ch_id);*/
}

byte setupWiFi() {
    Serial.println("1.");
  esp8266.println("AT");
  if(!esp8266.find("OK")) {
    return WIFI_ERROR_AT;
  }
  delay(500); 
  Serial.println("2.");
  // reset WiFi module
  esp8266.println("AT+RST");
  if(!esp8266.find("OK")) {
    return WIFI_ERROR_RST;
  }
  delay(500);
 
  // set mode 3
  Serial.println("3.");
   esp8266.print("AT+CWJAP=\"KV_2\",\"vltavsky\"");
   /**
  esp8266.print("AT+CWJAP=\"");
  esp8266.print(SSID);
  esp8266.print("\",\"");
  esp8266.print(PASS);
  esp8266.println("\"");
  **/
   delay(7000);
  
  
  if(!esp8266.find("OK")) {
   // return WIFI_ERROR_SSIDPWD;
  }
  
    Serial.println("4.");
  delay(500);
  
  // start server
  esp8266.println("AT+CIPMUX=1");
  if(!esp8266.find("OK")){
    return WIFI_ERROR_SERVER;
  }
  delay(500);
  
  esp8266.print("AT+CIPSERVER=1,"); // turn on TCP service
  esp8266.println(PORT);
  if(!esp8266.find("OK")){
    return WIFI_ERROR_SERVER;
  }
  delay(500);
  
  return WIFI_ERROR_NONE;
}
