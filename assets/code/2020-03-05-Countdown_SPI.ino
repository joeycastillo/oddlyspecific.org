#include <Wire.h>
#include <SPI.h>
#include <RTClib.h>

RTC_DS3231 rtc;

long deadline = 0;

byte Digits[] = {
  B1111110, // 0
  B0110000, // 1
  B1101101, // 2
  B1111001, // 3
  B0110011, // 4
  B1011011, // 5
  B1011111, // 6
  B1110000, // 7
  B1111111, // 8
  B1111011, // 9
};

const int PIN_DISPLAY_LATCH = 16;

void setup() {
  rtc.begin();
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(__DATE__, __TIME__));
    write_i2c_register(DS3231_ADDRESS, 0x08, packValue(21)); // 2021
    write_i2c_register(DS3231_ADDRESS, 0x09, packValue(1));
    write_i2c_register(DS3231_ADDRESS, 0x0A, packValue(1));
  }
  deadline = JulianDate(2000 + unpackValue(read_i2c_register(DS3231_ADDRESS, 0x08)), 
  unpackValue(read_i2c_register(DS3231_ADDRESS, 0x09)), 
  unpackValue(read_i2c_register(DS3231_ADDRESS, 0x0A)));
  pinMode(PIN_DISPLAY_LATCH, OUTPUT);
  SPI.begin();
}

void loop() {
  DateTime now = rtc.now();
  long julianNow = JulianDate(now.year(), now.month(), now.day());
  long remaining = deadline - julianNow - 1;
  Serial.println(deadline);
  Serial.println(julianNow);
  Serial.println(remaining);
  Serial.println();
  unsigned char ones = remaining % 10;
  unsigned char tens = (remaining / 10) % 10;
  unsigned char hundreds = (remaining / 100) % 10;
  
  if (remaining < 0) {
    // if date is passed, display " - "
    display(0, 0b00000001, 0);
  } else {
    display(Digits[hundreds], Digits[tens], Digits[ones]);
  }
}

void write_i2c_register(uint8_t addr, uint8_t reg, uint8_t val) {
  Wire.beginTransmission(addr);
  Wire.write((byte)reg);
  Wire.write((byte)val);
  Wire.endTransmission();
}

uint8_t read_i2c_register(uint8_t addr, uint8_t reg) {
  Wire.beginTransmission(addr);
  Wire.write((byte)reg);
  Wire.endTransmission();
  Wire.requestFrom(addr, (byte)1);
  return Wire.read();
}

uint8_t packValue(uint8_t value) {
  uint8_t ones = value % 10;
  uint8_t tens = (value / 10) % 10;
  return (tens << 4) | ones;
}

uint8_t unpackValue(uint8_t packed) {
  uint8_t ones = packed & 0xF;
  uint8_t tens = (packed >> 4) & 0xF;
  return tens * 10 + ones;
}

long JulianDate(int year, int month, int day) {
  return 367 * year - (7 * (year + 5001 + (month - 9) / 7)) / 4 + (275 * month) / 9 + day + 1729777;
}

void display(byte b1, byte b2, byte b3) {
  SPI.beginTransaction(SPISettings(100000, LSBFIRST, SPI_MODE0));
  digitalWrite(PIN_DISPLAY_LATCH, LOW);
  SPI.transfer(b1);
  SPI.transfer(b2);
  SPI.transfer(b3);
  digitalWrite(PIN_DISPLAY_LATCH, HIGH);
  SPI.endTransaction();
}
