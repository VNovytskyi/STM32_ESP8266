#ifndef ESP8266_H_
#define ESP8266_H_

#include "stdbool.h"

#define ESP_RX_buff_size 2048
#define ESP_TX_buff_size 64

#define ESP_DEBUG

bool ESP8266_ConnectToAnyAccessPointFromDefaultList();

void ESP8266_ClearRecvBuff();

bool ESP8266_Send(char *command);
bool ESP8266_Recv(char *correctAnswer);

char *ESP8266_GetAcceessPoints();

//Команда проходит, есть ответ, но модуль зависает
bool ESP8266_Restart();

void ESP8266_Init(UART_HandleTypeDef *huart, GPIO_TypeDef *pinPort, uint32_t pinNum);

void ESP8266_ON();
void ESP8266_OFF();

bool ESP8266_Test(void);

bool ESP8266_EnableEcho();
bool ESP8266_DisableEcho();

bool ESP8266_ConnectTo(char *wifiName, char *password);
bool ESP8266_DisconnectFromWifi();

char *ESP8266_SendRequest(char *type, char *ip, uint8_t port, char *request);
bool ESP8266_AT_CIPSTART(char *type, char *ip, uint8_t port);
bool ESP8266_AT_CIPSEND(int requestLength);
bool ESP8266_AT_SendData(char *request);

struct AccessPoint
{
	char accessPointName[64];
	char accessPointPass[64];
};

struct AccessPoint DefaultAccessPointsList[] = {{"Snapy", "31055243167vlad"}, {"MERCUSYS_7EBA", "3105vlad3010vlada"}};

#include "ESP8266.c"
#endif /* ESP8266_H_ */
