#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <SD.h>

const char* ssid = "YOUR_SSID";//Wifi Id
const char* password = "YOUR_PASSWORD";//Wifi password
byte buff[600000];//Buffer storage
StaticJsonBuffer<600000> jsonBuffer;
String payload;
void setup() {

  Serial.begin(115200);

  WiFi.begin(ssid, password);//Initializing Wifi

  if (!SD.begin(4))// CS_PIN is 4
  {
    Serial.println("Unable to connect to SD Card");
    //program ends
    return;
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wifi...");
  }
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;  //Object of class HTTPClient
    http.begin("http://demo4657392.mockable.io/list-tag-ids");//URL for the API file
    int httpCode = http.GET();//Check the returning code
    if (httpCode > 0) {
      // Get the request response payload
      payload = http.getString();
      if (httpCode == HTTP_CODE_OK) {
        //Writing to file
        File dataFile = SD.open("demo.txt", FILE_WRITE);
        payload.toCharArray((char*)buff, 600000);
        dataFile.print((char*)buff);
      }
    }
    //Reading from the file
    if (payload) {
      File dataFile = SD.open("demo.txt", FILE_READ);
      dataFile.read(buff, 600000);
      payload = String((char*)buff);
    }
  }
}

void loop() {
  
  JsonObject& root = jsonBuffer.parseObject(payload);//parse the object
  Serial.print("Enter API key or type EXIT to stop");
  String apiKey = Serial.readString();//Read the monitor input
  if((apiKey=="exit")||(apiKey=="EXIT")||(apiKey=="Exit"))
  {
    Serial.println("EXITING.....");
    return;
    }
  int start = micros();
  //searching in json
    if (!root.success()) {
    Serial.println("parseObject() failed");
       continue;
  }
  else{
for (JsonObject::iterator it=root.begin(); it!=root.end(); ++it) {
    Serial.println(it->key);
}
  }
  int end = micros();
  Serial.println("Time for query:" + String((end - start) * 1000) + "msec");


}
