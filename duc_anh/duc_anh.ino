#define BLYNK_TEMPLATE_ID "TMPxxxxxx"
#define BLYNK_TEMPLATE_NAME "Device"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

char auth[] = "pgei_HIh1qkvujjU5qLln7Pce5Os92El";
char ssid[] = "Wifi love 2";
char pass[] = "88888888";

#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define SWITCH_PIN 4

LiquidCrystal_I2C lcd(0x27, 16, 2);

BlynkTimer timer;

int quantity = 0;
bool lastSwitchState = HIGH;

void sendSensorData() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  bool currentSwitch = digitalRead(SWITCH_PIN);

  if (!isnan(temp) && !isnan(hum)) {
    Blynk.virtualWrite(V0, temp);
    Blynk.virtualWrite(V1, hum);
  }
  Blynk.virtualWrite(V2, currentSwitch);

  if (lastSwitchState == HIGH && currentSwitch == LOW) {
    if (quantity > 0) {
      quantity--;
      Blynk.virtualWrite(V3, quantity);
    }
  }
  lastSwitchState = currentSwitch;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp);
  lcd.print("C H:");
  lcd.print(hum);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("SL:");
  lcd.print(quantity);
  lcd.print(" Sw:");
  lcd.print(currentSwitch ? "1" : "0");
}

BLYNK_WRITE(V3) {
  int val = param.asInt();
  if (val >= 0) {
    quantity = val;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  dht.begin();
 Blynk.begin(auth, ssid, pass, "blynk.dke.vn", 8888);
  timer.setInterval(1000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
}
