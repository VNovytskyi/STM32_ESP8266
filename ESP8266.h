#ifndef ESP8266_H_
#define ESP8266_H_

//Send command "AT" and return answer as true/false
bool ESP_Test();

//Enable/Disable echo command
bool ESP_Set_Echo(bool enableEcho);

//Send command to ESP via USART2: TX(PD_5), RX(PD_6), 115200
char *ESP_SendCommand(char *command);






#endif /* ESP8266_H_ */