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
    char *str = "AT+RST\r\n";
    
	HAL_UART_Transmit(ESP8266_huart,(uint8_t*)str, strlen(str), 100);
	
	HAL_UART_Receive(ESP8266_huart, (uint8_t *)ESP_RX_buff, ESP_RX_buff_size, 100);
    
    if(strstr(ESP_RX_buff, "OK") == NULL)
    {
        ESP8266_Error("[ ERROR ] ESP8266_RESTART");
    }
    
    #ifdef ESP8266_DEBUG
        PC_Send("[ OK ] ESP8266_RESTART\n");
    #endif
}

void ESP8266_Error(char *errorMessage)
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
	char *str = "AT\r\n";
    
	HAL_UART_Transmit(ESP8266_huart,(uint8_t*)str, strlen(str), 100);
	
	HAL_UART_Receive(ESP8266_huart, (uint8_t *)ESP_RX_buff, ESP_RX_buff_size, 100);
    
    if(strstr(ESP_RX_buff, "OK") == NULL)
    {
        ESP8266_Error("[ ERROR ] ESP8266_TEST_ERROR");
    }
    
    #ifdef ESP8266_DEBUG
        PC_Send("[ OK ] ESP8266_TEST\n");
    #endif
}

void ESP8266_EnableEcho()
{
    char *str2 = "ATE1\r\n";
    HAL_UART_Transmit(ESP8266_huart,(uint8_t*)str2, strlen(str2), 100);
    HAL_UART_Receive(ESP8266_huart, (uint8_t *)ESP_RX_buff, ESP_RX_buff_size, 100);
    
    if(strstr(ESP_RX_buff, "OK") == NULL)
    {
        ESP8266_Error("[ ERROR ] ESP8266_ENABLE_ECHO_ERROR");
    }
    
    #ifdef ESP8266_DEBUG
        PC_Send("[ OK ] ESP8266_ENABLE_ECHO\n");
    #endif
}

void ESP8266_DisableEcho()
{
    char *str1 = "ATE0\r\n";
    HAL_UART_Transmit(ESP8266_huart,(uint8_t*)str1, strlen(str1), 100);
    
    memset(ESP_RX_buff, 0, ESP_RX_buff_size);
    do
	{
		HAL_UART_Receive(ESP8266_huart, (uint8_t *)ESP_RX_buff, ESP_RX_buff_size, 100);
    
	}while(strstr(ESP_RX_buff, "OK") == NULL && strstr(ESP_RX_buff, "ERROR") == NULL);
    
    if(strstr(ESP_RX_buff, "OK") == NULL)
    {
        ESP8266_Error("[ ERROR ] ESP8266_DISABLE_ECHO_ERROR");
    }
    
    #ifdef ESP8266_DEBUG
        PC_Send("[ OK ] ESP8266_DISABLE_ECHO\n");
    #endif
}

void ESP8266_ConnectTo(char *wifiName, char *password)
{
	sprintf(ESP_TX_buff, "AT+CWJAP_CUR=\"%s\",\"%s\"\r\n", wifiName, password);
	HAL_UART_Transmit(ESP8266_huart,(uint8_t*)ESP_TX_buff, strlen(ESP_TX_buff), 100);
	
    memset(ESP_RX_buff, 0, ESP_RX_buff_size);
	do
	{
		HAL_UART_Receive(ESP8266_huart, (uint8_t *)ESP_RX_buff, ESP_RX_buff_size, 100);
        
        if(strstr(ESP_RX_buff, "ERROR") != NULL)
        {
           sprintf(ESP_TX_buff, "[ ERROR ] Connect to %s\n", wifiName);
           ESP8266_Error(ESP_RX_buff);
        }
        
	} while(strstr(ESP_RX_buff, "WIFI GOT IP") == NULL);
    
    #ifdef ESP8266_DEBUG
        sprintf(ESP_TX_buff, "[ OK ] Connect to %s\n", wifiName);
        PC_Send(ESP_TX_buff);
    #endif
}

