#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define DHTPIN D4      // What digital pin we're connected to
#define DHTTYPE DHT11  // DHT 11
#define soilPin D2     // Use digital pin D2 for soil moisture sensor
int pompaPin = 14;

char auth[] = "";  // You should get Auth Token in the Blynk App.

// Your WiFi credentials.
char ssid[] = "";
char pass[] = "";

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// Initialize a NTPClient to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// Fungsi callback untuk pin V0
BLYNK_WRITE(V0) {
  int pinValue = param.asInt();  // Menyimpan nilai pin

  // Jika button pada pin V0 ditekan (nilai pin menjadi 1), panggil fungsi sendSensor
  if (pinValue == 1) {
    sendSensor();
  }
}

// Fungsi callback untuk pin V1
BLYNK_WRITE(V1) {
  int pinValue = param.asInt();  // Menyimpan nilai pin

  // Jika button pada pin V1 ditekan (nilai pin menjadi 1), aktifkan pompa
  if (pinValue == 1) {
    digitalWrite(pompaPin, HIGH);  // Mengaktifkan pompa
  } else {
    digitalWrite(pompaPin, LOW);  // Menonaktifkan pompa
  }
}

void sendSensor() {
  int soil = digitalRead(soilPin);  // Membaca data sensor kelembaban tanah
  float h = dht.readHumidity();     // Membaca data sensor kelembaban udara
  float t = dht.readTemperature();  // Membaca data sensor suhu

  // Mencetak data sensor ke Serial Monitor
  Serial.print("Soil Moisture: ");
  Serial.println(soil);
  Serial.print("Humidity: ");
  Serial.println(h);
  Serial.print("Temperature: ");
  Serial.println(t);


  if (isnan(h) || isnan(t)) {
    Blynk.virtualWrite(V3, "Can't read sensor!");  // Menampilkan teks "Can't read sensor!" di pin V3 jika data sensor tidak valid
  } else {
    Blynk.virtualWrite(V5, h);                // Mengirim data kelembaban udara ke pin V5
    Blynk.virtualWrite(V6, t);                // Mengirim data suhu ke pin V6
    Blynk.virtualWrite(V7, soil);             // Mengirim data kelembaban tanah ke pin V7
    Blynk.virtualWrite(V3, "Data Updated!");  // Menampilkan teks "Data Updated" di pin V3 jika data sensor berhasil diperbarui
  }
}


// Function to display time on Value Display widget
void displayTime() {
  timeClient.update();

  // Get hours and minutes
  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();

  // Create a string in the format "HH:MM"
  String time = (hours < 10 ? "0" : "") + String(hours) + ":" + (minutes < 10 ? "0" : "") + String(minutes);

  // Send the time to Value Display widget on virtual pin V3
  Blynk.virtualWrite(V4, time);
}

// Function to display countdown to next watering on Value Display widget
void displayCountdown() {
  timeClient.update();

  // Get current time in minutes
  int currentMinutes = timeClient.getHours() * 60 + timeClient.getMinutes();

  // Calculate time to next watering in minutes (6AM is 360 minutes past midnight, 6PM is 1080 minutes past midnight)
  int nextWatering = (currentMinutes < 360) ? 360 : (currentMinutes < 1080) ? 1080
                                                                            : 360 + 24 * 60;
  int countdown = nextWatering - currentMinutes;
  if (countdown < 0) countdown += 24 * 60;  // If it's past 6PM, countdown to 6AM next day

  // Convert countdown to hours and minutes
  int hours = countdown / 60;
  int minutes = countdown % 60;

  // Create a string in the format "x h, xx m"
  String timeCountdown = String(hours) + " h, " + (minutes < 10 ? "0" : "") + String(minutes) + " m";

  // Send the countdown to Value Display widget on virtual pin V3
  Blynk.virtualWrite(V8, timeCountdown);
}

void setup() {
  // Debug console
  Serial.begin(115200);

  pinMode(pompaPin, OUTPUT);
  digitalWrite(pompaPin, LOW);  // Ensure the pump is off when the microcontroller powers on or gets a program update

  pinMode(soilPin, INPUT);  // Set soil moisture sensor pin as input

  WiFi.begin(ssid, pass);
  // check if we are connected to WiFi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Blynk.begin(auth, ssid, pass);
  dht.begin();

  timeClient.begin();
  timeClient.setTimeOffset(25200);  // Set timezone to WIB (UTC+7)

  // Set timer to call displayTime function every minute
  timer.setInterval(15L * 1000L, displayTime);

  // Set timer to call displayCountdown function every minute
  timer.setInterval(15L * 1000L, displayCountdown);
}

void loop() {
  Blynk.run();
  timer.run();
}
