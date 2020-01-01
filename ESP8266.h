#ifndef ESP8266_H_
#define ESP8266_H_

#define ESP_RX_buff_size 64
#define ESP_TX_buff_size 64

#define ESP8266_DEBUG

void ESP8266_ClearRecvBuff();

bool ESP8266_Send(char *command);
bool ESP8266_Recv(char *correctAnswer);

//Некорректно работает
char *ESP8266_GetAcceessPointsList();

//Команда проходит, есть ответ, но модуль зависает
bool ESP8266_Restart();

void ESP8266_Init(UART_HandleTypeDef *huart, GPIO_TypeDef *pinPort, uint32_t pinNum);

void ESP8266_ON();
void ESP8266_OFF();

void ESP8266_ErrorHandler(char *errorMessage);

bool ESP8266_Test(void);

bool ESP8266_EnableEcho();
bool ESP8266_DisableEcho();

bool ESP8266_ConnectTo(char *wifiName, char *password);
bool ESP8266_DisconnectFromWifi();

bool ESP8266_SendRequest(char *type, char *ip, uint8_t port, char *request);
bool ESP8266_AT_SendData(char *request);
bool ESP8266_AT_CIPSEND(int requestLength);
bool ESP8266_AT_CIPSTART(char *type, char *ip, uint8_t port);

#include "ESP8266.c"

#endif /* ESP8266_H_ */
