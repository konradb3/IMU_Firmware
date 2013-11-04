/*
 * can.c
 *
 *  Created on: 2 lis 2013
 *      Author: Konrad Banachowicz
 */

#include "stm32f10x.h"

#include "can.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define GPIO_Pin_CAN_RX GPIO_Pin_11
#define GPIO_Pin_CAN_TX GPIO_Pin_12
#define GPIO_CAN GPIOA

#define MSG_MASK (63 << 5)

#define MSG_IMU_MSR1 48
#define MSG_IMU_MSR2 49

#define MSG_TRIGGER_CMD 16
#define MSG_TRQ_CMD 17

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t devid;
int can_message_pending = 0;
CanRxMsg RxMessage;
/* Private function prototypes -----------------------------------------------*/
void can_process(CanRxMsg *rx);
/* Private functions ---------------------------------------------------------*/

void can_init() {
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;

	/* GPIO clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* CANx Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	/* Configure CAN pin: RX */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_CAN_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIO_CAN, &GPIO_InitStructure);

	/* Configure CAN pin: TX */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_CAN_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_CAN, &GPIO_InitStructure);

	/* CAN register init */
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;

	/* CAN Baudrate = 1MBps*/
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
	CAN_InitStructure.CAN_Prescaler = 3;
	CAN_Init(CAN1, &CAN_InitStructure);

	devid = 12;
	/* CAN filter init */

	// filter single receiver messages
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = (0 | devid) << 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (16 | 15) << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

void can_istr(void) {
	if (can_message_pending == 1) {
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
		can_process(&RxMessage);
		can_message_pending = 0;
	}
}

void can_process(CanRxMsg *rx) {
	CanTxMsg TxMessage;

	if (rx->RTR == CAN_RTR_REMOTE) {

	} else {
		if ((rx->StdId & MSG_MASK) == (MSG_TRIGGER_CMD << 5)) {
			trigger_all();
		}
	}

}
