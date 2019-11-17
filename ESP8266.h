#ifndef ESP8266_H_
#define ESP8266_H_

#include "stm32f4xx_hal.h"
#include "stdio.h" 
#include "stdbool.h"
#include "string.h"

#define ESP_BOOL_ANSWER_SIZE 8
#define ESP_RX_buff_size 64
#define ESP_TX_buff_size 64

//Send command "AT" and return answer as true/false
bool ESP_Test(void);

//Enable/Disable echo of command
bool ESP_Set_Echo(bool enableEcho);

//Connect to Wi-fi point and return result true/false
bool connectTo(char *wifiName, char *password);

//Disconnect from Wi-fi
bool disconnectWifi();

// Send data to server
bool sendRequest(char *type, char *ip, uint8_t port, char *request);

#endif /* ESP8266_H_ */
