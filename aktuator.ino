#include <WiFi.h> // library WiFi 
#include <Wire.h> // Library Wire 
#include <PubSubClient.h> // Library MQTT

#define LED 2 // PIN LED bulit in pada board esp32

const char* ssid     = ""; // Konfigurasi SSID WiFi yang digunakan
const char* password = ""; // Konfigurasi Password WIF yang digunakan

const char* mqtt_server = ""; // Konfigurasi Server MQTT Broker
const char* topik = ""; // KOnfigurasi Topik MQTT
String AccessKey = ""; // Username MQTT
String AccessToken = ""; // Password MQTT
String clientId = ""; // Client ID MQTT
String pesan = ""; // variable untuk pesan yang diterima

WiFiClient espClient; // Inisiasi WiFi
PubSubClient client(espClient); // Inisiasi MQTT

void setup() {
  Serial.print("Connecting to "); // Mengkoneksikan Device ke WiFi
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, 1883); // Mengkoneksikan Device ke broker MQTT
  client.setCallback(callback);
  Serial.println("");

  pinMode(LED, OUTPUT);// deklarasi pin LED sebagai output 
  digitalWrite(LED, LOW); // matikan LED
}

void loop() {
  if (!client.connected()) { // jika esp32 terkoneksi dengan mqtt 
    reconnect(); // lakukan fungsi reconnect
  }
  client.loop(); 

}

void callback(char* topic, byte* payload, unsigned int length) {
  for (int i = 0; i < length; i++) {
    pesan += ((char)payload[i]);
  }
  Serial.println(pesan);
  if ( pesan == "0") // jika pesan yang diterima = 0 
  {
    digitalWrite(LED, LOW); // matikan LED 
  }
  else if (pesan == "1") // jika pesan yang diterima = 1
  {
    digitalWrite(LED, HIGH); // nyalakan LED
  }
  pesan = ""; // kosongkan isi variabel pesan 

}

void reconnect() { // fungsi recconect mqtt
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientId.c_str(), AccessKey.c_str(), AccessToken.c_str())) {
      Serial.println("connected");
      client.publish(topik, "hello world");
      client.subscribe(topik);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
