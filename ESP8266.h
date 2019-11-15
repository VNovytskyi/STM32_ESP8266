#ifndef ESP8266_H_
#define ESP8266_H_

#include "stm32f4xx_hal.h"
#include "stdio.h" 
#include "stdbool.h"
#include "string.h"

#define ESP_BOOL_ANSWER_SIZE 8
#define ESP_RX_buff_size 64
#define ESP_TX_buff_size 64

//Send command "AT" and return answer as true/false
bool ESP_Test(void);

//Enable/Disable echo command
bool ESP_Set_Echo(bool enableEcho);

//Send command to ESP via USART2: TX(PD_5), RX(PD_6), 115200
char *ESP_SendCommand(char *command);

//Test
#endif /* ESP8266_H_ */
