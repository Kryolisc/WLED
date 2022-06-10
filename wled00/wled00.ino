/*
 * WLED Arduino IDE compatibility file.
 * 
 * Where has everything gone?
 * 
 * In April 2020, the project's structure underwent a major change. 
 * Global variables are now found in file "wled.h"
 * Global function declarations are found in "fcn_declare.h"
 * 
 * Usermod compatibility: Existing wled06_usermod.ino mods should continue to work. Delete usermod.cpp.
 * New usermods should use usermod.cpp instead.
 */
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"
#include "wled.h"

void THREAD_C0(void * parameters);
TaskHandle_t TCORE0;

void setup() {
  WLED::instance().setup();


  xTaskCreatePinnedToCore(
    THREAD_C0, /* Function to implement the task */
    "THREAD_C0", /* Name of the task */
    4096,  /* Stack size in words */
    NULL,  /* Task input parameter */
    10,  /* Priority of the task */
    &TCORE0,  /* Task handle. */
    0); /* Core where the task should run */
}

void loop() {
  WLED::instance().loop();
}

void THREAD_C0(void * parameters) {
  for (;;) {

    // Disable watchdog triggering because server.handleClient() blocks the thread too long sometimes:
    // https://github.com/espressif/arduino-esp32/issues/922
    // TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
    // TIMERG0.wdt_feed = 1;
    // TIMERG0.wdt_wprotect = 0;

    WLED::instance().async();
  }
}