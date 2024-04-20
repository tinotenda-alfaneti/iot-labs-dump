/** 
 *  No AJAX
 *  This extends Hello Server and adds an HTTPCLient to 
 *  make requests to a remote service.
 *  This nodes must connect to the network in STA mode.
 *  (not AP mode) 
 *  
 *  Connect as client by visiting local_ipaddress/client
 */


#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>  //because of HTTPClient use


const char* ssid = "MuAtarist";
const char* password = "123456ix";

WebServer server(80);

const int led = 19;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp32!");
  digitalWrite(led, 0);
}

void handleNathan() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "nathan here");
  //digitalWrite(led, 0);
}

//this returns HTML
void handleYaw() {
  digitalWrite(led, 1);
  server.send(200, "text/html", "<html><body> <h1>Nana Adwoa </h1> </body> </html>");
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {    //multicast DNS service
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/nathan", handleNathan);
  server.on("/yaw", handleYaw);  
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);
  server.on("/client", doGETRequest);
  // server.on("/client2", getVal);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}
       
String serverName="http://192.168.55.179";  //need the http!
void doGETRequest(){
  //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;      
      String serverPath = serverName +"/index.html";
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());  //alt use char host[] = "example.com";
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
        server.send(200, "text/html", payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
}
//=====
/** //ignore the ff. Need to do all HTTP protocol
WiFiClient client;
char url[] = "/index.html";
char host[] = "example.com";
String data;

void getVal(){
  
  Serial.print("sourcing ");
  client.connect(host, 80);
  client.println(String("GET ") + url);
  client.println(String("Host: ") + host);
  client.println("User-Agent: ESP8266/0.1");
  client.println("Connection: close");
  client.println();
  client.flush();
  delay(100);
  while(client.connected())
  {
    if(client.available())
    {
      data = client.readStringUntil('\n'); 
      Serial.println(data);
      //do something.
     
       
        client.stop();
        delay(100);
    }
  }
}
*/
