	/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bkit_config.h"
#include "hw_driver.h"
#include "lcd.h"
#include <stdio.h>
#include "bkit_app.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART3_UART_Init();
  MX_FSMC_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_I2C2_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  hw_init();

  lcd_init();
    lcd_clear(BLACK);


    lcd_show_string(10, 10,  "=== IoT SLAVE ===", GREEN, BLACK, 24, 0);

    //MODE
    #if (BKIT_PHY_INTERFACE == PHY_UART)
        lcd_show_string(10, 45,  "Link: UART (Active)", CYAN, BLACK, 16, 0);
    #else
        lcd_show_string(10, 45,  "Link: I2C (Active) ", CYAN, BLACK, 16, 0);
    #endif


    lcd_show_string(10, 100, "DEVICE TELEMETRY:", YELLOW, BLACK, 16, 0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  sensor_data_t received_data;


	        if (bkit_receive_message(&received_data)) {
	            char buf[64];



	            // ID
	            sprintf(buf, "Node ID  : %lu   ", received_data.sensor_id);
	            lcd_show_string(20, 125, buf, WHITE, BLACK, 24, 0);

	            // NHIET DO
	            sprintf(buf, "Temp : %.2f C ", received_data.temperature);
	            lcd_show_string(20, 155, buf, RED, BLACK, 24, 0);

	            // THOI GIAN
	            sprintf(buf, "Time : %lu ms ", received_data.timestamp);
	            lcd_show_string(20, 185, buf, GRAY, BLACK, 16, 0);

	            // Nếu có humidity:
	            // sprintf(buf, "Humi     : %.1f %% ", received_data.humidity);
	            // lcd_show_string(20, 215, buf, BLUE, BLACK, 24, 0);

	            // EFFECT
	            lcd_show_string(200, 10, "*", RED, BLACK, 24, 0);
	            HAL_Delay(50);
	            lcd_show_string(200, 10, " ", BLACK, BLACK, 24, 0);
	        }
	        else {

	        }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
extern DMA_HandleTypeDef hdma_usart6_rx; // Khai báo extern để dùng handle
extern UART_HandleTypeDef huart6;
extern I2C_HandleTypeDef hi2c2;
extern volatile uint16_t rx_tail;// new

// Buffer và biến nằm bên hw_driver.c, ta cần extern để truy cập
// (Hoặc tốt nhất là include hw_driver.h và đảm bảo biến dma_rx_buffer được chia sẻ)
// Để đơn giản test, bạn khai báo lại biến extern ở đây:
#define RX_BUFFER_SIZE 256
extern uint8_t dma_rx_buffer[RX_BUFFER_SIZE];

// 1. XỬ LÝ LỖI UART
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART6) {
        __HAL_UART_CLEAR_OREFLAG(huart);
        __HAL_UART_CLEAR_NEFLAG(huart);
        __HAL_UART_CLEAR_FEFLAG(huart);
        __HAL_UART_CLEAR_PEFLAG(huart);
        HAL_UART_Receive_DMA(&huart6, (uint8_t*)dma_rx_buffer, RX_BUFFER_SIZE);
    }
}

/* --- CÁC CALLBACK QUAN TRỌNG CỦA I2C SLAVE --- */

// 1. Khi Master gọi đúng tên -> BẮT ĐẦU NHẬN
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode) {
    if (hi2c->Instance == I2C2) {
        // [QUAN TRỌNG] Reset vị trí đọc về 0 vì DMA I2C luôn ghi từ đầu buffer
        rx_tail = 0;

        // Mở cửa DMA để hứng dữ liệu
        HAL_I2C_Slave_Receive_DMA(&hi2c2, (uint8_t*)dma_rx_buffer, RX_BUFFER_SIZE);
    }
}

// 2. Khi Master gửi xong và phát xung STOP (Hoặc Slave buffer đầy)
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {
    if (hi2c->Instance == I2C2) {
        // Nhận xong gói này rồi. Master đã STOP.
        // Slave quay lại trạng thái "Lắng nghe" để chờ lần gọi tên tiếp theo
        HAL_I2C_EnableListen_IT(&hi2c2);
    }
}

// 3. Khi có lỗi (Nhiễu, Bus Error) -> Reset về Listen
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) {
    if (hi2c->Instance == I2C2) {
        // Reset lại I2C nếu cần (Optional: __HAL_I2C_RESET_HANDLE_STATE(hi2c));
        HAL_I2C_EnableListen_IT(&hi2c2);
    }
}

// 4. Bắt buộc: Xử lý sự kiện Listen Complete (khi Master NACK hoặc Stop đột ngột)
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c) {
    if (hi2c->Instance == I2C2) {
        // Đảm bảo luôn quay về trạng thái nghe
        HAL_I2C_EnableListen_IT(&hi2c2);
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
