/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */

 static u8 u8select = 0;














/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
  /*************************************************************/  
 //初始化PA0-PA6口，设为输出口
  AT91C_BASE_PIOA->PIO_PER    |= 0x7F; 
  AT91C_BASE_PIOA->PIO_PDR    &= 0xFFFFFF80;
  
  AT91C_BASE_PIOA->PIO_OER    |= 0x7F;
  AT91C_BASE_PIOA->PIO_ODR    &= 0xFFFFFF80;
  
  AT91C_BASE_PIOA->PIO_IFER   = PIOA_IFER_INIT;
  AT91C_BASE_PIOA->PIO_IFDR   = PIOA_IFDR_INIT;
  
  AT91C_BASE_PIOA->PIO_SODR   = PIOA_SODR_INIT;
  AT91C_BASE_PIOA->PIO_CODR   = PIOA_CODR_INIT;
  
  AT91C_BASE_PIOA->PIO_MDER   = PIOA_MDER_INIT;
  AT91C_BASE_PIOA->PIO_MDDR   = PIOA_MDDR_INIT;
  
  AT91C_BASE_PIOA->PIO_PPUDR  = PIOA_PPUDR_INIT;
  AT91C_BASE_PIOA->PIO_PPUER  = PIOA_PPUER_INIT;
  
  AT91C_BASE_PIOA->PIO_ABSR   = PIOA_ABSR_INIT;
  AT91C_BASE_PIOA->PIO_SCIFSR = PIOA_SCIFSR_INIT;
  
  AT91C_BASE_PIOA->PIO_DIFSR  = PIOA_DIFSR_INIT;
  AT91C_BASE_PIOA->PIO_SCDR   = PIOA_SCDR_INIT;
  
  AT91C_BASE_PIOA->PIO_OWER   |= 0x7F;
  AT91C_BASE_PIOA->PIO_OWDR   &= 0xFFFFFF80;
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
static void Delay(u8 u8Time)
 {
   static u8 u8TimeCount=0;
   u8TimeCount++;
   if(u8TimeCount == u8Time )
   {
     u8TimeCount=0;
     return;
   }
 }

static void SendCharMSB(u8 u8Data_)
{
  
  u8 u8flag;						//数据标志位
  u32 u8length;					//数据长度
  u8 u8test_temp = 0;
  u8test_temp = u8Data_;
  for(u8length = 0; u8length < 8; u8length++)	      //发送一个8位数据
  {
 

    
    //时钟线拉低
    CLR_LED_CLK();
    u8flag = u8test_temp & 0x80;	//判断数据最高位状态
    if(u8flag == 0)							
    {
      CLR_LED_DRI();									//当最高位为0时，将数据输出口拉低
    }
    else
    {
      SET_LED_DRI();									//当最高位为0时，将数据输出口拉低
    }

    SET_LED_CLK();				//时钟线拉高
    u8test_temp <<= 1;			//将数据左移1位
  
  }
 
  
}

//水平显示
static void LEDDispMoveHorizontal(void)
{
  static u8 u8Data;
  static u8 u8j=0;
  u8 u8k=0;
  static u8 u8Send_flag = 1;
  static u8 u8AarryData[112]={0x3C};
  static u16 u16Count=0;
   
  SET_LED_CS1();
        
         	
  if(u8select >= 8)		
  {
     u8select = 0;
  } 
  
  CLR_LED_LAT();  
 
  for(;u8k<112;u8k++)
  { 
    if((u8k-1)%7 == 0 ||u8k == 0)
    {
      switch(u8select)		//1--8行的输入情况选择
        {
          case 0:   { PA2_CODR();                 
                      PA1_CODR();
                      PA0_CODR();
                     }break;
          case 1:   { PA2_CODR();
                      PA1_CODR();  
                      PA0_SODR();
                     }break;
          case 2:   { PA2_CODR();
                      PA1_SODR();
                      PA0_CODR();
                     }break;
          case 3:	{ PA2_CODR();
                      PA1_SODR();
                      PA0_SODR();
                     }break;
          case 4:   { PA2_SODR();
                      PA1_CODR();
                      PA0_CODR();
                     }break;
          case 5:   { PA2_SODR();
                      PA1_CODR(); 
                      PA0_SODR();
                     }break;
          case 6:   { PA2_SODR();
                      PA1_SODR();
                      PA0_CODR();
                     }break;
          case 7:   { PA2_SODR();
                      PA1_SODR();
                      PA0_SODR(); 
                    }break;
         }
         u8select++;//行循环选择
    }
     u8Data=u8AarryData[u8j]; 
     SendCharMSB(u8Data);
     u8j++;
     if(u8j == 112)//发满一个屏需要112个字节，14个8位bit数据
     {
      u8j=0;
     }
  }
  SET_LED_LAT();
 
  }
 
 
  /*   
    SET_LED_LAT();
    Delay(5);
    CLR_LED_LAT();
  */
/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{

  UserApp1_StateMachine=LEDDispMoveHorizontal;
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
