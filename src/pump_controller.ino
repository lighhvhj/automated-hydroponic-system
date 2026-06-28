// ============================================================
//  Automated Hydroponic System — Pump Controller
//  ESP32 + Blynk IoT
//  Controls water pump: AUTO (1 min ON / 2 hrs OFF cycle)
//                       MANUAL override via Blynk app (V1)
//
//  Hardware: ESP32, 1-Channel Relay Module (GPIO 32)
//  Platform: Blynk IoT 2.0
// ============================================================

// --- Blynk Credentials (fill in your own) ---
#define BLYNK_TEMPLATE_ID   "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "hydroponic"
#define BLYNK_AUTH_TOKEN    "YOUR_AUTH_TOKEN"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// --- WiFi Credentials (fill in your own) ---
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

// --- Pin Definitions ---
#define RELAY_PIN 32   // Relay connected to GPIO 32

BlynkTimer timer;
int autoTimerID = -1;
int offTimerID  = -1;

// ---- Update Blynk Label (V2) ----
void updateLabel(const char* msg) {
  Blynk.virtualWrite(V2, msg);
  Serial.println(msg);
}

// ---- Start Pump (runs for 1 minute, then schedules next cycle) ----
void startPump() {
  digitalWrite(RELAY_PIN, HIGH);
  updateLabel("Pump ON (1 min)");

  // Cancel any existing timers
  if (offTimerID  != -1) timer.deleteTimer(offTimerID);
  if (autoTimerID != -1) timer.deleteTimer(autoTimerID);

  // Turn OFF after 1 minute (60,000 ms)
  offTimerID = timer.setTimeout(60000L, []() {
    digitalWrite(RELAY_PIN, LOW);
    updateLabel("Pump OFF - Next cycle in 2 hrs");

    // Schedule next auto-start after 2 hours (7,200,000 ms)
    autoTimerID = timer.setTimeout(7200000L, startPump);
  });
}

// ---- Stop Pump (manual stop) ----
void stopPump() {
  digitalWrite(RELAY_PIN, LOW);
  updateLabel("Pump OFF - Manual Stop");

  if (offTimerID  != -1) timer.deleteTimer(offTimerID);
  if (autoTimerID != -1) timer.deleteTimer(autoTimerID);
}

// ---- Blynk Virtual Pin V1 — Manual Switch ----
BLYNK_WRITE(V1) {
  int cmd = param.asInt();
  if (cmd == 1) startPump();
  else           stopPump();
}

// ---- Start pump automatically when Blynk connects ----
BLYNK_CONNECTED() {
  startPump();
}

// ---- Setup ----
void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);   // Pump OFF at startup
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  updateLabel("Connecting...");
}

// ---- Loop ----
void loop() {
  Blynk.run();
  timer.run();
}
