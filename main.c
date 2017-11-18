/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "gpio.h"
#include "LCD.h"
#include "glcdfont.h"
#include "shapes.h"
#include "tetris.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
volatile int l,c,cx,cy; 
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void tetris_setup();
void draw_tetrimono(int,int);
void erase_tetriono();
void rotate_tetrimono(int,int);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void HAL_SYSTICK_Callback(void);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	volatile int i,j,k;  // cx,cy, coordonnées du curseur
	int cmt=0; 								 // Compteur pour draw pixel d'une piece
	volatile int color[7]={0x001F,0xF800,0x07E0,0x07FF,0xF81F,0xFFE0,0xFFFF}; //Pour test
	

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
	LCD_Begin();

  /* USER CODE BEGIN 2 */
	//Parametres du lcd
	LCD_FillScreen(BLACK);
	LCD_SetTextSize(2);
	LCD_SetTextColor(WHITE,BLACK);
	LCD_SetCursor(1,20);
	cx=150;cy=150;
	
	//Espace de jeu
	tetris_setup();
	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

		draw_tetrimono(0,0);
		
		
		
  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
void HAL_SYSTICK_Callback(){
	static int j=0;
	j=j+1;
	if (j==1000)
		cy=cy+10;
}
	
void tetris_setup(){
	LCD_DrawRect(110,100,120,180,WHITE);
	LCD_DrawRect(10,50,80,50,BLUE);
	LCD_DrawRect(10,100,80,50,YELLOW);
	LCD_DrawRect(10,150,80,50,RED);
	LCD_DrawRect(10,200,80,80,GREEN);
	LCD_SetTextSize(5);
	LCD_SetCursor(30,10);
	LCD_Printf("TETRIS");
	LCD_SetTextSize(1);
	LCD_SetCursor(20,55);
	LCD_Printf("Score");
	LCD_SetCursor(20,105);
	LCD_Printf("Highscore");
	LCD_SetCursor(20,155);
	LCD_Printf("Ligne");
	LCD_SetCursor(20,205);
	LCD_Printf("P suivante");
}

void draw_tetrimono(int nb,int r){
	int i,j;
	for(i=0;i<SIZE;i++){
		for(j=0;j<SIZE;j++){
			if(PIECES[nb][r][i][j]==0){
				//cmt=cmt+1;
			}
			else {
				LCD_DrawRect(cx+i*10,cy+10*j,10,10,BLUE);

			}
		}
	}
		HAL_Delay(500);
		//cx=cx+i*10;          //A REGLER EN FONCTION DE COMMENT ON INCREMENTE cx etcxy
		//cy=cy+20;
}

void erase_tetriono(){
	int i,j;
	for(i=0;i<SIZE;i++){
		for(j=0;j<SIZE;j++){
				LCD_DrawRect(cx+i*10,cy+10*j,10,10,BLACK);

			}
		}
	}


void rotate_tetrimono(int nb,int r){
	erase_tetriono();
	draw_tetrimono(nb,r);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	HAL_Delay(7);
	if (HAL_GPIO_ReadPin(GPIOC,GPIO_Pin)==0){
		
		if(GPIO_Pin == 256)
			c = 0;
		else if(GPIO_Pin == 512)
			c = 1;
		else if(GPIO_Pin == 1024)
			c = 2;
		else
			c = 3;
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,1);
		HAL_Delay(7);
		if( HAL_GPIO_ReadPin(GPIOC,GPIO_Pin)==1)
			l= c;
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,0);
		
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,1);
		HAL_Delay(7);
		if( HAL_GPIO_ReadPin(GPIOC,GPIO_Pin)==1)
			l= c+4;
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,0);
		
	
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,1);
		HAL_Delay(7);
		if( HAL_GPIO_ReadPin(GPIOC,GPIO_Pin)==1)
			l= c+8;
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,0);
		
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,1);
		HAL_Delay(7);
		if( HAL_GPIO_ReadPin(GPIOC,GPIO_Pin)==1)
			l= c+12;
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,0);
		
		HAL_Delay(7);
		}
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9);
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_10);
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_11);
	
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
