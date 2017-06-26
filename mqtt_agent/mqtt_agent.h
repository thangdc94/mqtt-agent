/**
 * @file mqtt_agent.h
 * @brief Function prototypes for MQTT Agent
 * @details This contains the prototypes for MQTT Agent
 * 
 * @author Pham Ngoc Thang (thangdc94)
 * @bug No known bug
 */

#ifndef __MQTT_AGENT_H
#define __MQTT_AGENT_H

#include <MQTTClient.h>

/**
 * @brief Handle message received
 * @details Function for users to implement their own code
 * to handle message received
 * 
 * @param topicName Topic Name of the message
 * @param topicLen Length of topic name
 * @param messageContent content of the message
 */
typedef void mqttAgent_onMessage(char *topicName, int topicLen, char *messageContent);

/**
 * @brief Initialize MQTT Client
 * @details Create MQTT Client and connect to Broker
 * 
 * @param client Client to init
 * @param clientId ClientID you want to create
 */
void mqttAgent_initClient(MQTTClient *client, char *clientId);

/**
 * @brief Publish message
 * @details Publish message to MQTT Broker
 * 
 * @param client client object
 * @param topicName Name of the topic you want to public message to
 * @param content Content of the message
 */
void mqttAgent_publish(MQTTClient *client, char *topicName, char *content);

/**
 * @brief Subscribe topic
 * @details Subscribe to a topic
 * 
 * @param client client object
 * @param topicName Name of the topic you want to receive message
 */
void mqttAgent_subscribe(MQTTClient *client, char *topicName);

/**
 * @brief Register a message callback
 * @details Register a message handler so you can handle message yourself
 * 
 * @param topicName Name of the topic you want to handle
 * @param callback callback function pointer 
 * @see ::mqttAgent_onMessage()
 * 
 * @return 0 if OK and -1 if error
 */
int mqttAgent_registerMessageCallback(const char *topicName, mqttAgent_onMessage *callback);

/**
 * @brief Unregister a message callback
 * @details Unregister a message handler of a topic. You dont want to handle
 * messages from a topic anymore. 
 * 
 * @param topicName Name of the topic you want to unregister callback
 * @return 0 if OK and -1 if error
 */
int mqttAgent_unregisterMessageCallback(const char *topicName);

#endif //__MQTT_AGENT_H

