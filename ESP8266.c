#include "ESP8266.h"

char ESP_RX_buff[ESP_RX_buff_size];
char ESP_TX_buff[ESP_TX_buff_size];

volatile uint8_t recvByte;
int ESP_RX_buff_index = 0;

UART_HandleTypeDef *ESP8266_huart;
GPIO_TypeDef *ESP8266_PinPort;
uint32_t ESP8266_PinNum;

void ESP8266_Init(UART_HandleTypeDef *huart, GPIO_TypeDef *pinPort, uint32_t pinNum)
{
    ESP8266_PinPort = pinPort;
    ESP8266_PinNum = pinNum;
    ESP8266_huart = huart;
}

void ESP8266_ON()
{
    HAL_GPIO_WritePin(ESP8266_PinPort, ESP8266_PinNum, GPIO_PIN_SET);
}

void ESP8266_OFF()
{
    HAL_GPIO_WritePin(ESP8266_PinPort, ESP8266_PinNum, GPIO_PIN_RESET);
}

bool ESP8266_Restart()
{
    return ESP8266_Send("AT+RST\r\n") && ESP8266_Recv("OK");
}

bool ESP8266_Test(void)
{
	return ESP8266_Send("AT\r\n") && ESP8266_Recv("OK");
}

bool ESP8266_EnableEcho()
{
    return ESP8266_Send("ATE1\r\n") && ESP8266_Recv("OK");
}

bool ESP8266_DisableEcho()
{
    return ESP8266_Send("ATE0\r\n") && ESP8266_Recv("OK");
}

bool ESP8266_DisconnectFromWifi()
{
	return ESP8266_Send("AT+CWQAP\r\n") && ESP8266_Recv("OK");
}

char *ESP8266_SendRequest(char *type, char *ip, uint8_t port, char *request)
{
	return ESP8266_AT_CIPSTART(type, ip, port) && ESP8266_AT_CIPSEND(strlen(request) + 2) && ESP8266_AT_SendData(request)? ESP_RX_buff: NULL;
}

bool ESP8266_AT_CIPSTART(char *type, char *ip, uint8_t port)
{
    sprintf(ESP_TX_buff, "AT+CIPSTART=\"%s\",\"%s\",%d\r\n", type, ip, port);
    return ESP8266_Send(ESP_TX_buff) && ESP8266_Recv("OK");
}

bool ESP8266_AT_CIPSEND(int requestLength)
{
    sprintf(ESP_TX_buff, "AT+CIPSEND=%d\r\n", requestLength);
    return ESP8266_Send(ESP_TX_buff) && ESP8266_Recv("OK");
}

bool ESP8266_AT_SendData(char *request)
{
    sprintf(ESP_TX_buff, "%s\r\n", request);
    return ESP8266_Send(ESP_TX_buff) && ESP8266_Recv("OK");
}

bool ESP8266_Send(char *command)
{
	return  HAL_UART_Transmit(ESP8266_huart,(uint8_t*)command, strlen(command), 100) == HAL_OK? true: false;
}

bool ESP8266_Recv(char *correctAnswer)
{
    uint8_t prevRecvByte = 0;
    uint32_t timeout = 10000;
	uint32_t time = HAL_GetTick();

    ESP8266_ClearRecvBuff();
    HAL_UART_Receive_IT(ESP8266_huart, (uint8_t*)&recvByte, (uint16_t)1);

	while(HAL_GetTick() - time < timeout)
	{
		if(prevRecvByte != recvByte)
		{
			time = HAL_GetTick();
			prevRecvByte = recvByte;
		}

		if(strstr(ESP_RX_buff, correctAnswer) != NULL)
		{
			HAL_UART_AbortReceive(ESP8266_huart);
			return true;
		}

		//Код задержки
	}

	return false;
}

void ESP8266_ClearRecvBuff()
{
	memset(ESP_RX_buff, 0, ESP_RX_buff_size);
	ESP_RX_buff_index = 0;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == ESP8266_huart)
	{
		ESP_RX_buff[ESP_RX_buff_index++] = recvByte;
		HAL_UART_Receive_IT(ESP8266_huart, (uint8_t*)&recvByte, (uint16_t)1);
	}
}

bool ESP8266_ConnectTo(char *wifiName, char *password)
{
	sprintf(ESP_TX_buff, "AT+CWJAP_CUR=\"%s\",\"%s\"\r\n", wifiName, password);
	return ESP8266_Send(ESP_TX_buff) && ESP8266_Recv("OK");
}

bool ESP8266_ConnectToAnyAccessPointFromDefaultList()
{
	int accessPointsAmount = sizeof(DefaultAccessPointsList) / sizeof(DefaultAccessPointsList[0]);

	for(int i = 0; i < accessPointsAmount; i++)
		if(ESP8266_ConnectTo(DefaultAccessPointsList[i].accessPointName, DefaultAccessPointsList[i].accessPointPass))
			return true;

	return false;
}

char *ESP8266_GetAcceessPoints()
{
    return ESP8266_Send("AT+CWLAP\r\n") && ESP8266_Recv("OK")? ESP_RX_buff: NULL;
}
