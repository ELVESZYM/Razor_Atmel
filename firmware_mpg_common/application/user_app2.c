/**********************************************************************************************************************
File: user_app2.c                                                                

Description:
This is a user_app2.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp2Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp2RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp2"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp2Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp2_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp2_StateMachine;            /* The state machine function pointer */
//static u32 UserApp2_u32Timeout;                      /* Timeout counter used across states */

extern u8 u8_dot[3][32];
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
Function: UserApp2Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp2Initialize(void)
{
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp2_StateMachine = UserApp2SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp2_StateMachine = UserApp2SM_FailedInit;
  }

} /* end UserApp2Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp2RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp2RunActiveState(void)
{
  UserApp2_StateMachine();

} /* end UserApp2RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
static void SendCharMSB(u8 u8Data_)
{
 
 //  CLR_LED_LAT();
  
  u8 u8flag;						//数据标志位
  u32 u8length;					//数据长度
  u8 u8test_temp = 0;
  u8test_temp = u8Data_;
  for(u8length = 0; u8length < 8; u8length++)	      //发送一个8位数据
  {

    CLR_LED_CLK(); //时钟线拉低
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
//   SET_LED_LAT();
  
}

//水平显示
static void UserApp2SM_Idle(void)
{
  static u8 u8Data;
  static u8 u8y=0;
  static u8 u8select=0;
  static u16 u16Counter=0;
  u16Counter++;
  
  CLR_LED_LAT();
  
  for(u8 u8i=0;u8i<15;u8i=u8i+2)
  {

    SendCharMSB(0x00);
    SendCharMSB(0x00);
    for(u8 u8j=0;u8j<3;u8j++)
    {  
      SendCharMSB(u8_dot[u8j][u8i+16]);      
      SendCharMSB(u8_dot[u8j][u8i]);
      SendCharMSB(u8_dot[u8j][u8i+17]);
      SendCharMSB(u8_dot[u8j][u8i+1]); 
    }
    
    
    switch(u8i/2)		//1--8行的输入情况选择
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
    default: break;
    }
   // CLR_LED_LAT();
    SET_LED_LAT();
    SET_LED_CS1();   
    for(u8 u8k=0;u8k<200;u8k++);
    for(u8 u8k=0;u8k<200;u8k++);
    CLR_LED_CS1();
    
  }



}


/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */

     
#if 0
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp2SM_Error(void)          
{
  
} /* end UserApp2SM_Error() */
#endif


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserApp2SM_FailedInit(void)          
{
    
} /* end UserApp2SM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
