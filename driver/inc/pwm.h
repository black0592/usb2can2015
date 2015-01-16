#ifndef _PWM_H_
#define _PWM_H_

#define T_PWM (1440)//25kHz
//#define T_PWM (4500)//8kHz
//#define T_PWM (2250)//8kHz
//#define T_PWM (36000)//1kHz
/*
* PWM½Ó¿Ú:
* PWM1->PA9->TIM1 CH2
* PWM2->PA10->TIM1 CH3
* PWM3->PA8->TIM1 CH1
* ENA->PB14
* ENB->PB15
* ENC->PB13
*/
#define PWMA_EN() 	GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET)
#define PWMA_DIS() 	GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET)
#define PWMB_EN() 	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET)
#define PWMB_DIS() 	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET)
#define PWMC_EN() 	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET)
#define PWMC_DIS() 	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET)

#define PWM1_SET(TIM1,pwm) TIM_SetCompare1(TIM1,pwm)
#define PWM2_SET(TIM1,pwm) TIM_SetCompare2(TIM1,pwm)
#define PWM3_SET(TIM1,pwm) TIM_SetCompare3(TIM1,pwm)

#define PWMAB(TIM1,pwm) {PWM1_SET(TIM1,pwm);PWM2_SET(TIM1,0);PWM3_SET(TIM1,0);PWMA_EN();PWMB_EN();PWMC_DIS();}
#define PWMBA(TIM1,pwm) {PWM1_SET(TIM1,0);PWM2_SET(TIM1,pwm);PWM3_SET(TIM1,0);PWMA_EN();PWMB_EN();PWMC_DIS();}
#define PWMBC(TIM1,pwm) {PWM1_SET(TIM1,0);PWM2_SET(TIM1,pwm);PWM3_SET(TIM1,0);PWMA_DIS();PWMB_EN();PWMC_EN();}
#define PWMCB(TIM1,pwm) {PWM1_SET(TIM1,0);PWM2_SET(TIM1,0);PWM3_SET(TIM1,pwm);PWMA_DIS();PWMB_EN();PWMC_EN();}
#define PWMAC(TIM1,pwm) {PWM1_SET(TIM1,pwm);PWM2_SET(TIM1,0);PWM3_SET(TIM1,0);PWMA_EN();PWMB_DIS();PWMC_EN();}
#define PWMCA(TIM1,pwm) {PWM1_SET(TIM1,0);PWM2_SET(TIM1,0);PWM3_SET(TIM1,pwm);PWMA_EN();PWMB_DIS();PWMC_EN();}

void pwmConfiguration( void );
//void PWMAB(int pwm);
#endif
