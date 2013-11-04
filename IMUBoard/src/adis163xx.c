/*
 * adis163xx.c
 *
 *  Created on: 2 lis 2013
 *      Author: Konrad Banachowicz
 */

#include "stm32f10x.h"
#include "adis_regs.h"

#define GPIO_SPI GPIOA

#define GPIO_Pin_SPI_MISO 	GPIO_Pin_6
#define GPIO_Pin_SPI_MOSI 	GPIO_Pin_7
#define GPIO_Pin_SPI_SCK 	GPIO_Pin_5
#define GPIO_Pin_SPI_NSS	GPIO_Pin_4

#define GPIO_Pin_RDY		GPIO_Pin_2
#define GPIO_Pin_RST		GPIO_Pin_3

void delay(unsigned int lop) {
	while(lop != 0) {
		--lop;
	}
}

void adis163xx_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef	 SPI_InitStructure;

	/* SPIx Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    /* Configure SPI pin: MISO */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SPI_MISO | GPIO_Pin_RDY;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIO_SPI, &GPIO_InitStructure);

    /* Configure SPI pin: MOSI, SCK, NSS */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SPI_MOSI | GPIO_Pin_SPI_SCK | GPIO_Pin_SPI_NSS | GPIO_Pin_RST;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIO_SPI, &GPIO_InitStructure);

    SPI_I2S_DeInit(SPI1);
    SPI_StructInit(&SPI_InitStructure);

    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;

    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);
    //SPI_SSOutputCmd(SPI1, ENABLE);
    GPIO_SetBits(GPIO_SPI, GPIO_Pin_SPI_NSS);

    GPIO_ResetBits(GPIO_SPI, GPIO_Pin_RST);
    delay(1000000);
    GPIO_SetBits(GPIO_SPI, GPIO_Pin_RST);

}

void adis_write_reg(uint8_t addr, uint16_t val) {
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) {
	}

	SPI_I2S_SendData(SPI1, (ADIS16400_WRITE_REG(addr) << 8) | (val & 0xFF));

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) {
	}

	SPI_I2S_SendData(SPI1, (ADIS16400_WRITE_REG(addr + 1) << 8) | ((val >> 8) & 0xFF));
	SPI_I2S_ReceiveData(SPI1);
}

uint16_t adis_read_reg(uint8_t addr) {
    GPIO_ResetBits(GPIO_SPI, GPIO_Pin_SPI_NSS);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) {
	}

	SPI_I2S_SendData(SPI1, ADIS16400_READ_REG(addr) << 8);

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) {
	}
	SPI_I2S_ReceiveData(SPI1);
    GPIO_SetBits(GPIO_SPI, GPIO_Pin_SPI_NSS);

	delay(1000);

    GPIO_ResetBits(GPIO_SPI, GPIO_Pin_SPI_NSS);
	SPI_I2S_SendData(SPI1, 0);

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) {
	}

	uint16_t data = SPI_I2S_ReceiveData(SPI1);
    GPIO_SetBits(GPIO_SPI, GPIO_Pin_SPI_NSS);
	return data;
}
