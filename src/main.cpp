#include <Arduino.h>

constexpr uint8_t REL_PIN = 6;
constexpr uint8_t INPUT_PIN = 7;

constexpr uint32_t RELAY_PERIOD_US = 1000000;
constexpr unsigned long RELAY_DEBOUNCE_US = 5000;

volatile unsigned long lastRelayInputChangeUs = 0;
volatile bool isRelayInputOn = false;

uint32_t lastRelayChangeUs = 0;
bool isRelayOn = false;

IRAM_ATTR void handleRelayChange()
{
  isRelayInputOn = true;
  lastRelayInputChangeUs = micros();
}

void setup()
{
  Serial.begin(115200);

  pinMode(REL_PIN, OUTPUT);
  pinMode(INPUT_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(INPUT_PIN), handleRelayChange, FALLING);
  digitalWrite(REL_PIN, HIGH);
}

void loop()
{
  unsigned long now = micros();

  if (isRelayInputOn && now > lastRelayInputChangeUs && now - lastRelayInputChangeUs > RELAY_DEBOUNCE_US)
  {

    Serial.printf("Relay timeout = %lu us \n", lastRelayInputChangeUs - lastRelayChangeUs);
    isRelayInputOn = false;
  }

  if (now - lastRelayChangeUs > RELAY_PERIOD_US)
  {
    isRelayOn = !isRelayOn;
    digitalWrite(REL_PIN, isRelayOn ? LOW : HIGH);
    lastRelayChangeUs = now;
    Serial.println(isRelayOn ? "Relay ON" : "Relay OFF");
  }
}
