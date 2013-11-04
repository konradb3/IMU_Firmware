/*
 * can.h
 *
 *  Created on: 2 lis 2013
 *      Author: konrad
 */

#ifndef CAN_H_
#define CAN_H_

#include "stm32f10x.h"

void can_init(void);
void can_istr(void);
void can_process(CanRxMsg *rx);

#endif /* CAN_H_ */
