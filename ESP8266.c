#include "ESP8266.h"

char ESP_RX_buff[ESP_RX_buff_size];
char ESP_TX_buff[ESP_TX_buff_size];

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

void ESP8266_ErrorHandler(char *errorMessage)
{
    PC_Send(errorMessage);
    PC_Send("\n");
    
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        HAL_Delay(100);
    }
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

bool ESP8266_ConnectTo(char *wifiName, char *password)
{
	sprintf(ESP_TX_buff, "AT+CWJAP_CUR=\"%s\",\"%s\"\r\n", wifiName, password);
	return ESP8266_Send(ESP_TX_buff) && ESP8266_Recv("WIFI GOT IP");
}

bool ESP8266_DisconnectFromWifi()
{
	return ESP8266_Send("AT+CWQAP\r\n") && ESP8266_Recv("OK");
}

bool ESP8266_SendRequest(char *type, char *ip, uint8_t port, char *request)
{
	return ESP8266_AT_CIPSTART(type, ip, port) && ESP8266_AT_CIPSEND(strlen(request) + 2) && ESP8266_AT_SendData(request);
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
	return HAL_UART_Transmit(ESP8266_huart,(uint8_t*)command, strlen(command), 100) == HAL_OK? true: false;
}

bool ESP8266_Recv(char *correctAnswer)
{
	ESP8266_ClearRecvBuff();

	for(uint8_t i = 0; i < 64; ++i)
	{
		HAL_UART_Receive(ESP8266_huart, (uint8_t *)ESP_RX_buff, ESP_RX_buff_size, 100);

		if(strstr(ESP_RX_buff, correctAnswer) != NULL)
			return true;
	}

	return false;
}

char *ESP8266_GetAcceessPoints()
{
    char *str = "AT+CWLAP\r\n";

	HAL_UART_Transmit(ESP8266_huart,(uint8_t*)str, strlen(str), 100);

	ESP8266_ClearRecvBuff();

	do
	{
		HAL_UART_Receive(ESP8266_huart, (uint8_t *)ESP_RX_buff, ESP_RX_buff, 100);

        if(strstr(ESP_RX_buff, "ERROR") != NULL)
            ESP8266_Error("[ ERROR ] ESP8266_GET_ACCESS_POINTS");

	} while(strstr(ESP_RX_buff, "OK") == NULL);

    return ESP_RX_buff;
}

void ESP8266_ClearRecvBuff()
{
	memset(ESP_RX_buff, 0, ESP_RX_buff_size);
}

bool ESP8266_ConnectToAnyAccessPointFromDefaultList()
{
	int accessPointsAmount = sizeof(DefaultAccessPointsList) / sizeof(DefaultAccessPointsList[0]);

	for(int i = 0; i < accessPointsAmount; ++i)
		if(ESP8266_ConnectTo(DefaultAccessPointsList[i].accessPointName, DefaultAccessPointsList[i].accessPointPass))
			return true;

	return false;
}
