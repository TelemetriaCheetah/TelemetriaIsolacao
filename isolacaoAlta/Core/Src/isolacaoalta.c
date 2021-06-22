/*
 * isolacaoalta.c
 *
 *  Created on: 21 de jun de 2021
 *      Author: caio
 */

#include "isolacaoalta.h"

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle)
{
	HAL_CAN_GetRxMessage(CanHandle, CAN_RX_FIFO0, &RxHeader, CANRxData);
	uint16_t id = RxHeader.StdId;
	uint8_t inicio, fim;
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
		default:
			inicio = 1;
			fim = 8;
			break;
	};

	uint8_t cont = 0;
	for(uint8_t i = inicio ; i <= fim ; i++)
	{
		saida[i] = CANRxData[cont];
		cont++;
	}
	HAL_UART_Transmit(&huart1, saida ,sizeof(saida), 100);
}

void setup()
{
	memset(saida , 0 ,sizeof(saida));
	memset(ADC_DMA, 0 ,sizeof(ADC_DMA));

	HAL_ADCEx_Calibration_Start(&hadc1);
	saida[0] = 0xFF;
	saida[UARTByteCount - 1] = 0xFE;

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

void loop()
{
	while (1)
	{
		HAL_ADC_Start_DMA(&hadc1 , ADC_DMA , 2);
	}
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	uint16_t a = (uint16_t) ADC_DMA[0];
	uint16_t b = (uint16_t) ADC_DMA[1];

	saida[33] = a & 0x00FF;
	saida[34] = a >> 8;
	saida[35] = b & 0x00FF;
	saida[36] = b >> 8;
}
