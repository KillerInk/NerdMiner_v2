#include "displayDriver.h"

#ifdef NO_DISPLAY

#include <Arduino.h>
#include "monitor.h"
#include "wManager.h"

extern monitor_data mMonitor;
bool ledOn = false;

void noDisplay_Init(void)
{
  Serial.println("No display driver initialized");
  pinMode(LED_PIN, OUTPUT);
}

void noDisplay_AlternateScreenState(void)
{
  Serial.println("Switching display state");
  ledOn = !ledOn;
}

void noDisplay_AlternateRotation(void)
{
}

uint8_t showHeadercounter = 0;
void noDisplay_NoScreen(unsigned long mElapsed)
{
  mining_data data = getMiningData(mElapsed);
  if (showHeadercounter <= 0)
  {
    Serial.printf("|%13s|%13s|%13s|%13s|%13s|%13s|%13s|%13s|%13s|\n",
                  "Time",
                  "BlockTemplate",
                  "HashRate",
                  "BestDiff",
                  "Temperature",
                  "MHashes",
                  "KHashs",
                  "32bitShares",
                  "BlocksFound");
    showHeadercounter = 5;
  }
  showHeadercounter--;
  //              time       blocktemp         hash    bedif     temp        mhas
  Serial.printf("|%13s|%13s|%9sKH/s|%13s|%13s|%13s|%13s|%13s|%13s|\n",
                data.timeMining.c_str(),
                data.templates.c_str(),
                data.currentHashRate.c_str(),
                data.bestDiff.c_str(),
                data.temp.c_str(),
                data.totalMHashes.c_str(),
                data.totalKHashes.c_str(),
                data.completedShares.c_str(),
                data.valids.c_str());
}
void noDisplay_LoadingScreen(void)
{
  Serial.println("Initializing...");
}

void noDisplay_SetupScreen(void)
{
  Serial.println("Setup...");
}

// Variables para controlar el parpadeo con millis()
unsigned long previousMillis = 0;

void noDisplay_DoLedStuff(unsigned long frame)
{
  unsigned long currentMillis = millis();

  if (!ledOn)
  {
    digitalWrite(LED_PIN, INACTIVE_LED);
    return;
  }

  switch (mMonitor.NerdStatus)
  {

  case NM_waitingConfig:
    digitalWrite(LED_PIN, ACTIVE_LED); // LED encendido de forma continua
    break;

  case NM_Connecting:
    if (currentMillis - previousMillis >= 500)
    { // 0.5sec blink
      previousMillis = currentMillis;
      digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // Cambia el estado del LED
    }
    break;

  case NM_hashing:
    if (currentMillis - previousMillis >= 100)
    { // 0.1sec blink
      previousMillis = currentMillis;
      digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // Cambia el estado del LED
    }
    break;
  }
}

void noDisplay_AnimateCurrentScreen(unsigned long frame)
{
}

CyclicScreenFunction noDisplayCyclicScreens[] = {noDisplay_NoScreen};

DisplayDriver noDisplayDriver = {
    noDisplay_Init,
    noDisplay_AlternateScreenState,
    noDisplay_AlternateRotation,
    noDisplay_LoadingScreen,
    noDisplay_SetupScreen,
    noDisplayCyclicScreens,
    noDisplay_AnimateCurrentScreen,
    noDisplay_DoLedStuff,
    SCREENS_ARRAY_SIZE(noDisplayCyclicScreens),
    0,
    0,
    0,
};
#endif
