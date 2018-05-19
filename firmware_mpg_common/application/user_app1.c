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
extern u32 G_u32AntApiCurrentMessageTimeStamp;                    /* From ant_api.c */
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;    /* From ant_api.c */
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];  /* From ant_api.c */
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;                /* From ant_api.c */

extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;              /* The state machine function pointer */
static u32 UserApp1_u32Timeout;                        /* Timeout counter used across states */

static AntAssignChannelInfoType UserApp1_sChannelInfo; /* ANT setup parameters */

static u8 UserApp1_au8MessageFail[] = "\n\r***ANT channel setup failed***\n\n\r";

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/
  static u8 u8select = 0;
  static u8 u8UserInput[8]; //16点汉字 “啊”  的GB2312内码。 
  static u8 u8_dot[3][32];			  //存储汉字 “啊”  的点阵数据。 

  u8 u8_counter_,u8showcount=0;



/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

  /*
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
*/
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
static void LEDDispMoveHorizontal(void)
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
  
  if(u16Counter == 1000)
  {
    u16Counter=0;
    UserApp1_StateMachine=UserApp1SM_Idle;
    CLR_LED_LAT();

  }


}
 



/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/


static u8 readbyte(u32 u32addr)  //read one byte from GT ROM
 { 
  u8 u8i,u8RT_data=0x03;//赵：RT是Instruction-read方式 
  u8 u8Printf_screen=0;
  static u8 u8Instruction_flag;
  static u32 u32Adress_flag;
  static u32 u32IOLevel;
  static u32 u32Doorplate_addr;
  
  
  //CMOSI_PA14();                          //赵：??? 

  for(u8i=0;u8i<8;u8i++)	  //先写入0x03；//赵：发送1个字节的命令-read（03）-fast read（0B） 
  {
    u8Instruction_flag = u8RT_data & 0x80;
    
    for(u8 u8q=0;u8q<100;u8q++);
     
    CSCK_PA15();  
  
    if(u8Instruction_flag == 0)
    {
      CMOSI_PA14();
    }
    else
    {
      SMOSI_PA14();
    }
    for(u8 u8q=0;u8q<100;u8q++);
   
    SSCK_PA15();
    u8RT_data<<=1;    

  }
  
  u32Doorplate_addr=u32addr;
  
  for(u8i=0;u8i<24;u8i++)	   //再写三个字节地址 
  {
    
    u32Adress_flag = u32Doorplate_addr & 0x800000;
    
    for(u8 u8q=0;u8q<100;u8q++);
     
    CSCK_PA15();
     
    if(u32Adress_flag == 0)
    {
      CMOSI_PA14();
    }
    else
    {
      SMOSI_PA14();
    }
    for(u8 u8q=0;u8q<100;u8q++);
    
    SSCK_PA15();
    
    u32Doorplate_addr<<=1;
    
    
  }
 
   u8Printf_screen=0;
  
  for(u8i=0;u8i<8;u8i++)//读一个字节 
  {
    u32IOLevel = IO_STATE;
    
    SSCK_PA15();
      
    if((u32IOLevel & 0x2000) != 0)
    {
     u8Printf_screen|=0x01;
    }

    CSCK_PA15();
   
    u8Printf_screen<<=1;  

  }
   
  return (u8Printf_screen);

  

 }

