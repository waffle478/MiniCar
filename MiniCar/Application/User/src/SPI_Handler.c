/*
 * SPI_Handler.c
 *
 *  Created on: 2023. nov. 11.
 *      Author: Csabi
 */
#include "SPI_Handler.h"
#include "Statuses.h"

SPI_Data Data;

uint8_t SPI_AddMessageToQueue(SPI_message *messageAddress){
	uint8_t ret = 0;
	if (QUEUE_MAX_MESSAGES > Data.Queue.QueueLength) {
		Data.Queue.Queue[Data.Queue.QueueLength] = &messageAddress;
		Data.Queue.QueueLength++;
		ret = 1; //Message added to queue
	}

	return ret;
}


uint8_t SPI_INIT(){

}

void SPI_EmptyQueue(){

}

void SPI_AdvanceQueue(){
	for (int item = 1; item < QUEUE_MAX_MESSAGES; item++) {
		*Data.Queue.Queue[item - 1] = *Data.Queue.Queue[item];
	}
}

void SPI_Cycle(){
	switch (Data.Queue.Status) {
		case STATUS_WAITING:
			/* Enable correct module to communicate with and send messasge */
			SPI_EnableSSPin(Data.Queue.Queue[0]->ModuleType);
			HAL_SPI_Receive_IT(Data.HalSpiPort, Data.Queue.Queue[0]->CircularDataBuffer, Data.Queue.Queue[0]->MessageLenght);
			Data.Queue.Status = STATUS_TRANCIEVING;
			break;
		case STATUS_RECEIVED_MESSAGE:
			/* Disable the enabled pin and move the queue */
			SPI_DisableSSPin(Data.Queue.Queue[0]->ModuleType);
			SPI_AdvanceQueue();
			Data.Queue.Status = STATUS_WAITING;
			break;
		case STATUS_TRANCIEVING:
		default:
			/* DO NOTHING */
			break;
	}
}

void SPI_EnableSSPin(uint8_t Type){
	switch (Type) {
		case MODULE_TYPE_NRF01:
			HAL_GPIO_WritePin(SS_REGISTER_NRF01, SS_ENABLE_NRF01, RESET);
			break;
		case MODULE_TYPE_GYRO:
			HAL_GPIO_WritePin(SS_REGISTER_GYRO, SS_ENABLE_GYRO, RESET);
			break;
		case MODULE_TYPE_DISTANCE:
			HAL_GPIO_WritePin(SS_REGISTER_DISTANCE, SS_ENABLE_DISTANCE, RESET);
			break;
		default:
			/* DO NOTHING */
			break;
	}
}

void SPI_DisableSSPin(uint8_t Type){
	switch (Type) {
		case MODULE_TYPE_NRF01:
			HAL_GPIO_WritePin(SS_REGISTER_NRF01, SS_ENABLE_NRF01, SET);
			break;
		case MODULE_TYPE_GYRO:
			HAL_GPIO_WritePin(SS_REGISTER_GYRO, SS_ENABLE_GYRO, SET);
			break;
		case MODULE_TYPE_DISTANCE:
			HAL_GPIO_WritePin(SS_REGISTER_DISTANCE, SS_ENABLE_DISTANCE, SET);
			break;
		default:
			/* DO NOTHING */
			break;
	}
}

void HAL_SPI_RxCpltCallback (SPI_HandleTypeDef * hspi)
{
	Data.Queue.Status = STATUS_RECEIVED_MESSAGE;
}

void HAL_SPI_TxCpltCallback (SPI_HandleTypeDef * hspi)
{
	//HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, SET);
	//HAL_SPI_Receive(&hspi1, &RF_Data.Status, 1, DEFAULT_TIMEOUT);
	//HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, SET);
}
