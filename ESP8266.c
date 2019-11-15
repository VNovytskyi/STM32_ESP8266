#include "ESP8266.h"

//Send command "AT" and return answer as true/false
bool ESP_Test()
{
	char *str = "AT\r\n";
	HAL_UART_Transmit(&huart2,(uint8_t*)str, strlen(str), 100);
	
	HAL_UART_Receive(&huart2, (uint8_t *)bool_answer, BOOL_ANSWER_SIZE, 100);
	
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
	
	HAL_UART_Receive(&huart2, (uint8_t *)bool_answer, BOOL_ANSWER_SIZE, 100);
	
	return strstr(bool_answer, "OK") == NULL? false: true;
}

//Send command to ESP via USART2: TX(PD_5), RX(PD_6), 115200
char *ESP_SendCommand(char *command)
{
	sprintf(TX_buff, "%s\r\n", command);
	
	HAL_UART_Transmit(&huart2,(uint8_t*)TX_buff, strlen(TX_buff), 100);
	
	memset(RX_buff, 0, RX_buff_size);
	
	while(strlen(RX_buff) == 0)
	{
		HAL_UART_Receive(&huart2, (uint8_t *)RX_buff, RX_buff_size, 100);
	}
	
	return RX_buff;
}