#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

#define DHTPIN D1      // What digital pin we're connected to
#define DHTTYPE DHT11  // DHT 11
#define soilPin D2     // Use digital pin D2 for soil moisture sensor
int pompaPin = 14;

char auth[] = "";  // You should get Auth Token in the Blynk App.

// Your WiFi credentials.
char ssid[] = "";
char pass[] = "";

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
BlynkTimer lcdTimer;
WidgetLCD lcd(V3);

void clearLcd() {
  lcd.clear();
}

void sendSensor() {
  int soil = digitalRead(soilPin);  // Use digitalRead for soil moisture sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();  // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    lcd.print(0, 0, "Can't read sensor!");
  } else {
    // Send data to corresponding virtual pins on Blynk app
    Blynk.virtualWrite(V5, h);     // Send humidity data to V5
    Blynk.virtualWrite(V6, t);     // Send temperature data to V6
    Blynk.virtualWrite(V7, soil);  // Send soil moisture data to V7
    lcd.print(0, 0, "Data Updated");
  }

  lcdTimer.setTimeout(5000L, clearLcd);  // Clear the LCD after 5 seconds
}

BLYNK_WRITE(V0) {
  int pinValue = param.asInt();  // Assigning incoming value from pin V0 to a variable

  // process received value
  if (pinValue == 1) {
    sendSensor();
  }
}

BLYNK_WRITE(V1) {
  int pinValue = param.asInt();  // Assigning incoming value from pin V1 to a variable

  // Turn the pump on if 1, off if 0
  if (pinValue == 1) {
    digitalWrite(pompaPin, HIGH);
  } else {
    digitalWrite(pompaPin, LOW);
  }
}

void checkConnection() {
  if (!Blynk.connected()) {
    Blynk.connect();  // attempt to reconnect to server
  }
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
}

void loop() {
  Blynk.run();
  timer.run();
  lcdTimer.run();
}
