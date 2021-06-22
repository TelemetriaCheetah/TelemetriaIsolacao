/*
 * isolacaoalta.h
 *
 *  Created on: 21 de jun de 2021
 *      Author: caio
 */

#ifndef INC_ISOLACAOALTA_H_
#define INC_ISOLACAOALTA_H_
#define UARTByteCount 40 // CAN Bytes + Analog Bytes + 2

#include "stm32f1xx_hal.h"
#include "string.h"

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;
CAN_HandleTypeDef hcan;
TIM_HandleTypeDef htim2;
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t CANRxData[8];
uint8_t saida[UARTByteCount];
uint32_t ADC_DMA[2];

void setup();
void loop();

#endif /* INC_ISOLACAOALTA_H_ */
