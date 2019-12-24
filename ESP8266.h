#ifndef ESP8266_H_
#define ESP8266_H_

#include "stm32f1xx.h"
#include "stdio.h" 
#include "stdbool.h"
#include "string.h"

#define ESP_RX_buff_size 64
#define ESP_TX_buff_size 64
#define ESP_temp_buff_size 2048

#define ESP8266_DEBUG
//#define ESP8266_SHOW_ANSWER_ON_REQUEST

//Некорректно работает
char *ESP8266_GetAcceessPointsList();

//Команда проходит, есть ответ, но модуль зависает
void ESP8266_Restart();

void ESP8266_Init(UART_HandleTypeDef *huart, GPIO_TypeDef *pinPort, uint32_t pinNum);

void ESP8266_ON();

void ESP8266_OFF();

void ESP8266_Error(char *errorMessage);

void ESP8266_Test(void);

void ESP8266_EnableEcho();

void ESP8266_DisableEcho();

void ESP8266_ConnectTo(char *wifiName, char *password);

void ESP8266_DisconnectFromWifi();

void ESP8266_SendRequest(char *type, char *ip, uint8_t port, char *request);
void ESP8266_AT_SendData(char *request);
void ESP8266_AT_CIPSEND(char *request);
void ESP8266_AT_CIPSTART(char *type, char *ip, uint8_t port);

#endif /* ESP8266_H_ */
