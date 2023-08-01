#ifndef  __ENCODER_H
#define  __ENCODER_H
#include "stm32f4xx.h"

/* Encoder1   :   PH10  PH11 
   Encoder2   :   PA6   PA7     */

#include "motor.h"


#define GMR_ENCODER_TO_MOTOR_RPM 30  // 1/(500*2)*500*60            rpm

typedef enum
{
	Encoder1 = 0,
	Encoder2,
	Encoder3,  
	Encoder4,
} Encoder_TypeDef;

void Encoder_All_Init(void);
void Read_All_Encoder(void);


#endif  /*__ENCODER_H */
