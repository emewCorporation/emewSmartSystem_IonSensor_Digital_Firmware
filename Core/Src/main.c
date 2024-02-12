/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "./SystemRoutines/CANSetup.h"
#include "./SystemRoutines/SystemInitialization.h"
#include "./SystemRoutines/InformationPrintRoutines.h"

#include "./PrimaryStateMachine.h"
#include "./StructDefinitions.h"
#include "./Global_HALL.h"

#include "./GenericLibraries/DataSerializationLibrary.h"

#include <Peripheral_Libraries/EEPROM_Library.h>
#include "./Peripheral_Libraries/FlashMemory_Library.h"
#include <Peripheral_Libraries/MCP4726A1T_E.h>
#include <Peripheral_Libraries/MCP3421.h>
#include "./Peripheral_Libraries/CANBusProtocol_Library.h"
//#include <Peripheral_Libraries/STM32_ADC.h>

#include "./Support/TestingRoutine.h"
#include "./Support/ProgramEEPROM.h"

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
ADC_HandleTypeDef hadc1;

CAN_HandleTypeDef hcan;

I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim16;
TIM_HandleTypeDef htim17;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
sysinfo SysInfo;

char FirmwareVersion[] = "A.1.1";
char DeviceMode[] = "Ion Sensor Digital";
char PrintBuffer[COMM_SIZE];
uint8_t VerboseMode = true;

uint32_t TxMailbox;						//CANBus Mailbox
CAN_TxHeaderTypeDef pTxHeader;			//CANBus transmit properties struct
CAN_RxHeaderTypeDef pRxHeader;			//CANBus receive properties struct
uint8_t CAN_TransmitMessage[8]={0};		//Data array to send data over CANBus
uint8_t CAN_ReceivedMessage[8]={0};		//Data array to receive data over CANBus

HAL_StatusTypeDef ret;
uint8_t Status;

_Bool MasterActivation = true;
_Bool HeartBeatFlag = false;
uint8_t PrimaryState = 0x00;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM16_Init(void);
static void MX_TIM17_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
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
  MX_CAN_Init();
  MX_I2C1_Init();
  MX_SPI2_Init();
  MX_TIM6_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */

  //Prevents user code execution on firmware update
  HAL_Delay(1000);

  //Print the opening splash message
  StartupPrint_Routine();

  memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rFirmware Version: %s", FirmwareVersion );
  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
  memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\rDevice Mode: %s", DeviceMode );
  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);

  //Support functions (LEAVE DISABLED UNLESS NEEDED)
  if(0){ TestingRoutine();    		 }
  if(0){ ProgramEEPROM_All(); 		 }
  if(0){ ProgramEEPROM_CAN(); 		 }
  if(0){ ProgramEEPROM_SensorType(); }

  //Perform Ion Sensor activation routine
  if( SystemActivation() == true ){
	  memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\rAll subsystems successfully activated"  );
	  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
  }else{
	  MasterActivation = false;
	  memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\rCRITICAL ERROR - Could not activate all subsystems"  );
	  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
  }

  //Print the sensor information stored in the EEPROM
  SystemSettingPrint_Routine();

  //Init the CANBus with retrieved system information
  CANSetup(SysInfo.CAN_Address);
  uint32_t ReadRegister=0;
  ReadRegister=CANBUS_EXT.Instance->MCR;
  memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rCAN MCR Register: 0x%2lX", ReadRegister  );
  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  //Print message saying that the master while loop has been successfully entered
  if(MasterActivation==true){
	  memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rMaster while loop entered" );
	  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
  }


  //uint8_t Status;
  //uint8_t ReadBuffer[6] = {0x00};
  uint8_t ctrlbool=0;

  while(MasterActivation==true){

	  //Check heart beat LED
	  if( HeartBeatFlag == true ){
		  HeartBeatFlag = false;
		  Toggle_LED_HeartBeat();
	  }

	  //Check CAN FIFO0 for an instruction on the bus
  	  if(HAL_CAN_GetRxFifoFillLevel(&CANBUS_EXT, CAN_RX_FIFO0) != 0){		//If FIFO0 has a message, retrieve it
  		  CAN_ClearFIFO(CAN_RX_FIFO1);
  		  memset(CAN_ReceivedMessage, 0, 8);
  		  ret = HAL_CAN_GetRxMessage(&CANBUS_EXT, CAN_RX_FIFO0, &pRxHeader, CAN_ReceivedMessage);
  		  if( ret == HAL_OK ) {
  			  PrimaryState = CAN_ReceivedMessage[0];
  			  memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rCAN message received on FIFO0 from 0x%03lX", pRxHeader.StdId);
  			  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
  			  memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- PrimaryState = %u", PrimaryState);
  			  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
  		  }else{
  	  		  memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\rCAN message could not be received on FIFO0"  );
  	  		  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
  	  		  PrimaryState=0x00;
  		  }
  	  }

	  //Enter primary state machine
	  PrimaryStateMachine();




	  if(SysInfo.Button1==true){
		  SysInfo.Button1=false;
		  memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\rButton 1 Pressed");
		  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);

		  memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\rTS Pin -> Status %u", Read_USBC_SpareTS() );
		  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);

		  //HAL_ADCEx_Calibration_Start();
		  float VoltageValue1;
		  STM32_ADC_ReadVoltage(&VoltageValue1, CC1_CH);
		  memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\rCC%u Value -> %0.3f", CC1_CH, VoltageValue1 );
		  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );

		  /*
		  Status = MCP4726A1T_WriteAllMemory(VREF_BUFF,PD_NO,G_1X, 1600);
		  if(Status != 0x2F){
			  memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\rDAC Write All Fail -> Status 0x%02X", Status );
			  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
		  }else{
			  memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\rDAC Write All Success");
			  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
		  }
		  */

	  }


	  if(SysInfo.Button2==true){
		  SysInfo.Button2=false;
		  memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\rButton 2 Pressed");
		  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);

		  //HAL_ADCEx_Calibration_Start();
		  float VoltageValue2;
		  STM32_ADC_ReadVoltage(&VoltageValue2, CC2_CH);
		  memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\rCC%u Value -> %0.3f", CC2_CH, VoltageValue2 );
		  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );


		  /*
		  //Print routine enter message
		  memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rVoltage Measurement Routine Entered"  );
		  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);

		  //Measure voltage 5 times and take the average
		  float VoltageAverage = 0;
		  float VoltageSample;
		  for(int i=0; i<5; i++){
			  MCP3421_ADC_ReadVoltage( &VoltageSample );
			  VoltageAverage = VoltageAverage + VoltageSample;
		  }
		  VoltageAverage = VoltageAverage/5;

		  //Report voltage average to console
		  memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rVoltage Average Measurement %0.4f V", VoltageAverage*2 );
		  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
		   */
		  /*
		  Status = MCP4726A1T_ReadAllMemory(ReadBuffer);
		  if(Status != 0x2F){
			  memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\rDAC Write All Fail -> Status 0x%02X", Status );
			  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
		  }else{
			  memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\rDAC Write All Success");
			  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
		  }

		  memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\rRegister Read:");
		  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
		  for(int i=0; i<6; i++){
			  memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\tVal[%d]: 0x%02X", i, ReadBuffer[i] );
			  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
		  }*/




	  }


	  if(SysInfo.Button3==true){
		  SysInfo.Button3=false;
		  memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\rButton 3 Pressed");
		  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);

		  Toggle_LED_HeartBeat();
		  if(ctrlbool==0){
			 Set_LED_Metal(1);
			 Set_LED_Turbidity(0);
			 ctrlbool = 1;
		 }else{
			 Set_LED_Metal(0);
			 Set_LED_Turbidity(1);
			 ctrlbool = 0;
		 }
	  }


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }

  //Print message alerting user master while loops has been left
  memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rWARNING: Ion sensor shutting down" );
  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);


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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN;
  hcan.Init.Prescaler = 4;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_13TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = ENABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */

  /* USER CODE END CAN_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 16000-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 500-1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM16 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM16_Init(void)
{

  /* USER CODE BEGIN TIM16_Init 0 */

  /* USER CODE END TIM16_Init 0 */

  /* USER CODE BEGIN TIM16_Init 1 */

  /* USER CODE END TIM16_Init 1 */
  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 16-1;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 65535;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM16_Init 2 */

  /* USER CODE END TIM16_Init 2 */

}

