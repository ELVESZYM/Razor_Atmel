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
 static u8 u8UserInput[2] = {0xd5,0xd4}; //16点汉字 “啊”  的GB2312内码。 












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
  static u8 u8AarryData[112]={0x3C,0x40,0x01,0x01};
  static u16 u16Count=0;
   
  SET_LED_CS1();
        	
  if(u8select >= 8)		
  {
     u8select = 0;
  } 
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
  CLR_LED_LAT();  
 
  for(;u8j<112;u8k++)
  { 
 /*  if((u8k-1)%7 == 0 ||u8k == 0)
    {
     
    }
 */
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
    u8MSB=u8internal_code[0];
    u8LSB=u8internal_code[1];
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
  static u8 u8_dot[32];			  //存储汉字 “啊”  的点阵数据。 
  static u32 u32delay = 0;

  static u32 u32_dot_address;
  static u8 u8_yun[]={0x00,0x00,0x3F,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFE,0x02,0x00,
                       0x04,0x00,0x04,0x00,0x08,0x40,0x10,0x20,0x20,0x10,0x7F,0xF8,0x20,0x08,0x00,0x08};
  u8 u8_counter_;

 u32_dot_address = GB2312_Addr(u8UserInput,TYPE_16); //得到“啊” 点阵数据的起始地址 

  for( u8_counter_ = 0; u8_counter_ < 32; u8_counter_++ )   //得到“啊” 点阵数据 
  {
    CCS_PA16();
    u8_dot[u8_counter_] = readbyte(u32_dot_address+u8_counter_);
    SCS_PA16();
  }

}



/* Wait for ??? */
static void UserApp1SM_Idle(void)
{  

 UserApp1_StateMachine = font_character;
//UserApp1_StateMachine=LEDDispMoveHorizontal;
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
