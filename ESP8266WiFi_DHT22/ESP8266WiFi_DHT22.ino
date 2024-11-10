#include <DHT.h>
#include <ESP8266WiFi.h>
 
// replace with your channel's thingspeak API key, 
String apiKey1 = "O4C1E....";
String apiKey2 = "QM5M1X...";
const char* ssid = "HUAWEI.....";
const char* password = "...";
 
const char* server = "api.thingspeak.com";
#define DHTPIN1 0 // D3 pin on Nodemcu
#define DHTPIN2 4 // D2 pin on Nodemcu
 
DHT dht1(DHTPIN1, DHT22, 11);
DHT dht2(DHTPIN2, DHT22, 11);

WiFiClient client;

int counter=0;

void setup() {                
  Serial.begin(115200);
  delay(10);
  dht1.begin();
  dht2.begin();
  
  WiFi.begin(ssid, password);
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
   
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("connecting.....");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
}
  
void loop() {

  counter++;
  
  float h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature(true);
  float TC1 = (t1-32)*.5556;  // convert F to C

  float h2 = dht2.readHumidity();
  float t2 = dht2.readTemperature(true);
  float TC2 = (t2-32)*.5556;  // convert F to C
  
  if (isnan(h1) || isnan(TC1)) {
    Serial.println("Failed to read from DHT sensor 1!");
    return;
  }

  if (isnan(h2) || isnan(TC2)) {
    Serial.println("Failed to read from DHT sensor 2!");
    return;
  }
  if(counter==1){
  if (client.connect(server,80)) {  
    String postStr1 = apiKey1;
           postStr1 +="&field1=";
           postStr1 += String((int)TC1);
           postStr1 +="&field2=";
           postStr1 += String((int)h1);
 
     client.print("POST /update HTTP/1.1\n"); 
     client.print("Host: api.thingspeak.com\n"); 
     client.print("Connection: close\n"); 
     client.print("X-THINGSPEAKAPIKEY: "+apiKey1+"\n"); 
     client.print("Content-Type: application/x-www-form-urlencoded\n"); 
     client.print("Content-Length: "); 
     client.print(postStr1.length()); 
     client.print("\n\n"); 
     client.print(postStr1);
           
     Serial.print("Temperature1: ");
     Serial.print(TC1);
     Serial.print(" degrees Celcius Humidity1: "); 
     Serial.print(h1);
     Serial.println("% send to Thingspeak");
     Serial.print("Counter:    ");
     Serial.println(counter);   
    }
  }
  if(counter==50000){
  if (client.connect(server,80)) {
  String postStr2 = apiKey2;
           postStr2 +="&field1=";
           postStr2 += String((int)TC2);
           postStr2 +="&field2=";
           postStr2 += String((int)h2);
 
     client.print("POST /update HTTP/1.1\n"); 
     client.print("Host: api.thingspeak.com\n"); 
     client.print("Connection: close\n"); 
     client.print("X-THINGSPEAKAPIKEY: "+apiKey2+"\n"); 
     client.print("Content-Type: application/x-www-form-urlencoded\n"); 
     client.print("Content-Length: "); 
     client.print(postStr2.length()); 
     client.print("\n\n"); 
     client.print(postStr2);
           
     Serial.print("Temperature2: ");
     Serial.print(TC2);
     Serial.print(" degrees Celcius Humidity2: "); 
     Serial.print(h2);
     Serial.println("% send to Thingspeak");
     Serial.print("Counter:    ");
     Serial.println(counter);
   }
  }
  client.stop();
   
  if(counter==2500000){
    Serial.print("Counter:    ");
    Serial.println(counter);
    counter=0;
    }
 }
 
