#include <WiFi.h>
const char* ssid = "My Zone";
const char* password = "sreevani";

WiFiServer server(80); 
#define LED2  2    
String ledstatus = "";
int wait30 = 30000; 

void setup() {
  Serial.begin(115200);
  pinMode(LED2, OUTPUT);
  Serial.println();
  Serial.print("Connecting with ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected with WiFi.");
  server.begin();
  Serial.println("Web Server started.");
  Serial.print("This is IP to connect to the WebServer: ");
  Serial.print("http://");
  Serial.println(WiFi.localIP());
}
 
void loop() {
  if ((WiFi.status() != WL_CONNECTED) && (millis() > wait30)) {
    Serial.println("Trying to reconnect WiFi...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    wait30 = millis() + 30000;
  } 

  WiFiClient client = server.available();
  if (!client) {
    return;
  }
   
  Serial.print("New client: ");
  Serial.println(client.remoteIP());

  String req = client.readStringUntil('\r');
  Serial.println(req);

       if (req.indexOf("on2") != -1) {digitalWrite(LED2, HIGH); ledstatus = "ON";}
       if (req.indexOf("off2") != -1){digitalWrite(LED2, LOW); ledstatus = "OFF";}
     if (req.indexOf("check") != -1){
         if (digitalRead(LED2)){ledstatus = "LED2 now is ON";}
         else {ledstatus = "LED2 now is OFF";}
          }
     
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  Important.
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><meta charset=utf-8></head>");
  client.println("<body><center><font face='Arial'>");
  client.println("<h1>task - 12</h1>");
  client.println("<h2><font color='#009900'>MY ESP32 LED SWITCH SERVER</font></h2>");
  client.println("<br><br>");
  client.println("<a href='on2'><button>Click to ON LED2</button></a>");
  client.println("<a href='off2'><button>Click to OFF LED2</button></a>");
  client.println("<a href='check'><button>Consult status LED2</button></a>");
  client.println("<br><br>");
  client.println(ledstatus);
  client.println("</font></center></body></html>");

  Serial.print("Client disconnected: ");
  Serial.println(client.remoteIP());
  client.flush();
  client.stop();
}
