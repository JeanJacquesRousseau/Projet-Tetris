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
#include "stdlib.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
volatile int l,c,cx,cy,inte,b,r,vitesse,timer,up,down,fin,m; // b block r rotations 
volatile int zone_jeu[TETRIS_V][TETRIS_H];
volatile int score, nb_lignes,level, best_s,nb_l_sup;
static int j=0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void tetris_setup();
void jeu_init();
void test_ajout();
void t_color();	
int move(int);
void draw_tetrimono(int,int);
void erase_tetriono(int,int);
//void rotate_tetrimono(int,int);
//void move_right(int,int);
//void move_left(int,int);
int ligne_haut();
int ligne_bas();
void nouvelle_piece();
void efface_ligne(int);
void dessine_ligne(int);
void points();

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void HAL_SYSTICK_Callback(void);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	volatile int a,c,i,j,k,m,fin,tour,arret,droite,gauche,s,test;  // cx,cy, coordonnées du curseur
	int cmt=0;// Compteur 
	
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
	cx=110;cy=100;
	
	//Espace de jeu
	tetris_setup();
	cx=110;cy=100;
	inte =0; timer =0;
	vitesse =1000;
	best_s=0;
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)                    //Pour jouer plusieurs parties
	{
			
	//initialisation des valeurs et de la zone
	jeu_init();
	//Jeu
  while (fin==0){		
		//Apparition de la piece
		nouvelle_piece();
		up = ligne_haut(); 
		down = ligne_bas();
		
		//Peut-on l'ajouter au tableau?
		test_ajout();
		
		if(fin==0){
		//Piece dans le tableau de jeu
		//Lui attribuer la bonne couleur pourl'affichage
			//t_color();		
			draw_tetrimono(d.X,d.Y);
	
		//Elle descend
			while(arret==0){
				a=d.X;c=d.Y;
				if(timer==1){
					for(i=d.Y+SIZE-down-up;i>d.Y;i--){
						for(j=d.X;j<SIZE+d.X;j++){
							if(zone_jeu[i][j]>=1){
								if(zone_jeu[i+1][j]>=1)
									arret=1;
								else
									arret=0;
							}
						}
					}
					if(arret==0){
						d.Y++;
						if(d.Y==17)
							d.Y=17;
					}
						
					timer=0;
					}
					
			//Droite gauche par le sonar
			//m= sonar;  //nom de la fonction 
					droite=0;gauche=0;
					if(m>d.X){
						for(k=d.X;k<=m;k++){
								for(i=d.Y;i<SIZE+d.Y;i++){
									if(zone_jeu[i][d.X+4]==0){
									}
									else{
										if(zone_jeu[i][d.X+4+1]==0){
										}
										else{
											droite=1;
										}
									}
								}
								if(droite==0)
									d.X++;
						}
					}
					else if(m<d.X){
						for(k=d.X;k>=m;k--){
							for(i=d.Y;i<SIZE+d.Y;i++){
								if(zone_jeu[i][d.X]==0){}
								else{
									if(zone_jeu[i][d.X-1]==0){}
									else
										gauche=1;
								}
							}
							if(gauche==0)
								d.X--;
						}
					}
					
					else{} //La piecce ne bouge pas
				
			//Rotation bas sonar par interruption
			//on met a jour a chaque fois (while arret)
			erase_tetriono(a,c);
			draw_tetrimono(d.X,d.Y);
			}
		//Test _ligne
		for(i=0;i<TETRIS_V;i++){
			test=1;
			for(j=0;j<TETRIS_H;j++)
				test=test*zone_jeu[i][j];
			if(test==0){}
			else{
				nb_l_sup++;
				nb_lignes++;
				efface_ligne(i);
				for(k=i-1;k>1;k++){
					for(j=0;j<TETRIS_H;j++){
						zone_jeu[k][j]=zone_jeu[k-1][j];
						dessine_ligne(k);
					}
				}
			}
		}
		nb_l_sup=0;
		//Fonction score+Nb_lignes+Affichage
		points();
		}
	}
	//Focntion Game_over David
	
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

	j=j+1;
	if (j==vitesse){
		timer=1;
//		cy=cy+10;
//		if(cy==Y_limit_max)
//			cy=Y_limit_max;
//		j=0;
	}
}
	
void tetris_setup(){
	LCD_DrawRect(109,99,122,182,WHITE);
	LCD_DrawRect(10,50,80,50,BLUE);
	LCD_DrawRect(10,100,80,50,YELLOW);
	LCD_DrawRect(10,150,80,50,RED);
	LCD_DrawRect(10,200,80,50,GREEN);
	LCD_DrawRect(10,250,80,50,WHITE);
	LCD_SetTextSize(5);
	LCD_SetCursor(30,10);
	LCD_Printf("TETRIS");
	LCD_SetTextSize(1);
	LCD_SetCursor(20,55);
	LCD_Printf("Score");
	LCD_SetCursor(30,70);
	LCD_Printf("0");
	LCD_SetCursor(20,105);
	LCD_Printf("Highscore");
	LCD_SetCursor(30,120);
	LCD_Printf("0");
	LCD_SetCursor(20,155);
	LCD_Printf("Ligne");
	LCD_SetCursor(30,170);
	LCD_Printf("0");
	LCD_SetCursor(20,205);
	LCD_Printf("P suivante");
	LCD_SetCursor(20,255);
	LCD_Printf("Level");
}

