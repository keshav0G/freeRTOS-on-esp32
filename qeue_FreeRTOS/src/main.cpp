#include <Arduino.h>
#if CONFIG_FREERTOS_UNICORE
 static const BaseType_t app_cpu = 0;
 #else
 static const BaseType_t app_cpu = 1;
 #endif

 static const uint8_t msg_queue_len = 5;

 static QueueHandle_t msg_queue;



 void printMessage(void *paramter){
  int item;
  while(1){
    if(xQueueReceive(msg_queue,&item,0) == pdTRUE){
      Serial.println(item);
    }

    //wait before trying again
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
 }

void setup() {

  Serial.begin(115200);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  Serial.println();
  //create queue
  msg_queue = xQueueCreate(msg_queue_len, sizeof(int));

  //start print task
  xTaskCreatePinnedToCore(printMessage,
                          "print Messages",
                        1024,
                      NULL,
                    1,
                  NULL,
                app_cpu);
  
}

void loop() {
  static int num = 0;
  if(xQueueSend(msg_queue,(void *)&num,10) != pdTRUE){
    Serial.println("queue full");
  }
  num++;

  vTaskDelay(1000/portTICK_PERIOD_MS);
}
