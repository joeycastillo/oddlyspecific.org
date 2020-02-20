#include <Adafruit_SPIFlash.h>
#include <Adafruit_GPS.h>
#include <DHT.h>
#include <avr/dtostrf.h>

Adafruit_GPS GPS(&Serial1);
DHT dht(A1, DHT22);
Adafruit_FlashTransport_SPI flashTransport(SS1, &SPI1);
Adafruit_SPIFlash flash(&flashTransport);
FatFileSystem fatfs;

void setup() {
  dht.begin();

  flash.begin();
  fatfs.begin(&flash);

  GPS.begin(9600);
  // Turn on RMC (recommended data) and GGA (fix data), including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // Set the update rate to 1 Hz
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  // Turn off messages about antenna state
  GPS.sendCommand(PGCMD_NOANTENNA);
  // Enable satellite-based augmentation (SBAS)
  GPS.sendCommand("$PMTK313,1*2E\r\n");
  // Set differential GPS data source to WAAS
  GPS.sendCommand("$PMTK301,2*2E\r\n");
}

void loop() {
  /* GPS PARSING */
  GPS.read();
  if (GPS.newNMEAreceived()) {
    // try to parse the sentence, but if we can't, no worries, just try again on the next loop.
    if (!GPS.parse(GPS.lastNMEA())) return;
  }

  static bool shouldLog = true;
  int seconds = GPS.seconds;
  // Log data every minute, provided we have a valid timestamp
  if (shouldLog && seconds == 0 && GPS.year != 0) {
    String datapoint;
    char timestamp[20];
    sprintf(timestamp, "20%02d-%02d-%02dT%d:%02d:%02d", GPS.year, GPS.month, GPS.day, GPS.hour, GPS.minute, seconds);
    datapoint += timestamp;
    datapoint += ',';
    datapoint += (int)GPS.fix;
    datapoint += ',';
    if (GPS.fix) {
      char outstr[15];
      dtostrf(GPS.latitudeDegrees, 7, 7, outstr);
      datapoint += outstr;
      datapoint += ',';
      dtostrf(GPS.longitudeDegrees, 7, 7, outstr);
      datapoint += outstr;
      datapoint += ',';
      datapoint += GPS.HDOP;
      datapoint += ',';
      datapoint += GPS.speed * 1.852001;
      datapoint += ',';
      datapoint += GPS.angle;
      datapoint += ',';
      datapoint += GPS.altitude;
      datapoint += ',';
    }
    else
    {
      datapoint += ",,,,,,";
    }
    datapoint += (int)GPS.satellites;
    datapoint += ',';
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    
    if (isnan(t) || isnan(h)) {
      datapoint += ",,";
    }
    else {
      datapoint += t;
      datapoint += ',';
      datapoint += h;
      datapoint += ',';
    }
    float measuredvbat = analogRead(9);
    measuredvbat /= 1023;
    measuredvbat *= 3.3;
    measuredvbat *= 2;
    datapoint += (measuredvbat);
    
    File dataFile = fatfs.open("log.csv", FILE_WRITE);
    if (dataFile)
    {
      if (dataFile.size() == 0) {
        const char header[] = "Timestamp,GPS Fix,Latitude,Longitude,HDOP,Speed,Angle,Altitude,Satellites In View,Temperature (C),Relative Humidity (%),Battery Voltage";
        dataFile.println(header);
      }
      dataFile.println(datapoint);
      dataFile.close();
      digitalWrite(LED_BUILTIN, HIGH);
      delay(10);
      digitalWrite(LED_BUILTIN, LOW);
    }
    shouldLog = false;
  }
  else if (seconds == 1) {
    shouldLog = true;
  }
}
