
#include "stdio.h"
#include "stm32hal.h"
#if defined(CONFIG_ARCH_STM32F4)
#include "stm32f4xx_hal.h"
#elif defined(CONFIG_ARCH_STM32F1)
#include "stm32f1xx_hal.h"
#endif
#include "gpio.h"
#include "extirq.h"

static void button_click(int i, int edge);
void SystemClock_Config(void);
static void Error_Handler(void);

int main()
{
    HAL_Init();
    /* Configure the system clock */
    SystemClock_Config();

    /* LED (GPIO As Pin) initialize */
    gpio_init(GPIO_LED1, GPIO_OUT);

    /* BUTTON initialize */
    extirq_init(GPIO_USER_BUTTON, (EXTIRQ_Callback_t)button_click, EXTIRQ_FALLING_EDGE);

    while (1) {
        gpio_write(GPIO_LED1, 1);
	HAL_Delay(1000);
        gpio_write(GPIO_LED1, 0);
	HAL_Delay(1000);
    }
}

void button_click(int i, int edge)
{
    gpio_toggle(GPIO_LED1);
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
#if defined(STM32F4xx)
  __HAL_RCC_PWR_CLK_ENABLE();
#if defined(STM32F401xx) || defined(STM32F446xx)
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
#else
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
#endif
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
#if defined(STM32F401xx) || defined(STM32F446xx)
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
#else
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
#endif
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
#if defined(STM32F401xx) || defined(STM32F446xx)
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
#else
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
#endif
#elif defined(STM32F1xx)
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
#endif
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
#if defined(STM32F401xx) || defined(STM32F446xx) || defined(STM32F103xB) || defined(STM32F103x6)
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
#else
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
#endif

#if defined(STM32F401xx) || defined(STM32F446xx) || defined(STM32F103xB) || defined(STM32F103x6)
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
#else
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
#endif
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
}
