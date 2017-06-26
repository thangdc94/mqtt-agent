/**
 * @file mqtt_agent.c
 * @brief A MQTT Agent
 * @details MQTT Agent provides function to connect, subscribe
 * and publish message to MQTT Broker. 
 * 
 * @author Pham Ngoc Thang (thangdc94)
 * @bug No known bug
 */

#include <stdlib.h>     /* for EXIT_FAILURE macro */
#include <string.h>     /* for strlen() */

#include "mqtt_agent.h" /* for function prototypes */
#include "hashmap.h"    /* for hashtable */
#include "logutil.h"    /* for LOG() and log level */

/** Log debug level */
#define LOG_LEVEL LOG_DBG

/** QOS of Publish/Subscribe */
#define QOS 1

/** MQTT Broker Address */
#define BROKER_ADDRESS "tcp://thingxyz.net:1883"

struct hashmap g_callback_map;  /** hash table for topicName-callback */

/**
 * @brief Handle delivery Completed
 * @details Function pointer for MQTT Callback
 * 
 * @param context application-specific context
 * @param dt Delivery topken
 */
static void delivered(void *context, MQTTClient_deliveryToken dt)
{
    LOG(LOG_DBG, "Message with token value %d delivery confirmed\n", dt);
}

/**
 * @brief Handle Message arrived
 * @details Function pointer for MQTT Callback
 * 
 * @param context application-specific context
 * @param topicName topic name of the message
 * @param topicLen Length of topicName
 * @param message MQTT message
 * @return Returning 1 indicates that the message has been successfully handled.
 * Returning 0 indicates that there was a problem.
 */
static int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    char *payloadptr;
    payloadptr = message->payload;
    payloadptr[message->payloadlen] = '\0';
    // topicName[topicLen] = '\0';

    LOG(LOG_DBG, "topic: %s, message: %s", topicName, payloadptr);

    mqttAgent_onMessage *callback = (mqttAgent_onMessage *) hashmap_get(&g_callback_map, topicName);
    if (callback)
    {
        (*callback)(topicName, topicLen, payloadptr);
    }

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

/**
 * @brief Handle Connection lost
 * @details Function pointer for MQTT Callback
 * 
 * @param context application-specific context
 * @param cause The reason for the disconnection
 */
static void connlost(void *context, char *cause)
{
    LOG(LOG_DBG, "Connection lost. Cause: %s", cause);
    exit(1);
}

void mqttAgent_initClient(MQTTClient *client, char *clientId)
{
    LOG(LOG_DBG, "%s", __func__);
    // initialize MQTTClient
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(client, BROKER_ADDRESS, clientId, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 10;
    conn_opts.cleansession = 1;

    MQTTClient_setCallbacks(*client, NULL, connlost, msgarrvd, delivered);

    // send Connect message
    if ((rc = MQTTClient_connect(*client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        LOG(LOG_ERR, "Failed to connect, return code %d", rc);
        exit(EXIT_FAILURE);
    }

    if ((rc = hashmap_init(&g_callback_map, hashmap_hash_string, hashmap_compare_string, 0)) < 0)
    {
        LOG(LOG_ERR, "Create Hash Table failed, return code %d", rc);
    }
}

void mqttAgent_publish(MQTTClient *client, char *topicName, char *content)
{
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    pubmsg.payload = content;
    pubmsg.payloadlen = strlen(content);
    MQTTClient_publishMessage(*client, topicName, &pubmsg, NULL);
}

void mqttAgent_subscribe(MQTTClient *client, char *topicName)
{
    LOG(LOG_DBG, "%s", __func__);
    MQTTClient_subscribe(*client, topicName, QOS);
}

int mqttAgent_registerMessageCallback(const char *topicName, mqttAgent_onMessage *callback)
{
    LOG(LOG_DBG, "%s", __func__);
    if (!hashmap_put(&g_callback_map, topicName, callback))
    {
        LOG(LOG_ERR, "Malloc failed");
        return -1;
    }
    // @todo check if register same topic
    return 0;
}

int mqttAgent_unregisterMessageCallback(const char *topicName)
{
    LOG(LOG_DBG, "%s", __func__);
    if (!hashmap_remove(&g_callback_map, topicName))
    {
        LOG(LOG_ERR, "Entry not found");
        return -1;
    }
    return 0;
}