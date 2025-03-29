

#ifndef _STM8CONFIG_
#define _STM8CONFIG_

#include "stm8s.h"

#define OFFSET_ODR		0x00
#define OFFSET_IDR		0x01
#define OFFSET_DDR		0x02
#define OFFSET_CR1		0x03
#define OFFSET_CR2		0x04


#define GPIOA_BASE		0x5000
#define GPIOB_BASE		0x5005
#define GPIOC_BASE		0x500A
#define GPIOD_BASE		0x500F
#define GPIOE_BASE		0x5014
#define GPIOF_BASE		0x5019
#define GPIOG_BASE		0x501E
#define GPIOH_BASE		0x5023
#define GPIOI_BASE		0x5028

#define GPIO_DDR_OUTPUT		1
#define GPIO_DDR_INPUT		0

#define GPIO_CR1_PUSHPULL	1
#define GPIO_CR1_OPENDRAIN	0

#define GPIO_CR2_IN_DISEXTI	0
#define GPIO_CR2_IN_ENEXTI	1

#define GPIO_CR2_OUT_2MHZ	0
#define GPIO_CR2_OUT_10MHZ	1


struct STM_PORT
{
	__IO unsigned char PIN0 :1;
	__IO unsigned char PIN1 :1;
	__IO unsigned char PIN2 :1;
	__IO unsigned char PIN3 :1;
	__IO unsigned char PIN4 :1;
	__IO unsigned char PIN5 :1;
	__IO unsigned char PIN6 :1;
	__IO unsigned char PIN7 :1;
};


#define	GPIOA_ODR (*(volatile struct STM_PORT *) (GPIOA_BASE + OFFSET_ODR))
#define	GPIOA_IDR (*(volatile struct STM_PORT *) (GPIOA_BASE + OFFSET_IDR))
#define	GPIOA_DDR (*(volatile struct STM_PORT *) (GPIOA_BASE + OFFSET_DDR))
#define	GPIOA_CR1 (*(volatile struct STM_PORT *) (GPIOA_BASE + OFFSET_CR1))
#define	GPIOA_CR2 (*(volatile struct STM_PORT *) (GPIOA_BASE + OFFSET_CR2))


#define	GPIOB_ODR (*(volatile struct STM_PORT *) (GPIOB_BASE + OFFSET_ODR))
#define	GPIOB_IDR (*(volatile struct STM_PORT *) (GPIOB_BASE + OFFSET_IDR))
#define	GPIOB_DDR (*(volatile struct STM_PORT *) (GPIOB_BASE + OFFSET_DDR))
#define	GPIOB_CR1 (*(volatile struct STM_PORT *) (GPIOB_BASE + OFFSET_CR1))
#define	GPIOB_CR2 (*(volatile struct STM_PORT *) (GPIOB_BASE + OFFSET_CR2))


#define	GPIOC_ODR (*(volatile struct STM_PORT *) (GPIOC_BASE + OFFSET_ODR))
#define	GPIOC_IDR (*(volatile struct STM_PORT *) (GPIOC_BASE + OFFSET_IDR))
#define	GPIOC_DDR (*(volatile struct STM_PORT *) (GPIOC_BASE + OFFSET_DDR))
#define	GPIOC_CR1 (*(volatile struct STM_PORT *) (GPIOC_BASE + OFFSET_CR1))
#define	GPIOC_CR2 (*(volatile struct STM_PORT *) (GPIOC_BASE + OFFSET_CR2))

#define	GPIOD_ODR (*(volatile struct STM_PORT *) (GPIOD_BASE + OFFSET_ODR))
#define	GPIOD_IDR (*(volatile struct STM_PORT *) (GPIOD_BASE + OFFSET_IDR))
#define	GPIOD_DDR (*(volatile struct STM_PORT *) (GPIOD_BASE + OFFSET_DDR))
#define	GPIOD_CR1 (*(volatile struct STM_PORT *) (GPIOD_BASE + OFFSET_CR1))
#define	GPIOD_CR2 (*(volatile struct STM_PORT *) (GPIOD_BASE + OFFSET_CR2))


#define	GPIOE_ODR (*(volatile struct STM_PORT *) (GPIOE_BASE + OFFSET_ODR))
#define	GPIOE_IDR (*(volatile struct STM_PORT *) (GPIOE_BASE + OFFSET_IDR))
#define	GPIOE_DDR (*(volatile struct STM_PORT *) (GPIOE_BASE + OFFSET_DDR))
#define	GPIOE_CR1 (*(volatile struct STM_PORT *) (GPIOE_BASE + OFFSET_CR1))
#define	GPIOE_CR2 (*(volatile struct STM_PORT *) (GPIOE_BASE + OFFSET_CR2))

#define	GPIOF_ODR (*(volatile struct STM_PORT *) (GPIOF_BASE + OFFSET_ODR))
#define	GPIOF_IDR (*(volatile struct STM_PORT *) (GPIOF_BASE + OFFSET_IDR))
#define	GPIOF_DDR (*(volatile struct STM_PORT *) (GPIOF_BASE + OFFSET_DDR))
#define	GPIOF_CR1 (*(volatile struct STM_PORT *) (GPIOF_BASE + OFFSET_CR1))
#define	GPIOF_CR2 (*(volatile struct STM_PORT *) (GPIOF_BASE + OFFSET_CR2))

#define	GPIOG_ODR (*(volatile struct STM_PORT *) (GPIOG_BASE + OFFSET_ODR))
#define	GPIOG_IDR (*(volatile struct STM_PORT *) (GPIOG_BASE + OFFSET_IDR))
#define	GPIOG_DDR (*(volatile struct STM_PORT *) (GPIOG_BASE + OFFSET_DDR))
#define	GPIOG_CR1 (*(volatile struct STM_PORT *) (GPIOG_BASE + OFFSET_CR1))
#define	GPIOG_CR2 (*(volatile struct STM_PORT *) (GPIOG_BASE + OFFSET_CR2))

#define	GPIOH_ODR (*(volatile struct STM_PORT *) (GPIOH_BASE + OFFSET_ODR))
#define	GPIOH_IDR (*(volatile struct STM_PORT *) (GPIOH_BASE + OFFSET_IDR))
#define	GPIOH_DDR (*(volatile struct STM_PORT *) (GPIOH_BASE + OFFSET_DDR))
#define	GPIOH_CR1 (*(volatile struct STM_PORT *) (GPIOH_BASE + OFFSET_CR1))
#define	GPIOH_CR2 (*(volatile struct STM_PORT *) (GPIOH_BASE + OFFSET_CR2))

#define	GPIOI_ODR (*(volatile struct STM_PORT *) (GPIOI_BASE + OFFSET_ODR))
#define	GPIOI_IDR (*(volatile struct STM_PORT *) (GPIOI_BASE + OFFSET_IDR))
#define	GPIOI_DDR (*(volatile struct STM_PORT *) (GPIOI_BASE + OFFSET_DDR))
#define	GPIOI_CR1 (*(volatile struct STM_PORT *) (GPIOI_BASE + OFFSET_CR1))
#define	GPIOI_CR2 (*(volatile struct STM_PORT *) (GPIOI_BASE + OFFSET_CR2))







#endif