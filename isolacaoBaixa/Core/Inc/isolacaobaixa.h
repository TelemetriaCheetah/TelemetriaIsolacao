/*
 * isolacaobaixa.h
 *
 *  Created on: Jun 22, 2021
 *      Author: caio
 */

#ifndef INC_ISOLACAOBAIXA_H_
#define INC_ISOLACAOBAIXA_H_
#define UARTByteCount 40

#include "stm32f1xx_hal.h"

void sendCANMessage(uint16_t id);
void setup();
void loop();


ADC_HandleTypeDef hadc1;
CAN_HandleTypeDef hcan;
TIM_HandleTypeDef htim2;
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
uint8_t UARTRxData[UARTByteCount];
CAN_TxHeaderTypeDef TxHeader;
uint32_t mailbox;
uint8_t mux;

#endif /* INC_ISOLACAOBAIXA_H_ */
