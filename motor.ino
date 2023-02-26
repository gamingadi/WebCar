
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

uint8_t p[3] = {D0,D5,D6};
uint8_t p1[3] = {D1,D7,D8};

#ifndef APSSID
#define APSSID "Car"
#define APPSK  "Password"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;
const String page="<form action=\"/action\" method=\"post\" enctype=\"application/x-www-form-urlencoded\">\
        <label>Servo</label> <input type=\"number\" name=\"servo\" value=\"50\"/>\
        <br/>\
        <label>Bo</label> <input type=\"number\" name=\"bo\" value=\"1024\"/>\
        <button type=\"submit\">Save</button>\
    </form>"; 
//Declaration
ESP8266WebServer server(80);
int sToi(String str)
{ 
    int temp = 0;
    for (int i = 0; i < str.length(); i++) {
 
        // Since ASCII value of character from '0'
        // to '9' are contiguous. So if we subtract
        // '0' from ASCII value of a digit, we get
        // the integer value of the digit.
        int c = (str[i] - '0');
        if((c)>=0 && c<=9)
        temp = temp * 10 + (c);
    }
    return temp;
}

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot() {
  int value1 = 50;
  int value2 = 1024;
  String key = "servo";
  for(int i = 0;i<server.args();i++){
      key = server.argName(i);
      int value = sToi(server.arg(i)); 
      if(key == "servo"){
        value1 = value;
      }
      if(key == "bo"){
          value2 = value;      
      }
  }  
  analogWrite(p1[0],value1);
  analogWrite(p[0],value2);
  delay(1000);
  server.send(200, "text/html", page);
}
void action(){
  String m = server.arg(0);
  Serial.println("query : "+ server.arg(0));
  if(m == "F"){
    digitalWrite(p[1],HIGH);
    digitalWrite(p[2],LOW);
    server.send(200,"text/html","<h1>Move Forward</h1>");  
  }else if(m == "B"){
    digitalWrite(p[1],LOW);
    digitalWrite(p[2],HIGH);
    server.send(200,"text/html","<h1>Move Back</h1>");  
  }else if(m == "L"){
    digitalWrite(p1[1],HIGH);
    digitalWrite(p1[2],LOW);
    server.send(200,"text/html","<h1>Move Left</h1>");  
  }else if(m == "R"){
    digitalWrite(p1[1],LOW);
    digitalWrite(p1[2],HIGH);
    server.send(200,"text/html","<h1>RIGHT</h1>");  
  }else{
    digitalWrite(p[1],LOW);
    digitalWrite(p[2],LOW);
    digitalWrite(p1[1],LOW);
    digitalWrite(p1[2],LOW);
    server.send(200,"text/html","<h1>STOPED</h1>");
  }
}
void setup() {
  delay(1000);
  for(int i=0;i<3;i++) pinMode(p[i],OUTPUT);
  delay(1000);
  for(int i=0;i<3;i++) pinMode(p1[i],OUTPUT);
  delay(1000);
  analogWrite(p[0],1024);
  analogWrite(p1[0],80);
  // myservo.attach(D1,500,2400);
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/action", handleRoot);
  server.on("/move",action);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
