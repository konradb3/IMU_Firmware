#include "stm32f10x.h"

#define TRIGGER_RCC_BIT RCC_APB2Periph_GPIOB
#define TRIGGER_PORT GPIOB

#define TRIGGER1_PIN GPIO_Pin_12
#define TRIGGER2_PIN GPIO_Pin_13
#define TRIGGER3_PIN GPIO_Pin_14
#define TRIGGER4_PIN GPIO_Pin_15

void trigger_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIO Periph clock enable */
	RCC_APB2PeriphClockCmd(TRIGGER_RCC_BIT, ENABLE);

	/* Configure pin in output push/pull mode */
	GPIO_InitStructure.GPIO_Pin = TRIGGER1_PIN | TRIGGER2_PIN | TRIGGER3_PIN | TRIGGER4_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(TRIGGER_PORT, &GPIO_InitStructure);
}

void trigger1_on() {
	GPIO_SetBits(TRIGGER_PORT, TRIGGER1_PIN);
}

void trigger2_on() {
	GPIO_SetBits(TRIGGER_PORT, TRIGGER2_PIN);
}

void trigger3_on() {
	GPIO_SetBits(TRIGGER_PORT, TRIGGER3_PIN);
}

void trigger4_on() {
	GPIO_SetBits(TRIGGER_PORT, TRIGGER4_PIN);
}

void trigger_all_on() {
	GPIO_SetBits(TRIGGER_PORT, TRIGGER1_PIN | TRIGGER2_PIN | TRIGGER3_PIN | TRIGGER4_PIN);
}

void trigger1_off() {
	GPIO_ResetBits(TRIGGER_PORT, TRIGGER1_PIN);
}

void trigger2_off() {
	GPIO_ResetBits(TRIGGER_PORT, TRIGGER2_PIN);
}

void trigger3_off() {
	GPIO_ResetBits(TRIGGER_PORT, TRIGGER3_PIN);
}

void trigger4_off() {
	GPIO_ResetBits(TRIGGER_PORT, TRIGGER4_PIN);
}

void trigger_all_off() {
	GPIO_ResetBits(TRIGGER_PORT, TRIGGER1_PIN | TRIGGER2_PIN | TRIGGER3_PIN | TRIGGER4_PIN);
}

void trigger_all(void) {
	TIM_PrescalerConfig()
}
