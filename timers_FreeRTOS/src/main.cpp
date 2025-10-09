#include <Arduino.h>

//using only oone core
#if CONFIG_FREERTOS_UNIcORE 
static const BaseType_t app_cpu = 0;
#else 
static const BaseType_t app_cpu = 1;
#endif


static TimerHandle_t one_shot_timer = NULL;
static TimerHandle_t auto_reload_timer = NULL;
//callbacks

void myTimerCallback(TimerHandle_t xTimer){
  //print message if timer 0 has expired
  if((uint32_t)pvTimerGetTimerID(xTimer) == 0){
    Serial.println("one shot timer expired");
  }

  if((uint32_t)pvTimerGetTimerID(xTimer) == 1){
        Serial.println("auto reload timer expired");
  }
}
void setup() {
  Serial.begin(115200);

  vTaskDelay(1000/portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("FreeRTOS timer demo");

  //create one shot timer
  one_shot_timer = xTimerCreate(
                      "one-shot timer",         //name of timer
                      2000/ portTICK_PERIOD_MS, //period of timer
                      pdFALSE,  //auto-reload
                      (void*)0, //timer ID
                      myTimerCallback //callback funtion
  );

    //create auto reload tiemr
  auto_reload_timer = xTimerCreate(
                      "auto reload timer",         //name of timer
                      1000/ portTICK_PERIOD_MS, //period of timer
                      pdTRUE,  //auto-reload
                      (void*)1, //timer ID
                      myTimerCallback //callback funtion
  );

  if(one_shot_timer || auto_reload_timer == NULL){
    Serial.println("could not create one of the timer");
  }else{

    //wait and then print out a message that were starting the timer
    vTaskDelay(1000/portTICK_PERIOD_MS);
    Serial.println("Staritng timers . . . ");

    //start timer (max block time if command queue is full)
    xTimerStart(one_shot_timer, portMAX_DELAY);
    xTimerStart(auto_reload_timer,portMAX_DELAY); // what does portMAX delay mean
  }

  // delete self task to show that timers will work with no user tasks
  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}

