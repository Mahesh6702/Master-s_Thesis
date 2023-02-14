
//#include "timer2.h"
TIM_HandleTypeDef htim2;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim);
void MX_TIM2_Init(void);

int flag;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(96000);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(flag == 1)
  {
  int value =HAL_RCC_GetSysClockFreq() / 1000000;
  Serial.print("Clock Frequency   "); Serial.print(value); Serial.println(" MHz");
  delay(100);
  }
  flag =0;

}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{

  //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
  flag = 1;
}

void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 1000;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 18000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
}
