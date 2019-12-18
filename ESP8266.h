#ifndef ESP8266_H_
#define ESP8266_H_

#include "stm32f1xx.h"
#include "stdio.h" 
#include "stdbool.h"
#include "string.h"

#define ESP_RX_buff_size 64
#define ESP_TX_buff_size 64

#define ESP8266_TEST_OK 0
#define ESP8266_TEST_ERROR 1

#define ESP8266_ENABLE_ECHO_OK 2
#define ESP8266_ENABLE_ECHO_ERROR 3

#define ESP8266_DISABLE_ECHO_OK 4
#define ESP8266_DISABLE_ECHO_ERROR 5

#define ESP8266_WIFI_CONNECT_OK 6
#define ESP8266_WIFI_CONNECT_ERROR 7

#define ESP8266_WIFI_DISCONNECT_OK 8
#define ESP8266_WIFI_DISCONNECT_ERROR 9

#define ESP8266_AT_CIPSTART_ERROR 10
#define ESP8266_AT_CIPSEND_ERROR 11
#define ESP8266_SEND_REQUEST_ERROR 12
#define ESP8266_SEND_REQUEST_OK 12

#define ESP8266_DEBUG
#define ESP8266_SHOW_ANSWER_ON_REQUEST

void ESP8266_AT_SendData(char *request);

void ESP8266_AT_CIPSEND(char *request);

void ESP8266_AT_CIPSTART(char *type, char *ip, uint8_t port);

void ESP8266_Error(char *errorMessage);

void ESP8266_Test(void);

void ESP8266_EnableEcho();

void ESP8266_DisableEcho();

void ESP8266_ConnectTo(char *wifiName, char *password);

void ESP8266_DisconnectFromWifi();

void ESP8266_SendRequest(char *type, char *ip, uint8_t port, char *request);

#endif /* ESP8266_H_ */
