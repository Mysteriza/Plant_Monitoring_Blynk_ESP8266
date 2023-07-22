#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

#define DHTPIN D1          // What digital pin we're connected to
#define DHTTYPE DHT11     // DHT 11
#define sensorPin A0
#define pompaPin 14

char auth[] = "";  // You should get Auth Token in the Blynk App.

// Your WiFi credentials.
char ssid[] = "";
char pass[] = "";

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
BlynkTimer clearTerminalTimer;

int soil;
WidgetTerminal terminal(V3); // Set the Terminal Widget to Virtual Pin V3

// This function will be called every time a Widget
// in Blynk app writes value to the Virtual Pin V1
BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V1 to a variable

  // Turn the pump on if 1, off if 0
  if (pinValue == 1) {
    digitalWrite(pompaPin, HIGH);
  } else {
    digitalWrite(pompaPin, LOW);
  }
}

void sendSensor()
{
  soil = analogRead(sensorPin);
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  // Print the sensor readings to the Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C\t");
  Serial.print("Soil Moisture: ");
  Serial.println(soil);

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (!isnan(t) && !isnan(h)) {
    Blynk.virtualWrite(V5, h);
    Blynk.virtualWrite(V6, t);
    Blynk.virtualWrite(V7, soil);
  } else {
    terminal.println("Failed to read from DHT sensor!");
    terminal.flush();
  }
}

BLYNK_WRITE(V0)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable

  // process received value
  if(pinValue == 1) {
    sendSensor();
    terminal.println("Data Updated!");
    terminal.flush();
    clearTerminalTimer.setTimeout(5000L, []() {
      terminal.clear();
      terminal.flush();
    });
  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  pinMode(pompaPin, OUTPUT);
  digitalWrite(pompaPin, LOW); // Ensure the pump is off when the microcontroller powers on or gets a program update
  
  WiFi.begin(ssid, pass);
  // check if we are connected to WiFi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Blynk.begin(auth, ssid, pass);
  dht.begin();

  // Setup a function to be called every second
  // timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
  clearTerminalTimer.run();
}
