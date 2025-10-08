#include <Arduino.h>
#if CONFIG_FREERTOS_UNICORE 
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static SemaphoreHandle_t mutex;

static int shared_var = 0;

//increment the shared variable 
void incTask(void *paramters){
  if(xSemaphoreTake(mutex,0) == pdTRUE){
  int local_var;
  while(1){
    local_var = shared_var;
    local_var++;
    vTaskDelay(random(100,500)/portTICK_PERIOD_MS);
    shared_var = local_var;

    // give mutex after critical section
    xSemaphoreGive(mutex);

    Serial.println(shared_var);


  }
  } else {
    //do sumn else
  }
}
 
void setup() {
  // put your setup code here, to run once:
  randomSeed(analogRead(0));

  Serial.begin(115200);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  Serial.println();

  //create mutex before starting tasks
  mutex = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(incTask,
                          "incriment task",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);



 
}

void loop() {
  // put your main code here, to run repeatedly:
}
