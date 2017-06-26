/**
 * @file test.c
 * @brief An example code for how to use MQTT Agent
 * @details An example code for how to use MQTT Agent
 * Need to define LOG_LEVEL
 * 
 * @author Pham Ngoc Thang (thangdc94)
 * @bug No known bug
 */

#include <unistd.h>

#include "mqtt_agent.h"
#include "logutil.h"

#define LOG_LEVEL LOG_DBG

void onMessage(char *topicName, int topicLen, char *messageContent)
{
    LOG(LOG_DBG, "Receive %s\n", messageContent);
}

int main(int argc, char const *argv[])
{
    MQTTClient mqttClient;
    mqttAgent_initClient(&mqttClient, "demo");
    mqttAgent_publish(&mqttClient, "demo", "hello guys");
    mqttAgent_subscribe(&mqttClient, "test");
    mqttAgent_registerMessageCallback("test", onMessage);
    while(1)
    {
        sleep(15000);
    }
    return 0;
}
