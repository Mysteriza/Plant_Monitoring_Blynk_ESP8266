# Smart Plant Monitoring ESP8266 + Blynk New (Free Version)
An Arduino IoT program connected to Blynk to monitor plants and perform watering through the application.

## Feature
1. Monitoring soil moisture
2. Monitoring temperature & humidity
3. Watering plants with just one button
4. 'Update button' in app to refresh sensor data. I added this feature to prevent the microcontroller from continuously reading and sending data to Blynk, thus conserving battery life. This is useful if you plan on using batteries. But in this case, I directly connect the microcontroller to the AC current so there is no need to recharge the battery. But of course, you need to tap this button everytime you want to get the latest data.
5. Terminal, I use this to see whether the program responds correctly when we press the 'update button'. If there are no issues, the terminal will display a message saying 'Data Updated!'.

## What you need
1. ESP8266
2. DHT11/DHT22 (I use DHT11)
3. Relay 1 Channel
4. Motor DC 3.3v
5. Capacitive Soil Moisture Sensor
6. PCB 5x7 cm double layer (opsional, I use this to simplify the assembly of components)

## Blynk App
<img src="https://github.com/Mysteriza/Smart-Plant-Monitoring/assets/76644526/a308da5e-0765-4ec0-bdb5-528d33a6930d" width="300">
<img src="https://github.com/Mysteriza/Smart-Plant-Monitoring/assets/76644526/96dc3a24-ba70-4e78-a062-4201003882f5" width="300">

### Datastream:
1. Water Button = V1
2. Update/Refresh Button = V0
3. Temperature = V6
4. Humidity = V5
5. Soil Moisture = V7
6. Terminal = V3

## Schematic
I don't have time to make a schematic, so maybe you can check inside the Arduino program what needs to be connected to each pin.

# Any suggestions? Write it down in Discussion!