void ESP8266_DisconnectFromWifi()
{
	char *str = "AT+CWQAP\r\n";
	HAL_UART_Transmit(ESP8266_huart,(uint8_t*)str, strlen(str), 100);
	
	memset(ESP_RX_buff, 0, ESP_RX_buff_size);
	
	do
	{
		HAL_UART_Receive(ESP8266_huart, (uint8_t *)ESP_RX_buff, ESP_RX_buff_size, 100);
		
	}while(strstr(ESP_RX_buff, "OK") == NULL && strstr(ESP_RX_buff, "ERROR") == NULL);
    
    if(strstr(ESP_RX_buff, "OK") == NULL)
    {
        ESP8266_Error("[ ERROR ] ESP8266_WIFI_DISCONNECT_ERROR");
    }
    
    #ifdef ESP8266_DEBUG
        PC_Send("[ OK ] ESP8266_WIFI_DISCONNECT\n");
    #endif
}

/*
    Send request
*/

void ESP8266_SendRequest(char *type, char *ip, uint8_t port, char *request)
{
    ESP8266_AT_CIPSTART(type, ip, port);
    ESP8266_AT_CIPSEND(request);
	ESP8266_AT_SendData(request);
}

void ESP8266_AT_CIPSTART(char *type, char *ip, uint8_t port)
{
    sprintf(ESP_TX_buff, "AT+CIPSTART=\"%s\",\"%s\",%d\r\n", type, ip, port);
	HAL_UART_Transmit(ESP8266_huart,(uint8_t*)ESP_TX_buff, strlen(ESP_TX_buff), 100);
	
	memset(ESP_RX_buff, 0, ESP_RX_buff_size);
	do
	{
		HAL_UART_Receive(ESP8266_huart, (uint8_t *)ESP_RX_buff, ESP_RX_buff_size, 100);
        
        if(strstr(ESP_RX_buff, "ERROR") != NULL)
            ESP8266_Error("[ ERROR ] ESP8266_AT_CIPSTART_ERROR");
 
	} while(strstr(ESP_RX_buff, "OK") == NULL);
    
    #ifdef ESP8266_DEBUG
        PC_Send("[ OK ] ESP8266_AT_CIPSTART\n");
    #endif
}

void ESP8266_AT_CIPSEND(char *request)
{
    sprintf(ESP_TX_buff, "AT+CIPSEND=%d\r\n", strlen(request) + 2);
	HAL_UART_Transmit(ESP8266_huart,(uint8_t*)ESP_TX_buff, strlen(ESP_TX_buff), 100);
	
    memset(ESP_RX_buff, 0, ESP_RX_buff_size);
	do
	{
		HAL_UART_Receive(ESP8266_huart, (uint8_t *)ESP_RX_buff, ESP_RX_buff_size, 100);
        
        if(strstr(ESP_RX_buff, "ERROR") != NULL)
        {
            PC_Send(ESP_RX_buff);
            ESP8266_Error("[ ERROR ] ESP8266_AT_CIPSEND_ERROR");
        }
            

	}while(strstr(ESP_RX_buff, "OK") == NULL);
    
    #ifdef ESP8266_DEBUG
        PC_Send("[ OK ] ESP8266_AT_CIPSEND\n");
    #endif
}

void ESP8266_AT_SendData(char *request)
{
    sprintf(ESP_TX_buff, "%s\r\n", request);
	HAL_UART_Transmit(ESP8266_huart,(uint8_t*)ESP_TX_buff, strlen(ESP_TX_buff), 100);
    
    memset(ESP_temp_buff, 0, ESP_temp_buff_size);
	do
	{
		HAL_UART_Receive(ESP8266_huart, (uint8_t *)ESP_temp_buff, ESP_temp_buff_size, 200);
        
        if(strstr(ESP_temp_buff, "ERROR") != NULL)
            ESP8266_Error("[ ERROR ] ESP8266_SEND_REQUEST_ERROR");
        
	} while(strstr(ESP_temp_buff, "OK") == NULL);
    
    #ifdef ESP8266_DEBUG
        PC_Send("[ OK ] ESP8266_SEND_REQUEST\n");
    #endif
    
    #ifdef ESP8266_SHOW_ANSWER_ON_REQUEST
        PC_Send(ESP_temp_buff);
    #endif
}
