#include <Arduino.h>

//using only one core
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else 
static const BaseType_t app_cpu = 1;
#endif

//pins
static const int Ledpin = 2;

//our task to blink an led
void toggleLED(void *paramter){
  while (1)
  {
    digitalWrite(Ledpin, HIGH);
    vTaskDelay(500/portTICK_PERIOD_MS);
    digitalWrite(Ledpin,LOW);
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
  
}
void setup() {

  Serial.begin(115200);
  pinMode(Ledpin,OUTPUT);
  //creating task
  xTaskCreatePinnedToCore(  // use xTaskCreate in vanilla FreeRTOS
    toggleLED, // function to be called
    "blinkingLED",// name of the task
    1024, // stack size 
    NULL, // parameter to be passed
    1, // priority
    NULL, // task handle
    app_cpu // core to run the task
  );
 
}

void loop() {
  // put your main code here, to run repeatedly:
}
