const int ledPin = 16;  // 16 corresponds to GPIO16
uint16_t dutyCycle;
uint16_t previousDutyCycle = 0;
// setting PWM properties
const int freq = 5000;
// const int freq = 167;
const int ledChannel = 0;
const int resolution = 13;

#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  3           /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int bootCount = 0;

unsigned long timer = 0;

void setup(){
  Serial.begin(115200);
  ++bootCount;
  //Print the wakeup reason for ESP32
  print_wakeup_reason();
  Serial.println("Boot number: " + String(bootCount));
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,0); //1 = High, 0 = Low
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);

  timer = millis();
}
void loop(){
  dutyCycle =  analogRead(A0);

  if(previousDutyCycle != dutyCycle){ 
    Serial.println(dutyCycle);
  }

  previousDutyCycle = dutyCycle;

  // changing the LED brightness with PWM
  ledcWrite(ledChannel, dutyCycle);
  //  delay(15);

  
  if(millis() - timer >= 4000){
    Serial.println("Activating Deep Sleep…");
    esp_deep_sleep_start();    
  }
}

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}