//**************汉字在芯片中的点阵数据起始地址  ****************************
static u32  GB2312_Addr(u8 *u8internal_code,u8 u8type)
{
  static u32 u32temp = 0;
  static u8 u8MSB,u8LSB;
  
  if(u8internal_code[0]<0x80)	   	//ascii  半角字符 
  {
    if( u8internal_code[0] >= ' ' )  //判断是否大于“空格键 ” 0x20 
    {
       u32temp = u8internal_code[0] - ' ';
    }
    if( u8type == TYPE_8 )  	  
    {
       u32temp = u32temp*8  + ASC0808D2HZ_ADDR;//7*8 ascii code
    }  		 	
    else if( u8type == TYPE_12 )	
    {
      u32temp = u32temp*12 + ASC0812M2ZF_ADDR;	//6*12 ascii code
    }		
    else if( u8type == TYPE_16 )	
    {
      u32temp = u32temp*16 + GBEX0816ZF_ADDR;			//8*16 ascii code
    }
  }
  else               //chinese tab zone
  {
    switch (u8showcount)
    {
    case 0: u8MSB=u8internal_code[0];
             u8LSB=u8internal_code[1];
             break;
    case 1: u8MSB=u8internal_code[2];
             u8LSB=u8internal_code[3];
             break;
    case 2: u8MSB=u8internal_code[4];
             u8LSB=u8internal_code[5];
             break;
    }
   
    //符号区,全角 
    if(u8MSB>=0xA1 && u8MSB <= 0xAB && u8LSB>=0xa1)
    {
      if( u8type == TYPE_12 ) //16 点汉字字符 
      {
        if( u8MSB>=0xA1 && u8MSB <= 0xA3 && u8LSB>=0xa1 )//ZF 272 ZONE
        {
           u32temp = (u8MSB - 0xA1) * 94 + (u8LSB - 0xA1);
        }
        else if( u8MSB == 0xa9 && u8LSB>=0xa1  )//tab 94 zone
        {
           u32temp = 282 + ( u8LSB - 0xA1 );
        }
         return( u32temp *24 + ZF1112B2ZF_ADDR);		 //11*12 12点字符 
      }
      else
      {
        u32temp = (u8MSB - 0xA1) * 94 + (u8LSB - 0xA1);
        return(u32temp *32 + CUTS1516ZF_ADDR);			 //16*16 16点字符 
      }

    }
    //GBK双字节2区 6768
    else if(u8MSB>=0xb0 && u8MSB <= 0xf7 && u8LSB>=0xa1)
    {
      u32temp= (u8MSB - 0xB0) * 94 + (u8LSB - 0xA1);
      if( u8type == TYPE_12 )
      {
         u32temp = u32temp*24 + HZ1112B2HZ_ADDR;
      }
      else if( u8type == TYPE_16 )
      {
         u32temp = u32temp*32 + JFLS1516HZ_ADDR;
      }
     
    }
  }
 
return u32temp;

}
/*-------------------------------------------------------------------------------------------------------------------*/
static void font_character(void)
{
  static u32 u32_dot_address;

 u32_dot_address = GB2312_Addr(u8UserInput,TYPE_16); //得到“啊” 点阵数据的起始地址 

  for( u8_counter_ = 0; u8_counter_ < 32; u8_counter_++ )   //得到“啊” 点阵数据 
  {
    CCS_PA16();
    u8_dot[u8showcount][u8_counter_] = readbyte(u32_dot_address+u8_counter_);
    SCS_PA16();
  }
  u8showcount++;
  if(u8showcount == 3)
  {
    u8showcount=0;
    UserApp1_StateMachine=LEDDispMoveHorizontal;

  }
}

  
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
  u8 au8WelcomeMessage[] = "ANT Master";

 /****************************点阵*********************************/  
 //初始化PA0-PA6口，设为输出口
 // AT91C_BASE_PIOA->PIO_PER    |= 0x7F; //点阵口配置
 // AT91C_BASE_PIOA->PIO_PDR    &= 0xFFFFFF80;//点阵口配置
  
  AT91C_BASE_PIOA->PIO_PER    |= 0x1E07F;//点阵口配置和字库io口配置
  AT91C_BASE_PIOA->PIO_PDR    &= 0xFFFE1F80;//点阵口配置和字库io口配置
  
  AT91C_BASE_PIOA->PIO_OER    |= 0x1C07F;
  AT91C_BASE_PIOA->PIO_ODR    &= 0xFFFE3F80;
  
  AT91C_BASE_PIOA->PIO_IFER   |= 0x2000;
  AT91C_BASE_PIOA->PIO_IFDR   &= 0xFFFFDFFF;
  
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
  
  AT91C_BASE_PIOA->PIO_OWER   |= 0x1C07F;
  AT91C_BASE_PIOA->PIO_OWDR   &= 0xFFFE3F80;
    
  /*************************************************************/  
 
  
  
  /* Write a weclome message on the LCD */
