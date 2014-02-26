/*
 * stateMachine.h
 *
 *  Created on: Jan 14, 2014
 *      Author: Jeff
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#define IDLE 0
#define READY_DEVICE_SELECT 5 //select either coarse 5504 = DC1 or 7512 = DC7512
#define READY_MS_CHAR1 4 //most significant character EG the 7 in 0x7004
#define READY_CHAR1_3 3
#define READY_CHAR1_2 2
#define READY_LS_CHAR1 1 //least significant character EG the 4 in 0x7004

#define READY_MS_CHAR2 14 //most significant character EG the 7 in 0x7004
#define READY_CHAR2_3 13
#define READY_CHAR2_2 12
#define READY_LS_CHAR2 11 //least significant character EG the 4 in 0x7004



#define SOH 1
#define EOT 4
#define DC1 17
#define DC2	18

#endif /* STATEMACHINE_H_ */
