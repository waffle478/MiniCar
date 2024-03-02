/*
 * SPI_Handler.c
 *
 *  Created on: 2023. nov. 11.
 *      Author: Csabi
 */
#include "SPI_Handler.h"
#include <main.h>
#include <stdio.h>
#include <string.h>

SPI_Data Data;

uint8_t SPI_AddMessageToQueue(SPI_message *message){
	uint8_t ret = 0;
	if (QUEUE_MAX_MESSAGES > Data.Queue.QueueLength) {
		memcpy(Data.Queue.Queue[Data.Queue.QueueLength].CircularDataBuffer, message->CircularDataBuffer, 32);

		memcpy(&Data.Queue.Queue[Data.Queue.QueueLength], message, sizeof(SPI_message));
		Data.Queue.QueueLength++;
		ret = 1; //Message added to queue
	}

	return ret;
}


uint8_t SPI_INIT(SPI_HandleTypeDef *port){
	Data.HalSpiPort = port;
	Data.Queue.QueueLength = 0;
	return 0;
}

void SPI_EmptyQueue(){

}

void SPI_AdvanceQueue(){
	for (int item = 1; item < QUEUE_MAX_MESSAGES; item++) {
		Data.Queue.Queue[item - 1] = Data.Queue.Queue[item];
	}
	SPI_resetMessage(&Data.Queue.Queue[Data.Queue.QueueLength - 1]);
	Data.Queue.QueueLength--;
}

void SPI_Cycle(){
	switch (Data.Queue.Status) {
		case STATUS_WAITING:
			/* Enable correct module to communicate with and send message */
			if (Data.Queue.QueueLength != 0 && CURRENT_QUEUE_ELEMENT.MessageLenght != 0) {
				SPI_EnableSSPin(CURRENT_QUEUE_ELEMENT.Module.ModuleType);
				HAL_SPI_Receive_IT(Data.HalSpiPort, CURRENT_QUEUE_ELEMENT.CircularDataBuffer, CURRENT_QUEUE_ELEMENT.MessageLenght);
				Data.Queue.Status = STATUS_TRANCIEVING;
			}
			break;
		case STATUS_RECEIVED_MESSAGE:
			/* If the next message is related to the currently set message then transfer the needed data */
			if (TRUE == CURRENT_QUEUE_ELEMENT.RelatedTo.EarlierMessage) {
				/* 					The related data and the selected byte				 	OR						The earlier sent data and its related byte				*/
				NEXT_QUEUE_ELEMENT.CircularDataBuffer[CURRENT_QUEUE_ELEMENT.RelatedTo.Byte] |= CURRENT_QUEUE_ELEMENT.CircularDataBuffer[CURRENT_QUEUE_ELEMENT.RelatedTo.Byte];
			}

			/* Call queue element's function and advance queue */
			if (TRUE == CURRENT_QUEUE_ELEMENT.Module.FunctionExecution) {
				CURRENT_QUEUE_ELEMENT.Module.ModuleFunction(CURRENT_QUEUE_ELEMENT);
			}

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
			HAL_GPIO_WritePin(SPI_NSS_RF_MODULE_GPIO_Port, SPI_NSS_RF_MODULE_Pin, RESET);
			break;

#ifdef GYRO
		case MODULE_TYPE_GYRO:
			HAL_GPIO_WritePin(SPI_NSS_GYRO_MODULE_GPIO_Port, SPI_NSS_GYRO_MODULE_Pin, RESET);
			break;
#endif

#if DISTANCE == 1
		case MODULE_TYPE_DISTANCE:
			HAL_GPIO_WritePin(SS_REGISTER_DISTANCE, SS_ENABLE_DISTANCE, RESET);
			break;
#endif
		default:
			/* DO NOTHING */
			break;
	}
}

void SPI_DisableSSPin(uint8_t Type){
	switch (Type) {
		case MODULE_TYPE_NRF01:
			HAL_GPIO_WritePin(SPI_NSS_RF_MODULE_GPIO_Port, SPI_NSS_RF_MODULE_Pin, SET);
			break;
#if GYRO == 1
		case MODULE_TYPE_GYRO:
			HAL_GPIO_WritePin(SPI_NSS_GYRO_MODULE_GPIO_Port, SPI_NSS_GYRO_MODULE_Pin, SET);
			break;
#endif

#if DISTANCE == 1
		case MODULE_TYPE_DISTANCE:
			HAL_GPIO_WritePin(SS_REGISTER_DISTANCE, SS_ENABLE_DISTANCE, SET);
			break;
#endif
		default:
			/* DO NOTHING */
			break;
	}
}

void SPI_resetMessage(SPI_message *message)
{
	/* TODO: Use other way to clear an array. */
	for (int i = 0; i < 32; ++i) {
		message->CircularDataBuffer[i] = 0u;
	}
	message->MessageLenght = 0;
	message->Module.ModuleType = 0;
	message->Module.FunctionExecution = 0;
	message->RelatedTo.EarlierMessage = 0;
	message->RelatedTo.Byte = 0;
}






void HAL_SPI_RxCpltCallback (SPI_HandleTypeDef * hspi)
{
	/* Disable the enabled pin and move the queue */
	SPI_DisableSSPin(CURRENT_QUEUE_ELEMENT.Module.ModuleType);
	Data.Queue.Status = STATUS_RECEIVED_MESSAGE;
}

void HAL_SPI_TxCpltCallback (SPI_HandleTypeDef * hspi)
{
	//HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, SET);
	//HAL_SPI_Receive(&hspi1, &RF_Data.Status, 1, DEFAULT_TIMEOUT);
	//HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, SET);
}
