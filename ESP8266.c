#include "ESP8266.h"

bool ESP8266_Debug = false;
char ESP_RX_buff[ESP_RX_buff_size];
char ESP_TX_buff[ESP_TX_buff_size];

char ESP_temp_buff[ESP_temp_buff_size];

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


char *ESP8266_GetAcceessPointsList()
{
    char *str = "AT+CWLAP\r\n";
    
	HAL_UART_Transmit(ESP8266_huart,(uint8_t*)str, strlen(str), 100);
	
	memset(ESP_temp_buff, 0, ESP_temp_buff_size);
	do
	{
		HAL_UART_Receive(ESP8266_huart, (uint8_t *)ESP_temp_buff, ESP_temp_buff_size, 100);
        
        if(strstr(ESP_temp_buff, "ERROR") != NULL)
            ESP8266_Error("[ ERROR ] ESP8266_GET_ACCESS_POINTS");
        
	} while(strstr(ESP_temp_buff, "OK") == NULL);
    
    return ESP_temp_buff;
}

void ESP8266_Restart()
{
    ESP8266_SendCommand("AT+RST\r\n", "OK");
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

void ESP8266_Test(void)
{
	ESP8266_SendCommand("AT\r\n", "OK");
}

void ESP8266_EnableEcho()
{
    ESP8266_SendCommand("ATE1\r\n", "OK");
}

void ESP8266_DisableEcho()
{
    ESP8266_SendCommand("ATE0\r\n", "OK");
}

void ESP8266_ConnectTo(char *wifiName, char *password)
{
	sprintf(ESP_TX_buff, "AT+CWJAP_CUR=\"%s\",\"%s\"\r\n", wifiName, password);
	ESP8266_SendCommand(ESP_TX_buff, "WIFI GOT IP");
}

void ESP8266_DisconnectFromWifi()
{
	char *str = "AT+CWQAP\r\n";
	ESP8266_SendCommand(ESP_TX_buff, "OK");
}

void ESP8266_SendRequest(char *type, char *ip, uint8_t port, char *request)
{
    ESP8266_AT_CIPSTART(type, ip, port);
    ESP8266_AT_CIPSEND(strlen(request) + 2);
	ESP8266_AT_SendData(request);
}

void ESP8266_AT_CIPSTART(char *type, char *ip, uint8_t port)
{
    sprintf(ESP_TX_buff, "AT+CIPSTART=\"%s\",\"%s\",%d\r\n", type, ip, port);
    ESP8266_SendCommand(ESP_TX_buff, "OK");
}

void ESP8266_AT_CIPSEND(int requestLength)
{
    sprintf(ESP_TX_buff, "AT+CIPSEND=%d\r\n", requestLength);
    ESP8266_SendCommand(ESP_TX_buff, "OK");
}

void ESP8266_AT_SendData(char *request)
{
    sprintf(ESP_TX_buff, "%s\r\n", request);
    ESP8266_SendCommand(ESP_TX_buff, "OK");
}

bool ESP8266_SendCommand(char *command, char *correctAnswer)
{
	HAL_UART_Transmit(ESP8266_huart,(uint8_t*)command, strlen(command), 100);

	memset(ESP_RX_buff, 0, ESP_RX_buff_size);

	do
	{
		HAL_UART_Receive(ESP8266_huart, (uint8_t *)ESP_temp_buff, ESP_temp_buff_size, 200);

	    if(strstr(ESP_temp_buff, "ERROR") != NULL)
	    	return false;

	} while(strstr(ESP_temp_buff, correctAnswer) == NULL);

	return true;
}