/**
  * @brief TIM17 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM17_Init(void)
{

  /* USER CODE BEGIN TIM17_Init 0 */

  /* USER CODE END TIM17_Init 0 */

  /* USER CODE BEGIN TIM17_Init 1 */

  /* USER CODE END TIM17_Init 1 */
  htim17.Instance = TIM17;
  htim17.Init.Prescaler = 16000-1;
  htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim17.Init.Period = 65535;
  htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim17.Init.RepetitionCounter = 0;
  htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim17) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM17_Init 2 */

  /* USER CODE END TIM17_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, HRTBEAT_LED_Pin|SENSOR_LED_1_Pin|SENSOR_LED_2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, INDICATION_LED_1_Pin|INDICATION_LED_2_Pin|INDICATION_LED_3_Pin|INDICATION_LED_4_Pin
                          |INDICATION_LED_5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(FLASH_EN_GPIO_Port, FLASH_EN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(FLASH_WP_GPIO_Port, FLASH_WP_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : HRTBEAT_LED_Pin */
  GPIO_InitStruct.Pin = HRTBEAT_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(HRTBEAT_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SENSOR_LED_1_Pin SENSOR_LED_2_Pin */
  GPIO_InitStruct.Pin = SENSOR_LED_1_Pin|SENSOR_LED_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : BUTTON1_EXTI5_Pin BUTTON2_EXTI6_Pin BUTTON3_EXTI7_Pin */
  GPIO_InitStruct.Pin = BUTTON1_EXTI5_Pin|BUTTON2_EXTI6_Pin|BUTTON3_EXTI7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : INDICATION_LED_1_Pin INDICATION_LED_2_Pin INDICATION_LED_3_Pin INDICATION_LED_4_Pin
                           INDICATION_LED_5_Pin FLASH_EN_Pin */
  GPIO_InitStruct.Pin = INDICATION_LED_1_Pin|INDICATION_LED_2_Pin|INDICATION_LED_3_Pin|INDICATION_LED_4_Pin
                          |INDICATION_LED_5_Pin|FLASH_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : FLASH_WP_Pin */
  GPIO_InitStruct.Pin = FLASH_WP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(FLASH_WP_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : CC1_DETECT_EXTI11_Pin CC2_DETECT_EXTI12_Pin */
  GPIO_InitStruct.Pin = CC1_DETECT_EXTI11_Pin|CC2_DETECT_EXTI12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SPARE_TS_Pin */
  GPIO_InitStruct.Pin = SPARE_TS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SPARE_TS_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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

#ifdef  USE_FULL_ASSERT
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
