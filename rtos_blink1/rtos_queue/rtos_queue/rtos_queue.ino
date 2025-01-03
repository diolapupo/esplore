#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif
 
static const uint8_t msg_queue_len = 5;

static QueueHandle_t msg_queue;

///**************
//Tasks

void printMessages(void *parameters){
  int item;

  while(1){
    if(xQueueReceive(msg_queue, (void *)&item, 0)== pdTRUE){
     // Serial.println(item);
      }
       Serial.println(item);
      vTaskDelay(500/portTICK_PERIOD_MS);
    }
  }
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.println("--FreeRTOS Queue Demo--- ");
  //create queue
  msg_queue = xQueueCreate(msg_queue_len, sizeof(int));
  //start queue task
  xTaskCreatePinnedToCore(printMessages, "Print Messages", 1024, NULL,1,NULL, app_cpu);
  
}

void loop() {

  static int num = 0;
 /// add item to queue for 10 tick
  if(xQueueSend(msg_queue, (void *) &num, 10) != pdTRUE){
    Serial.println("Queue full");
    }
    num++;
  // put your main code here, to run repeatedly:
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
