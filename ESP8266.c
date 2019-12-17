#include "ESP8266.h"

char ESP_RX_buff[ESP_RX_buff_size];
char ESP_TX_buff[ESP_TX_buff_size];

extern UART_HandleTypeDef huart2;

void ESP8266_Error(char *errorMessage)
{
    PC_Send(errorMessage);
    
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        HAL_Delay(100);
    }
}

void ESP8266_Test(void)
{
	char *str = "AT\r\n";
    
	HAL_UART_Transmit(&huart2,(uint8_t*)str, strlen(str), 100);
	
	HAL_UART_Receive(&huart2, (uint8_t *)ESP_RX_buff, ESP_RX_buff_size, 100);
    
    if(strstr(ESP_RX_buff, "OK") == NULL)
    {
        ESP8266_Error("[ ERROR ] ESP8266_TEST_ERROR");
    }
}

void ESP8266_EnableEcho()
{
    char *str2 = "ATE1\r\n";
    HAL_UART_Transmit(&huart2,(uint8_t*)str2, strlen(str2), 100);
    HAL_UART_Receive(&huart2, (uint8_t *)ESP_RX_buff, ESP_RX_buff_size, 100);
    
    if(strstr(ESP_RX_buff, "OK") == NULL)
    {
        ESP8266_Error("[ ERROR ] ESP8266_ENABLE_ECHO_ERROR");
    }
}

void ESP8266_DisableEcho()
{
    char *str1 = "ATE0\r\n";
    HAL_UART_Transmit(&huart2,(uint8_t*)str1, strlen(str1), 100);
    HAL_UART_Receive(&huart2, (uint8_t *)ESP_RX_buff, ESP_RX_buff_size, 100);
    
    if(strstr(ESP_RX_buff, "OK") == NULL)
    {
        ESP8266_Error("[ ERROR ] ESP8266_DISABLE_ECHO_ERROR");
    }
}

void ESP8266_ConnectTo(char *wifiName, char *password)
{
	sprintf(ESP_TX_buff, "AT+CWJAP_CUR=\"%s\",\"%s\"\r\n", wifiName, password);
	HAL_UART_Transmit(&huart2,(uint8_t*)ESP_TX_buff, strlen(ESP_TX_buff), 100);
	
	memset(ESP_RX_buff, 0, ESP_RX_buff_size);
	
	do
	{
		HAL_UART_Receive(&huart2, (uint8_t *)ESP_RX_buff, ESP_RX_buff_size, 100);
        
	}while(strstr(ESP_RX_buff, "OK") == NULL && strstr(ESP_RX_buff, "ERROR") == NULL);
    
    if(strstr(ESP_RX_buff, "OK") == NULL)
    {
       ESP8266_Error("[ ERROR ] Connect to MERCUSYS_7EBA\n");
    }
}

void ESP8266_DisconnectFromWifi()
{
	char *str = "AT+CWQAP\r\n";
	HAL_UART_Transmit(&huart2,(uint8_t*)str, strlen(str), 100);
	
	memset(ESP_RX_buff, 0, ESP_RX_buff_size);
	
	do
	{
		HAL_UART_Receive(&huart2, (uint8_t *)ESP_RX_buff, ESP_RX_buff_size, 100);
		
	}while(strstr(ESP_RX_buff, "OK") == NULL && strstr(ESP_RX_buff, "ERROR") == NULL);
    
    if(strstr(ESP_RX_buff, "OK") == NULL)
    {
        ESP8266_Error("[ ERROR ] ESP8266_WIFI_DISCONNECT_ERROR");
    }
}

void ESP8266_SendRequest(char *type, char *ip, uint8_t port, char *request)
{
	sprintf(ESP_TX_buff, "AT+CIPSTART=\"%s\",\"%s\",%d\r\n", type, ip, port);
	HAL_UART_Transmit(&huart2,(uint8_t*)ESP_TX_buff, strlen(ESP_TX_buff), 100);
	
	memset(ESP_RX_buff, 0, ESP_RX_buff_size);
	do
	{
		HAL_UART_Receive(&huart2, (uint8_t *)ESP_RX_buff, ESP_RX_buff_size, 100);
        
        if(strstr(ESP_RX_buff, "ERROR") != NULL)
            ESP8266_Error("[ ERROR ] ESP8266_AT_CIPSTART_ERROR");
 
	} while(strstr(ESP_RX_buff, "OK") == NULL);
	
	sprintf(ESP_TX_buff, "AT+CIPSEND=%d\r\n", strlen(request) + 2);
	HAL_UART_Transmit(&huart2,(uint8_t*)ESP_TX_buff, strlen(ESP_TX_buff), 100);
	
    memset(ESP_RX_buff, 0, ESP_RX_buff_size);
	do
	{
		HAL_UART_Receive(&huart2, (uint8_t *)ESP_RX_buff, ESP_RX_buff_size, 100);
        
        if(strstr(ESP_RX_buff, "ERROR") != NULL)
            ESP8266_Error("[ ERROR ] ESP8266_AT_CIPSEND_ERROR");

	}while(strstr(ESP_RX_buff, "OK") == NULL);

	sprintf(ESP_TX_buff, "%s\r\n", request);
	HAL_UART_Transmit(&huart2,(uint8_t*)ESP_TX_buff, strlen(ESP_TX_buff), 100);
    
    memset(ESP_RX_buff, 0, ESP_RX_buff_size);
	do
	{
		HAL_UART_Receive(&huart2, (uint8_t *)ESP_RX_buff, ESP_RX_buff_size, 100);
        
        if(strstr(ESP_RX_buff, "ERROR") != NULL)
            ESP8266_Error("[ ERROR ] ESP8266_SEND_REQUEST_ERROR");
        
	} while(strstr(ESP_RX_buff, "OK") == NULL);
    
    PC_Send(ESP_RX_buff);
}
