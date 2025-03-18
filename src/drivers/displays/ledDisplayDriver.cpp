#include "displayDriver.h"

#ifdef LED_DISPLAY

#include <Arduino.h>
#include "monitor.h"
#include "wManager.h"

#ifdef USE_LED
#include <FastLED.h>
#endif

#ifdef USE_LED
#define MAX_BRIGHTNESS 16
#define SLOW_FADE 1;
#define FAST_FADE 4;

CRGB leds(0, 0, 0);
int brightness = 0;
int fadeDirection = 1;
int fadeAmount = 0;
#endif // USE_LED

bool ledOn = false;
extern monitor_data mMonitor;

void ledDisplay_Init(void)
{
  Serial.println("Led display driver initialized");
#ifdef USE_LED
  FastLED.addLeds<RGB_LED_CLASS, RGB_LED_PIN, RGB_LED_ORDER>(&leds, 1);
  FastLED.show();
#endif // USE_LED
}

void ledDisplay_AlternateScreenState(void)
{
  Serial.println("Switching display state");
  ledOn = !ledOn;
}

void ledDisplay_AlternateRotation(void)
{
}
uint8_t showHeadercounter = 0;
void ledDisplay_NoScreen(unsigned long mElapsed)
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
void ledDisplay_LoadingScreen(void)
{
  Serial.println("Initializing...");
}

void ledDisplay_SetupScreen(void)
{
  Serial.println("Setup...");
}

// Variables para controlar el parpadeo con millis()
unsigned long previousMillis = 0;

void ledDisplay_DoLedStuff(unsigned long frame)
{

#ifdef USE_LED

  if (!ledOn)
  {
    FastLED.clear(true);
    return;
  }

  switch (mMonitor.NerdStatus)
  {
  case NM_waitingConfig:
    brightness = MAX_BRIGHTNESS;
    leds.setRGB(255, 255, 0);
    fadeAmount = 0;
    break;

  case NM_Connecting:
    leds.setRGB(0, 0, 255);
    fadeAmount = SLOW_FADE;
    break;

  case NM_hashing:
    leds.setRGB(0, 0, 255);
    fadeAmount = FAST_FADE;
    break;
  }

  leds.fadeLightBy(0xFF - brightness);
  FastLED.show();

  brightness = brightness + (fadeDirection * fadeAmount);
  if (brightness <= 0 || brightness >= MAX_BRIGHTNESS)
  {
    fadeDirection = -fadeDirection;
  }
  brightness = constrain(brightness, 0, MAX_BRIGHTNESS);
#endif
}

void ledDisplay_AnimateCurrentScreen(unsigned long frame)
{
}

CyclicScreenFunction ledDisplayCyclicScreens[] = {ledDisplay_NoScreen};

DisplayDriver ledDisplayDriver = {
    ledDisplay_Init,
    ledDisplay_AlternateScreenState,
    ledDisplay_AlternateRotation,
    ledDisplay_LoadingScreen,
    ledDisplay_SetupScreen,
    ledDisplayCyclicScreens,
    ledDisplay_AnimateCurrentScreen,
    ledDisplay_DoLedStuff,
    SCREENS_ARRAY_SIZE(ledDisplayCyclicScreens),
    0,
    0,
    0,
};
#endif
