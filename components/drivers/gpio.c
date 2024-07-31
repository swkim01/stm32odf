/*
 *----------------------------------------------------------------------
 * Copyright (C) Seong-Woo Kim, 2016
 * 
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of
 * this software and associated documentation files
 * (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and
 * to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice
 * shall be included in all copies or substantial portions
 * of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *----------------------------------------------------------------------
 */
#include "gpio.h"

#if defined(CONFIG_GPIO_DRIVER_MODULE_ENABLED)

/* Private variables */
static uint16_t gpio_outputpins[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
static uint16_t gpio_inputpins[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
static uint16_t gpio_afpins[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};

/* Private functions */
void gpio_enable_clock(GPIO_TypeDef* GPIOx);
void gpio_disable_clock(GPIO_TypeDef* GPIOx);
void gpio_alternate_init(GPIO_Pin_t GPIO_Pin, GPIO_Mode_t GPIO_Mode, uint8_t GPIO_Alternate);

void gpio_init(GPIO_Pin_t GPIO_Pin, GPIO_Mode_t GPIO_Mode) {
    gpio_alternate_init(GPIO_Pin, GPIO_Mode, 0);
}

void gpio_alternate_init(GPIO_Pin_t GPIO_Pin, GPIO_Mode_t GPIO_Mode, uint8_t GPIO_Alternate) {
    uint8_t ptr = gpio_get_port_offset(GPIO_Pin);
    GPIO_TypeDef *GPIOx = gpio_get_port_base(GPIO_Pin);
    GPIO_InitTypeDef gpio;
    
    assert_param(IS_GPIO_PIN_NUMBER(GPIO_Pin));

    /* Enable clock for GPIO */
    gpio_enable_clock(GPIOx);

    gpio.Pin = (uint32_t)GPIO_REG_VALUE(GPIO_Pin);
    if (GPIO_Mode == GPIO_OUT) {
        gpio.Mode = GPIO_MODE_OUTPUT_PP;
        gpio_outputpins[ptr] |= GPIO_REG_VALUE(GPIO_Pin);
        gpio_inputpins[ptr] &= ~GPIO_REG_VALUE(GPIO_Pin);
        gpio_afpins[ptr] &= ~GPIO_REG_VALUE(GPIO_Pin);
    } else if (GPIO_Mode == GPIO_IN) {
        gpio.Mode = GPIO_MODE_INPUT;
        gpio_inputpins[ptr] |= GPIO_REG_VALUE(GPIO_Pin);
        gpio_outputpins[ptr] &= ~GPIO_REG_VALUE(GPIO_Pin);
        gpio_afpins[ptr] &= ~GPIO_REG_VALUE(GPIO_Pin);
    } else if (GPIO_Mode == GPIO_AF || GPIO_Mode == GPIO_OD) {
        if (GPIO_Mode == GPIO_AF)
            gpio.Mode = GPIO_MODE_AF_PP;
        else
            gpio.Mode = GPIO_MODE_AF_OD;
        gpio_afpins[ptr] |= GPIO_REG_VALUE(GPIO_Pin);
        gpio_inputpins[ptr] &= ~GPIO_REG_VALUE(GPIO_Pin);
        gpio_outputpins[ptr] &= ~GPIO_REG_VALUE(GPIO_Pin);
    } else {
        gpio.Mode = (uint32_t) GPIO_Mode;
    }
#if !defined(CONFIG_ARCH_STM32F1)
    gpio.Alternate = GPIO_Alternate;
#else
/*
    if (GPIO_Mode & GPIO_EXTI) {
        int8_t edge = 3 - ((GPIO_Mode >> 20) & 0xF);
	if (edge == GPIO_PM_PULLUP)
            gpio.Mode = GPIO_MODE_IT_FALLING;
	else if (edge == GPIO_PM_PULLDOWN)
            gpio.Mode = GPIO_MODE_IT_RISING;
	else
            gpio.Mode = GPIO_MODE_IT_RISING_FALLING;
    }
*/
#endif

    /* default mode */
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;

    /* Do initialization */
    HAL_GPIO_Init(GPIOx, &gpio);
}

void gpio_deinit(GPIO_Pin_t GPIO_Pin) {
    uint8_t ptr = gpio_get_port_offset(GPIO_Pin);

    assert_param(IS_GPIO_PIN_NUMBER(GPIO_Pin));

    HAL_GPIO_DeInit(gpio_get_port_base(GPIO_Pin), GPIO_REG_VALUE(GPIO_Pin));
    gpio_outputpins[ptr] &= ~GPIO_REG_VALUE(GPIO_Pin);
    gpio_inputpins[ptr] &= ~GPIO_REG_VALUE(GPIO_Pin);
}

uint16_t gpio_get_port_source(GPIO_TypeDef* GPIOx) {
    /* Get port source number */
    /* Offset from GPIOA  Difference between 2 GPIO addresses */
    return ((uint32_t)GPIOx - (GPIOA_BASE)) / ((GPIOB_BASE) - (GPIOA_BASE));
}

uint8_t gpio_get_port_offset(GPIO_Pin_t Pin) {
    /* Get port source number */
    /* Port Offset from pin type */
    return ((uint8_t)((Pin & 0x70) >> 4));
}

GPIO_TypeDef* gpio_get_port_base(GPIO_Pin_t Pin) {
    /* Get port base address */
    /* GPIOA address + portnumber * difference between 2 GPIO addresses */
    return ((GPIO_TypeDef*)(GPIOA_BASE + ((Pin & 0x70) >> 4)*((GPIOB_BASE) - (GPIOA_BASE))));
}

void gpio_set_pinmode(GPIO_Pin_t GPIO_Pin, GPIO_PullMode_t GPIO_PullMode) {
    GPIO_TypeDef *GPIOx = gpio_get_port_base(GPIO_Pin);

    assert_param(IS_GPIO_PIN_NUMBER(GPIO_Pin));

#if defined(CONFIG_ARCH_STM32F1)
    /*
    uint8_t pin = GPIO_PIN_VALUE(GPIO_Pin);
    uint32_t config = 0x00u;
    uint32_t offset;

    offset = pin < 8 ?  pin * 4 : (pin - 8) * 4;
    if (GPIO_PullMode == GPIO_PM_NOPULL)
        config = 0x04;
    else if (GPIO_PullMode == GPIO_PM_PULLUP) {
        config = 0x08;
	GPIOx->BSRR = GPIO_REG_VALUE(GPIO_Pin);
    } else {
        config = 0x08;
	GPIOx->BRR = GPIO_REG_VALUE(GPIO_Pin);
    }
    if (pin < 8) {
        GPIOx->CRL &= (uint32_t)(~((GPIO_CRL_MODE0 | GPIO_CRL_CNF0) << offset));
        GPIOx->CRL |= (uint32_t)(config << offset);
    }
    else {
        GPIOx->CRH &= (uint32_t)(~((GPIO_CRL_MODE0 | GPIO_CRL_CNF0) << offset));
        GPIOx->CRH |= (uint32_t)(config << offset);
    }
    */
#else
    /* Set GPIO PUPD register */
    GPIOx->PUPDR &= (uint32_t)(~(0x03 << (2 * (GPIO_Pin & 0xF))));
    GPIOx->PUPDR |= (uint32_t)(GPIO_PullMode << (2 * (GPIO_Pin & 0xF)));
#endif
}

void gpio_write(GPIO_Pin_t GPIO_Pin, uint16_t PinState)
{
    GPIO_TypeDef *GPIOx = gpio_get_port_base(GPIO_Pin);

    /* Check the parameters */
    assert_param(IS_GPIO_PIN_NUMBER(GPIO_Pin));
    assert_param(IS_GPIO_PIN_OUTPUT(GPIO_Pin));

    if (PinState != 0) {
        GPIOx->BSRR = (uint32_t)GPIO_REG_VALUE(GPIO_Pin);
    } else {
        GPIOx->BSRR = (uint32_t)((uint32_t)GPIO_REG_VALUE(GPIO_Pin) << 16U);
    }
}

void gpio_toggle(GPIO_Pin_t GPIO_Pin)
{
    GPIO_TypeDef *GPIOx = gpio_get_port_base(GPIO_Pin);

    /* Check the parameters */
    assert_param(IS_GPIO_PIN_NUMBER(GPIO_Pin));
    assert_param(IS_GPIO_PIN_OUTPUT(GPIO_Pin));

    GPIOx->ODR ^= (uint32_t)GPIO_REG_VALUE(GPIO_Pin);
}

uint16_t gpio_read(GPIO_Pin_t GPIO_Pin)
{
    GPIO_TypeDef *GPIOx = gpio_get_port_base(GPIO_Pin);

    /* Check the parameters */
    assert_param(IS_GPIO_PIN_NUMBER(GPIO_Pin));

    if (IS_GPIO_PIN_OUTPUT(GPIO_Pin)) {
        if ((GPIOx->ODR & (uint32_t)GPIO_REG_VALUE(GPIO_Pin)) != (uint32_t)GPIO_PIN_RESET) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if ((GPIOx->IDR & (uint32_t)GPIO_REG_VALUE(GPIO_Pin)) != (uint32_t)GPIO_PIN_RESET) {
            return 1;
        } else {
            return 0;
        }
    }
}

/* Private functions */
void gpio_enable_clock(GPIO_TypeDef* GPIOx) {
    switch ((uint32_t)GPIOx) {
    case GPIOA_BASE:
        __HAL_RCC_GPIOA_CLK_ENABLE();
        break;
    case GPIOB_BASE:
        __HAL_RCC_GPIOB_CLK_ENABLE();
        break;
    case GPIOC_BASE:
        __HAL_RCC_GPIOC_CLK_ENABLE();
        break;
#if defined GPIOD_BASE
    case GPIOD_BASE:
        __HAL_RCC_GPIOD_CLK_ENABLE();
        break;
#endif
#if defined GPIOE_BASE
    case GPIOE_BASE:
        __HAL_RCC_GPIOE_CLK_ENABLE();
        break;
#endif
#if defined GPIOF_BASE
    case GPIOF_BASE:
        __HAL_RCC_GPIOF_CLK_ENABLE();
        break;
#endif
#if defined GPIOG_BASE
    case GPIOG_BASE:
        __HAL_RCC_GPIOG_CLK_ENABLE();
        break;
#endif
#if defined GPIOH_BASE
    case GPIOH_BASE:
        __HAL_RCC_GPIOH_CLK_ENABLE();
        break;
#endif
#if defined GPIOI_BASE
    case GPIOI_BASE:
        __HAL_RCC_GPIOI_CLK_ENABLE();
        break;
#endif
#if defined GPIOJ_BASE
    case GPIOJ_BASE:
        __HAL_RCC_GPIOJ_CLK_ENABLE();
        break;
#endif
#if defined GPIOK_BASE
    case GPIOK_BASE:
        __HAL_RCC_GPIOK_CLK_ENABLE();
        break;
#endif
    default:
        //error("EnableClock error: wrong port number.");
        break;
    }
}

void gpio_disable_clock(GPIO_TypeDef* GPIOx) {
    switch ((uint32_t)GPIOx) {
    case GPIOA_BASE:
        __HAL_RCC_GPIOA_CLK_DISABLE();
        break;
    case GPIOB_BASE:
        __HAL_RCC_GPIOB_CLK_DISABLE();
        break;
    case GPIOC_BASE:
        __HAL_RCC_GPIOC_CLK_DISABLE();
        break;
#if defined GPIOD_BASE
    case GPIOD_BASE:
        __HAL_RCC_GPIOD_CLK_DISABLE();
        break;
#endif
#if defined GPIOE_BASE
    case GPIOE_BASE:
        __HAL_RCC_GPIOE_CLK_DISABLE();
        break;
#endif
#if defined GPIOF_BASE
    case GPIOF_BASE:
        __HAL_RCC_GPIOF_CLK_DISABLE();
        break;
#endif
#if defined GPIOG_BASE
    case GPIOG_BASE:
        __HAL_RCC_GPIOG_CLK_DISABLE();
        break;
#endif
#if defined GPIOH_BASE
    case GPIOH_BASE:
        __HAL_RCC_GPIOH_CLK_DISABLE();
        break;
#endif
#if defined GPIOI_BASE
    case GPIOI_BASE:
        __HAL_RCC_GPIOI_CLK_DISABLE();
        break;
#endif
#if defined GPIOJ_BASE
    case GPIOJ_BASE:
        __HAL_RCC_GPIOJ_CLK_DISABLE();
        break;
#endif
#if defined GPIOK_BASE
    case GPIOK_BASE:
        __HAL_RCC_GPIOK_CLK_DISABLE();
        break;
#endif
    default:
        //error("DisableClock error: wrong port number.");
        break;
    }
}

uint16_t gpio_get_usedpins(GPIO_TypeDef* GPIOx) {
    /* Return used */
    return gpio_outputpins[gpio_get_port_source(GPIOx)] | gpio_inputpins[gpio_get_port_source(GPIOx)] | gpio_afpins[gpio_get_port_source(GPIOx)];
}

uint16_t gpio_get_freepins(GPIO_TypeDef* GPIOx) {
    /* Return free pins */
    return ~(gpio_outputpins[gpio_get_port_source(GPIOx)] | gpio_inputpins[gpio_get_port_source(GPIOx)] | gpio_afpins[gpio_get_port_source(GPIOx)]);
}
#endif //defined(CONFIG_GPIO_DRIVER_MODULE_ENABLED)