#if EIE1
  /* Set a message up on the LCD. Delay is required to let the clear command send. */
  LCDCommand(LCD_CLEAR_CMD);
  for(u32 i = 0; i < 10000; i++);
  LCDMessage(LINE1_START_ADDR, au8WelcomeMessage);
#endif /* EIE1 */
  
#if 0 // untested for MPG2
  
#endif /* MPG2 */

 /* Configure ANT for this application */
  UserApp1_sChannelInfo.AntChannel          = ANT_CHANNEL_USERAPP;
  UserApp1_sChannelInfo.AntChannelType      = ANT_CHANNEL_TYPE_USERAPP;
  UserApp1_sChannelInfo.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  UserApp1_sChannelInfo.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;
 
  UserApp1_sChannelInfo.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
  UserApp1_sChannelInfo.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
  UserApp1_sChannelInfo.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  UserApp1_sChannelInfo.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  UserApp1_sChannelInfo.AntFrequency        = ANT_FREQUENCY_USERAPP;
  UserApp1_sChannelInfo.AntTxPower          = ANT_TX_POWER_USERAPP;

  UserApp1_sChannelInfo.AntNetwork = ANT_NETWORK_DEFAULT;
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    UserApp1_sChannelInfo.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
  }
  
  /* Attempt to queue the ANT channel setup */
  if( AntAssignChannel(&UserApp1_sChannelInfo) )
  {
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_AntChannelAssign;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    DebugPrintf(UserApp1_au8MessageFail);
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
/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ANT channel assignment */
static void UserApp1SM_AntChannelAssign()
{
  if( AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
  {
    /* Channel assignment is successful, so open channel and
    proceed to Idle state */
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  
  /* Watch for time out */
  if(IsTimeUp(&UserApp1_u32Timeout, 3000))
  {
    DebugPrintf(UserApp1_au8MessageFail);
    UserApp1_StateMachine = UserApp1SM_Error;    
  }
     
} /* end UserApp1SM_AntChannelAssign */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  static u8 au8TestMessage[] = {0, 0, 0, 0, 0xA5, 0, 0, 0};
  u8 au8DataContent[] = "xxxxxxxxxxxxxxxx";
  static u8 u8AntMessage[]={0xff,0x00,0x01,0x03,0xff,0x00,0x00,0x00};
  static u8 u8AntMessage_counter=0;
  
 
 
  
  if( AntReadAppMessageBuffer() )
  {
     /* New message from ANT task: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      /* We got some data: parse it into au8DataContent[] */
      for(u8 i = 0; i < ANT_DATA_BYTES; i++)
      {
        au8DataContent[2 * i]     = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[i] / 16);
        au8DataContent[2 * i + 1] = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[i] % 16);
        u8UserInput[i]=G_au8AntApiCurrentMessageBytes[i];
      }
      UserApp1_StateMachine = font_character;
      
#ifdef EIE1
     LCDMessage(LINE2_START_ADDR, au8DataContent);
     
      
#endif /* EIE1 */
      
#ifdef MPG2
#endif /* MPG2 */
      
    }
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
     /* Update and queue the new message data */
      au8TestMessage[7]++;
      if(au8TestMessage[7] == 0)
      {
        au8TestMessage[6]++;
        if(au8TestMessage[6] == 0)
        {
          au8TestMessage[5]++;
        }
      }
      AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP, au8TestMessage);
    }
  } /* end AntReadData() */
/*
   for(u8 u8i=0;u8i<8;u8i++)
  {
    if(G_au8AntApiCurrentMessageBytes[u8i] == u8AntMessage[u8i])
    {
      u8AntMessage_counter++;
      if(u8AntMessage_counter == 8)
      {
        u8AntMessage_counter=0;
        UserApp1_StateMachine = font_character;
      }

    }
  }
*/
} /* end UserApp1SM_Idle() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error (for now, do nothing) */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
