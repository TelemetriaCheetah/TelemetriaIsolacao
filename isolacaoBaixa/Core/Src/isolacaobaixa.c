/*
 * isolacaobaixa.c
 *
 *  Created on: Jun 22, 2021
 *      Author: caio
 */

#include "isolacaobaixa.h"

void setup()
{
	mux = 1;
	HAL_UART_Receive_DMA(&huart1 , UARTRxData ,UARTByteCount );
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	TxHeader.StdId = 0x0;
	TxHeader.ExtId = 0x0;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA ;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;

	CAN_FilterTypeDef sFilterConfig;
	sFilterConfig.FilterIdHigh = 0x0;
	sFilterConfig.FilterIdLow = 0x0;
	sFilterConfig.FilterMaskIdHigh = 0x0;
	sFilterConfig.FilterMaskIdLow = 0x0;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;
	HAL_CAN_ConfigFilter(&hcan , &sFilterConfig);
	HAL_CAN_Start(&hcan);
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(UARTRxData[0] == 0xFF && UARTRxData[UARTByteCount - 1] == 0xFE)
	{
		if(mux==1)
		{
			sendCANMessage(0x14);
			sendCANMessage(0x15);
			mux = 0;
		}
		else
		{
			sendCANMessage(0x16);
			sendCANMessage(0x18);
			mux = 1;
		}
	}
	HAL_UART_Receive_DMA(&huart1 , UARTRxData , UARTByteCount);
}

void sendCANMessage(uint16_t id)
{
	TxHeader.StdId = id;
	TxHeader.ExtId = 0x0;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA ;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;

	uint8_t inicio = 25, fim = 32;
	switch(id)
	{
		case 0x14:
			inicio = 1;
			fim = 8;
			break;
		case 0x15:
			inicio = 9;
			fim = 16;
			break;
		case 0x16:
			inicio = 17;
			fim = 24;
			break;
		case 0x17:
			inicio = 25;
			fim = 32;
			break;
		case 0x18:
			inicio = 33;
			fim = 40;
			break;
	}
	uint8_t saida[8];
	uint8_t cont = 0;
	for(uint8_t i = inicio ; i <= fim ; i++)
	{
		saida[cont] = UARTRxData[i];
		cont++;
	}
	HAL_CAN_AddTxMessage(&hcan , &TxHeader , saida , &mailbox);
}

void loop()
{
	while(1)
	{
		uint16_t a = UARTRxData[33] | UARTRxData[34]<<8;
		uint16_t b = UARTRxData[36] | UARTRxData[35]<<8;
		TIM2->CCR3 = a;
	}
}