void jeu_init(){
	int i,j;
	score=0; nb_lignes =0; level=0;
	for(i=0;i<TETRIS_V;i++){
		for(j=0;j<TETRIS_H;j++){
			zone_jeu[i][j]=0;
			LCD_DrawRect(cx+j*10,cy+i*10,10,10,BLACK);
		}
	}
	fin =0;
}

void test_ajout(){
	int i,j;
	for(i=up;i<SIZE-down;i++){
			for(j=0;j<SIZE;j++){
				zone_jeu[i-up][j+4]=zone_jeu[i-up][j+4]+PIECES[b][r][i][j];
				if(zone_jeu[i-up][j+4]>2)
					fin=1;
				
			}
		}
}

void t_color(){
	int i,j;
	for(i=0;i<SIZE-down-up;i++){
				for(j=4;j<SIZE+4;j++){
					if(zone_jeu[i][j]==1)
						zone_jeu[i][j]=zone_jeu[i][j]*b+1;
				}
			}
}
int ligne_haut(){
	int i,j,cmt,up;
	up=0;
	cmt=0;
	for(i=0;i<SIZE;i++){
		for(j=0;j<SIZE;j++){
			if(PIECES[b][r][i][j]==0){
				cmt++;
				if(cmt==4){
					cmt=0;
					up++;
				}
			}
			else
				return(up);
		}
	}
	return(up);
}
int ligne_bas(){
	int i,j,cmt,down;
	down = 0;
	cmt=0;
	for(i=3;i>0;i--){
		for(j=0;j<SIZE;j++){
			if(PIECES[b][r][i][j]==0){
				cmt++;
				if(cmt==4){
					cmt=0;
					down++;
				}
			}
			else
				return(down);
		}
	}
	return(down);
}

void draw_tetrimono(int x,int y){
	int i,j,color;
	for(i=0;i<SIZE-up-down;i++){
		for(j=0;j<SIZE;j++){
			if(zone_jeu[i][j]==0){
				}
			else {
				if(zone_jeu[i][j]==1)
					color=YELLOW;
				else if(zone_jeu[i][j]==2)
					color=CYAN;
				else if(zone_jeu[i][j]==3)
					color=GREEN;
				else if(zone_jeu[i][j]==4)
					color=RED;
				else if(zone_jeu[i][j]==5)
					color=ORANGE;
				else if(zone_jeu[i][j]==6)
					color=BLUE;
				else
					color=PURPLE;
				LCD_DrawRect(cx+y+i*10,cy+x+10*j,10,10,color);

			}
		}
	}
		HAL_Delay(10);
}


void erase_tetriono(int x, int y){
	int i,j;
	for(i=0;i<SIZE-up-down;i++){
		for(j=0;j<SIZE;j++)
				LCD_DrawRect(cx+y+i*10,cy+x+10*j,10,10,BLACK);
		}
	}


/*
void rotate_tetrimono(int nb,int r){
	erase_tetriono();
	draw_tetrimono(nb,r);
}
*/
/*
void move_right(int nb, int r){
	erase_tetriono();
	cx=cx+10;
	if(cx>=X_limit_max)
		cx = X_limit_max;
	draw_tetrimono(nb,r);
}
*/

/*
void move_left(int nb, int r){
	erase_tetriono();
	cx=cx-10;
	if(cx<=X_limit_min)
		cx = X_limit_min;
	draw_tetrimono(nb,r);
}
*/

void nouvelle_piece(){
		b=rand()%7;
		r=0;
	
		d.Type=b;
		if(b==0)
			d.color=YELLOW;
		else if(b==1)
			d.color=CYAN;
		else if(b==2)
			d.color=GREEN;
		else if(b==3)
			d.color=RED;
		else if(b==4)
			d.color=ORANGE;
		else if(b==5)
			d.color=BLUE;
		else
			d.color=PURPLE;
		
		d.rotation=r;
		d.X=4;
		d.Y=0;	
}

void efface_ligne(int i){
	int j,color;
	for(j=0;j<TETRIS_H;j++)
		LCD_DrawRect(10*j,i,10,10,BLACK);
	
}
void dessine_ligne(int i){
	int j,color;
	for(j=0;j<TETRIS_H;j++){
		if(zone_jeu[i][j]==1)
					color=YELLOW;
				else if(zone_jeu[i][j]==2)
					color=CYAN;
				else if(zone_jeu[i][j]==3)
					color=GREEN;
				else if(zone_jeu[i][j]==4)
					color=RED;
				else if(zone_jeu[i][j]==5)
					color=ORANGE;
				else if(zone_jeu[i][j]==6)
					color=BLUE;
				else if(zone_jeu[i][j]==0)
					color=BLACK;
				else
					color=PURPLE;
		LCD_DrawRect(cx+10*j,cy+i,10,10,color);
		
	}
	cx=110;cy=100;
	LCD_DrawRect(cx,cy,120,10,BLACK);
}

void points(){
	if(nb_lignes==0)
		level=0;
	else if(nb_lignes %10==0){
		level++;
	}
	else{}
	
	if (nb_l_sup==1)
		score = score+ 40*(level+1);
	else if (nb_l_sup==2)
		score = score+ 100*(level+1);
	else if (nb_l_sup==3)
		score = score+ 300*(level+1);
	else
		score = score+ 1200*(level+1);
	if(score>=best_s)
		best_s=score;
	cx=30;cy=270;
	//LCD_Printf("%d",level);
	cx=30;cy=70;
	LCD_Printf("%d",score);
	cx=30;cy=120;
	LCD_Printf("%d",best_s);
	cx=110;cy=100;
	
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	HAL_Delay(7);
	if (HAL_GPIO_ReadPin(GPIOC,GPIO_Pin)==0){
		inte=1;
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
