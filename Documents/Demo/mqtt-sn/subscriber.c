

#include "contiki.h"
#include "lib/random.h"
#include "clock.h"
#include "sys/ctimer.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "mqtt_sn.h"
#include "dev/leds.h"
#include "net/rime/rime.h"
#include "net/ip/uip.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LED_RED        LEDS_RED
#define LED_GREEN      LEDS_GREEN
#define LED_BLUE       LEDS_BLUE


typedef struct{
  uint8_t temp;
  uint8_t humid;
} SENSOR;

uint16_t GetMsgID(char* message);
uint8_t GetMsgValue(char* message);
static void v_LED_Handle(void);

SENSOR sensorValue[100];

static uint8_t idx = 0;

static uint16_t udp_port = 1884;
static uint16_t keep_alive = 5;
static uint16_t broker_address[] = {0xaaaa, 0, 0, 0, 0, 0, 0, 0x1};
static struct   etimer time_poll;
// static uint16_t tick_process = 0;
static char     led_cmd[20];    // command for led control
static char     pub_test[20];
static char     device_id[17];
static char     topic_hw[25];
static char     *topics_mqtt[] = {"/led_control",
                                  "/temp",
                                  "/humid"};
// static char     *will_topic = "/6lowpan_node/offline";
// static char     *will_message = "O dispositivo esta offline";
// This topics will run so much faster than others

mqtt_sn_con_t mqtt_sn_connection;

void mqtt_sn_callback(char *topic, char *message){
  uint16_t ID;

  printf("\nMessage received: topic %s payload:%s",topic,message); 

  if(strcmp(topic, "/led_control") == 0){
    strcpy(led_cmd, message);
  }
  else if(strcmp(topic, "/temp") == 0){
    ID = GetMsgID(message);
    sensorValue[ID].temp = GetMsgValue(message);
  }
  else if(strcmp(topic, "/humid") == 0){
    ID = GetMsgID(message);
    sensorValue[ID].humid = GetMsgValue(message);
  }
}

void init_broker(void){
  char *all_topics[ss(topics_mqtt)+1];
  sprintf(device_id,"%02X%02X%02X%02X%02X%02X%02X%02X",
          linkaddr_node_addr.u8[0],linkaddr_node_addr.u8[1],
          linkaddr_node_addr.u8[2],linkaddr_node_addr.u8[3],
          linkaddr_node_addr.u8[4],linkaddr_node_addr.u8[5],
          linkaddr_node_addr.u8[6],linkaddr_node_addr.u8[7]);
  sprintf(topic_hw,"Hello addr:%02X%02X",linkaddr_node_addr.u8[6],linkaddr_node_addr.u8[7]);

  mqtt_sn_connection.client_id     = device_id;
  mqtt_sn_connection.udp_port      = udp_port;
  mqtt_sn_connection.ipv6_broker   = broker_address;
  mqtt_sn_connection.keep_alive    = keep_alive;
  //mqtt_sn_connection.will_topic    = will_topic;   // Configure as 0x00 if you don't want to use
  //mqtt_sn_connection.will_message  = will_message; // Configure as 0x00 if you don't want to use
  mqtt_sn_connection.will_topic    = 0x00;
  mqtt_sn_connection.will_message  = 0x00;

  mqtt_sn_init();   // Inicializa alocação de eventos e a principal PROCESS_THREAD do MQTT-SN

  size_t i;
  for(i=0;i<ss(topics_mqtt);i++)
    all_topics[i] = topics_mqtt[i];
  all_topics[i] = topic_hw;

  mqtt_sn_create_sck(mqtt_sn_connection,
                     all_topics,
                     ss(all_topics),
                     mqtt_sn_callback);
  mqtt_sn_sub((char*)"#",0);
}

uint16_t GetMsgID(char* message){
  uint16_t msgID = 0;
  msgID = (message[0]-0x30)*100;
  msgID += (message[1]-0x30)*10;
  msgID += (message[2]-0x30);

  return msgID;
}

uint8_t GetMsgValue(char* message){
  uint8_t msgValue = 0;
  msgValue = (message[4]-0x30)*100;
  msgValue += (message[5]-0x30)*10;
  msgValue += (message[6]-0x30);

  return msgValue;
}


static void v_LED_Handle(void) {
  if (strcmp(led_cmd, "red_on") == 0) {
    leds_on(LED_RED);    
  }
  else if (strcmp(led_cmd, "red_off") == 0) {
    leds_off(LED_RED);    
  }
  else if (strcmp(led_cmd, "green_on") == 0) {
    leds_on(LED_GREEN);    
  }
  else if (strcmp(led_cmd, "green_off") == 0) {
    leds_off(LED_GREEN);    
  }
}


/*---------------------------------------------------------------------------*/
PROCESS(init_system_process, "[Contiki-OS] Initializing OS");
AUTOSTART_PROCESSES(&init_system_process);
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(init_system_process, ev, data) {
  PROCESS_BEGIN();

  debug_os("Initializing the MQTT_SN_DEMO");

  init_broker();

  etimer_set(&time_poll, CLOCK_SECOND);

  while(1) {
    PROCESS_WAIT_EVENT();

    v_LED_Handle();

    for(idx=3; idx<10; idx++){
        printf(pub_test, "ID%d, temp: %d, humid: %d", idx,
                sensorValue[idx].temp, sensorValue[idx].humid);
    }
   
    //sprintf(pub_test,"%s",topic_hw);
    mqtt_sn_sub("/led_control",0);
    // debug_os("State MQTT:%s",mqtt_sn_check_status_string());
    if (etimer_expired(&time_poll))
      etimer_reset(&time_poll);
  }

  PROCESS_END();
}
