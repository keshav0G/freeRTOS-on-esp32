#include <Arduino.h>

#if CONFIG_FREERTOS_UNICPRE
 static const BaseType_t app_cpu = 0;
 #else
 static const BaseType_t app_cpu = 0;
#endif



static const uint8_t LED = 2;
int led_delay = 500;

static TaskHandle_t task1 = NULL;
static TaskHandle_t task2 = NULL;

void blinkTask(void *parameter){
  while(1){
  digitalWrite(LED,HIGH);
  vTaskDelay(led_delay/portTICK_PERIOD_MS);
  digitalWrite(LED, LOW);
  vTaskDelay(led_delay/portTICK_PERIOD_MS);
  }
}

void takeInput(void *paramter){
  while(1){
  if(Serial.available()> 0){
    int new_delay = Serial.parseInt();
    Serial.readStringUntil('\n');
    if(new_delay >0){
      led_delay = new_delay;
       Serial.printf("Delay updated to: %d ms\n", led_delay);
      } else {
        Serial.println("Invalid input received. Please enter a positive number.");
      }

    }
  }
}
void setup() {
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
  Serial.println("enter delay period");
   xTaskCreatePinnedToCore(
    blinkTask,
    "blink task",
    2048,
    NULL,
    1,
    &task1,
    app_cpu
   );
  
   xTaskCreatePinnedToCore(
    takeInput,
    "input task",
    2048,
    NULL,
    2,
    &task2,
    app_cpu
   );
}

void loop() {
  // put your main code here, to run repeatedly:
}

