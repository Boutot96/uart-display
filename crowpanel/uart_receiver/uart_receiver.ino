#include <Wire.h>
#include <Arduino_GFX_Library.h>
#include <PCF8574.h>

#define I2C_SDA_PIN 38
#define I2C_SCL_PIN 39
#define BKL_PIN 6

PCF8574 pcf8574(0x21);

Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
  16, 2, 1,
  40, 7, 15, 41,
  46, 3, 8, 18, 17,
  14, 13, 12, 11, 10, 9,
  5, 45, 48, 47, 21
);

Arduino_ST7701_RGBPanel *gfx = new Arduino_ST7701_RGBPanel(
  bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */,
  false /* IPS */, 480 /* width */, 480 /* height */,
  st7701_type5_init_operations, sizeof(st7701_type5_init_operations),
  true /* BGR */,
  10 /* hsync_front_porch(10) */, 4 /* hsync_pulse_width(8) */, 20 /* hsync_back_porch(50) */,
  10 /* vsync_front_porch(10) */, 4 /* vsync_pulse_width(8) */, 20 /* vsync_back_porch(20) */);

String packet;
String last_dist = "--";

void drawDistance(String dist) {
  gfx->fillScreen(BLACK);
  gfx->setTextColor(WHITE);
  gfx->setTextSize(5);
  gfx->setCursor(120, 210);
  gfx->print(dist + " cm");
}

void initPins() {
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  pcf8574.pinMode(P0, OUTPUT);
  pcf8574.pinMode(P2, OUTPUT);
  pcf8574.pinMode(P3, OUTPUT);
  pcf8574.pinMode(P4, OUTPUT);
  if (!pcf8574.begin()) {
    Serial.println("Can't init pcf8574");
  }
  pcf8574.digitalWrite(P3, HIGH);
  delay(100);
  pcf8574.digitalWrite(P4, HIGH);
  delay(100);
  pcf8574.digitalWrite(P4, LOW);
  delay(120);
  pcf8574.digitalWrite(P4, HIGH);
  delay(120);
  pcf8574.digitalWrite(P0, HIGH);
  delay(100);
  pcf8574.digitalWrite(P0, LOW);
  delay(120);
  pcf8574.digitalWrite(P0, HIGH);
  delay(120);
  pcf8574.digitalWrite(P2, HIGH);
  delay(120);
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 44, 43);
  initPins();
  pinMode(BKL_PIN, OUTPUT);
  analogWrite(BKL_PIN, 200);
  gfx->begin();
  drawDistance("--");
  Serial.println("Setup done");
}

void loop() {
  if (Serial1.available()) {
    packet = Serial1.readStringUntil('\n');
    packet.trim();
    if (packet.startsWith("<DIST:") && packet.endsWith(">")) {
      last_dist = packet.substring(6, packet.length() - 1);
      drawDistance(last_dist);
    }
  }
  delay(5);
}