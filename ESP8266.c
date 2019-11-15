#include "ESP8266.h"

char bool_answer[ESP_BOOL_ANSWER_SIZE];
char ESP_RX_buff[ESP_RX_buff_size];
char ESP_TX_buff[ESP_TX_buff_size];

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

//Send command "AT" and return answer as true/false
bool ESP_Test(void)
{
	char *str = "AT\r\n";
	HAL_UART_Transmit(&huart2,(uint8_t*)str, strlen(str), 100);
	
	HAL_UART_Receive(&huart2, (uint8_t *)bool_answer, ESP_BOOL_ANSWER_SIZE, 100);
	
	return strstr(bool_answer, "OK") == NULL? false: true;
}

//Enable/Disable echo of commands
bool ESP_Set_Echo(bool enableEcho)
{
	char *str1 = "ATE0\r\n";
	char *str2 = "ATE1\r\n";
	
	if(enableEcho)
		HAL_UART_Transmit(&huart2,(uint8_t*)str2, strlen(str2), 100);
	else
		HAL_UART_Transmit(&huart2,(uint8_t*)str1, strlen(str1), 100);
	
	HAL_UART_Receive(&huart2, (uint8_t *)bool_answer, ESP_BOOL_ANSWER_SIZE, 100);
	
	return strstr(bool_answer, "OK") == NULL? false: true;
}

//Send command to ESP via USART2: TX(PD_5), RX(PD_6), 115200
char *ESP_SendCommand(char *command)
{
	sprintf(ESP_TX_buff, "%s\r\n", command);
	
	HAL_UART_Transmit(&huart2,(uint8_t*)ESP_TX_buff, strlen(ESP_TX_buff), 100);
	
	memset(ESP_RX_buff, 0, ESP_RX_buff_size);
	
	while(strlen(ESP_RX_buff) == 0)
	{
		HAL_UART_Receive(&huart2, (uint8_t *)ESP_RX_buff, ESP_RX_buff_size, 100);
	}
	
	return ESP_RX_buff;
}
