#include <Arduino.h>

#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
 static const BaseType_t app_cpu = 1;
 #endif

 const char msg[] = "Hello from app_cpu ";

 //task handles

static TaskHandle_t Task1 = NULL; // handles are used to reference a task
static TaskHandle_t Task2 = NULL;

//tasks
//************************************************************************************/
void StartTask1(void *parameter){
  int msg_len = strlen(msg);
  while(1) {
    Serial.println();
    for(int i = 0; i<msg_len; i++){
      Serial.print(msg[i]);
      vTaskDelay(1000/portTICK_PERIOD_MS);
    }
  }
}

void StartTask2(void *parameter){
  while(1) {
   Serial.print('*');
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}


void setup() {
Serial.begin(300);
Serial.println();
xTaskCreatePinnedToCore(
  StartTask1,
  "Task1",
  1024,
  NULL,
  1,
  &Task1,
  app_cpu
);

xTaskCreatePinnedToCore(
  StartTask2,
  "Task2",
  1024,
  NULL,
  2,
  &Task2,
  app_cpu
);


}

void loop() {
  // put your main code here, to run repeatedly:
  //suspend the higher priority task for some intervals
  for(int i = 0; i<3; i++){
    vTaskSuspend(Task2);
    vTaskDelay(2000/portTICK_PERIOD_MS);
    vTaskResume(Task2);;
    vTaskDelay(2000/portTICK_PERIOD_MS);
  }

  // delete the lower priority task
  if(Task1 != NULL){
    vTaskDelete(Task1);
    Task1 = NULL;
  }
  
}

