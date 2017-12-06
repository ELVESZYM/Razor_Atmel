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

extern u32 G_u32AntApiCurrentDataTimeStamp;                  
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;   
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;  

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
static u32 UserApp_u32Timeout;
static u32 UserApp_u32DataMsgCount = 0;   /* ANT_DATA packets received */
static u32 UserApp_u32TickMsgCount = 0;   /* ANT_TICK packets received */
static bool bStateflag=TRUE    ;           /*true->master     flase->slave*/
static u8 u8MAorSL;
static bool bShow=TRUE;
static bool bLcdSlaveShow=FALSE;
static bool bLedShow=FALSE;
AntAssignChannelInfoType sAntSlave,sAntSetupData;

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
/* Wait for ANT channel assignment */
static void UserApp1SM_WaitChannelAssign()
{
 
  if( AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
  {
    /* Channel assignment is successful, so open channel and
    proceed to Idle state */
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  

  /* Watch for time out */
  if(IsTimeUp(&UserApp_u32Timeout, 3000))
  {
    UserApp1_StateMachine = UserApp1SM_Error;    
  }
    
} /* end UserApp1SM_AntChannelAssign */
/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/

//static void UserApp1SlaveInitialize(void)
void UserApp1Initialize(void)
{
  static u8 au8WelcomeMessage[] = " heart rate tester";
  static u8 au8NetWorkKey[]={0xB9, 0xA5, 0x21, 0xFB, 0xBD, 0x72, 0xC3, 0x45};

  /* Clear screen and place start messages */
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR, au8WelcomeMessage); 

  /* Configure ANT for this application */
  sAntSetupData.AntChannel          = ANT_CHANNEL_USERAPP;
  sAntSetupData.AntChannelType      = ANT_CHANNEL_TYPE_SLAVE;
  sAntSetupData.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  sAntSetupData.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;

  sAntSetupData.AntDeviceIdLo       = ANT_DerEVICEID_LO_USERAPP;
  sAntSetupData.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
  sAntSetupData.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  sAntSetupData.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  sAntSetupData.AntFrequency        = ANT_FREQUENCY_USERAPP;
  sAntSetupData.AntTxPower          = ANT_TX_POWER_USERAPP;

 // sAntSetupData.AntNetwork = ANT_NETWORK_DEFAULT;
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    sAntSetupData.AntNetworkKey[i] = au8NetWorkKey[i];
  }
    
  /* If good initialization, set state to Idle */
  if( AntAssignChannel(&sAntSetupData) )
  {
    /* Channel is configured, so change LED to yellow */
    UserApp1_StateMachine = UserApp1SM_WaitChannelAssign;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    LedBlink(RED, LED_4HZ);
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

static void UserAppSM_WaitChannelClose(void)
{
  static u8 u8Count=0;
   u8Count++;
  /* Monitor the channel status to check if channel is closed */
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CLOSED )
  {
     LCDMessage(LINE2_START_ADDR, "tester over...         ");
       
  if(WasButtonPressed(BUTTON1))
  {
     ButtonAcknowledge(BUTTON1);
     UserApp1_StateMachine = UserApp1Initialize  ;
  }
  }
  
 
} /* end UserAppSM_WaitChannelClose() */
static u8 HEX_OCT(u8 *u8H_O)
{
  static u8  u8num=0;
  for(u8 i=0;u8H_O[i]!='\0';u8H_O++)
   {
     if(u8H_O[i]>='0' && u8H_O[i]<='9')
     {
      u8num=u8num*16+u8H_O[i]-'0';
     } 
     else if(u8H_O[i]>='a' && u8H_O[i]<='f')
     {
      u8num=u8num*16+u8H_O[i]-'a'+10;
     }
     else if(u8H_O[i]>='A' && u8H_O[i]<='F')
     {
       u8num=u8num*16+u8H_O[i]-'A'+10;
     }
  }   
 return u8num;
}
static void UserAppSM_Saving(void)
{
  static u8 u8Count=0;
   u8Count++;
   LCDMessage(LINE2_START_ADDR, "tester over...         ");
   if(u8Count == 1000)
   {
     u8Count=0;
     LCDCommand(LCD_DISPLAY_CMD);
   }

  if(WasButtonPressed(BUTTON1))
  {
     ButtonAcknowledge(BUTTON1);
     LCDCommand(LCD_DISPLAY_CMD | LCD_DISPLAY_ON);
     UserApp1_StateMachine = UserAppSM_ChannelOpen  ;
  }
}
static void UserAppSM_ChannelOpen(void)
{
  static u8 au8TestMessage[] = {0, 0, 0, 0, 0xA5, 0, 0, 0};
  static u8 au8DataContent[3];
  static u8 u8Hexshow=0;
  static u8 au8Two_Number[7]="      ";
  static u8 au8Three_Number[7]="      ";
  static u32 au32Real_HeartRatr[10];
  static u8 u8HeartRate=0;
  static bool bNumber_Ten=TRUE;
  static u16 u16average_Number=0;
  static u8 au8average_Numbe[3];
  static u8 u8c;
  if( AntReadAppMessageBuffer() )
  {
     /* New message from ANT task: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      /* We got some data: parse it into au8DataContent[] */
      for(u8 i = 0; i < ANT_DATA_BYTES; i++)
      {
        if(i == 7)
        {
          au8DataContent[0] = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[i] / 16);
          au8DataContent[1] = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[i] % 16);   

          u8Hexshow = HEX_OCT(au8DataContent);  
        }
      }
      if(u8Hexshow>=0 &&u8Hexshow<100)
      {
        au8Two_Number[0]=(u8Hexshow/10)+0x30;
        au8Two_Number[1]=(u8Hexshow%10)+0x30;
        au8Two_Number[2]=' ';
        LCDMessage(LINE2_START_ADDR, au8Two_Number);
      }
      else if(u8Hexshow>=100 && u8Hexshow<200)
      {
        au8Three_Number[0]=(u8Hexshow/100)+0x30;
        au8Three_Number[1]=(u8Hexshow%100/10)+0x30;
        au8Three_Number[2]=(u8Hexshow%10)+0x30;
        LCDMessage(LINE2_START_ADDR, au8Three_Number);
      }
      else
      {
         UserApp1_StateMachine = UserApp1SM_Error;
      }
      
      
      
      
      if(u8Hexshow>=45 && u8Hexshow<100)
      {
        LCDMessage(LINE2_START_ADDR+5, " Normal      ");  
        LedOn(GREEN);
        LedOff(YELLOW);
        LedOff(RED);
      }
      else if(u8Hexshow>=100 && u8Hexshow<=140)
      {
        LCDMessage(LINE2_START_ADDR+5, "excited       ");  
        LedOn(YELLOW);
        LedOff(GREEN);
        LedOff(RED);
      }
      else if(u8Hexshow<=160 &&u8Hexshow >140 )
      {
         LCDMessage(LINE2_START_ADDR+5, " quickly          ");  
         LedOn(RED);
         LedOff(GREEN);
         LedOff(YELLOW);
      }
      else
      {
        LCDMessage(LINE2_START_ADDR+5, "Superman");  
        LedOn(RED);
        
      }
      
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
  
 
    
  if(WasButtonPressed(BUTTON3))
  {
     ButtonAcknowledge(BUTTON3);
     static u8 u8Count=0;
     u8Count++;
#if 0
     AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
    /* Set timer and advance states */
    UserApp_u32Timeout = G_u32SystemTime1ms;
    LedOff(RED);
    LedOff(GREEN);
    LedOff(YELLOW);
    UserApp1_StateMachine = UserAppSM_WaitChannelClose  ;
  
#endif 
    UserApp1_StateMachine = UserAppSM_Saving  ;
  
  } /* end if(WasButtonPressed(BUTTON3)) */
  
  if(bNumber_Ten)
  {
   au32Real_HeartRatr[u8HeartRate]=u8Hexshow;
   u8HeartRate++;
  }

  if(u8HeartRate == 10)
  {
   bNumber_Ten=FALSE;
   for(u8 u8j=0;u8j<9;u8j++)
   {
     au32Real_HeartRatr[u8j]=au32Real_HeartRatr[u8j+1];
     u16average_Number+=au32Real_HeartRatr[u8j]; 
   }
    au32Real_HeartRatr[9]=u8Hexshow;
    u16average_Number=(u16average_Number+au32Real_HeartRatr[9])/10;
  }
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    if(u16average_Number>=100 && u16average_Number<=1000)
    {
      au8average_Numbe[0]=(u16average_Number/100)+0x30;
      au8average_Numbe[1]=(u16average_Number%100/10)+0x30;
      au8average_Numbe[2]=(u16average_Number%10)+0x30;
    }
    else if(u16average_Number>0 && u16average_Number<100)
    {
      au8average_Numbe[0]=(u16average_Number/10)+0x30;
      au8average_Numbe[1]=(u16average_Number%10)+0x30;
      au8average_Numbe[2]=' ';
    }
   
    LCDMessage(LINE2_START_ADDR+14,au8average_Numbe);  
    u8c++;
    if(u8c++ ==1000)
    {
      u8c=0;
    }
  }
}

static void UserAppSM_WaitChannelOpen(void)
{
  /* Monitor the channel status to check if channel is opened */
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_OPEN )
  {
    
    UserApp1_StateMachine = UserAppSM_ChannelOpen;
  }

  /* Check for timeout */
  if( IsTimeUp(&UserApp_u32Timeout, TIMEOUT_VALUE) )
  {
    AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
    LedOff(GREEN);
    LedOn(YELLOW);
    UserApp1_StateMachine = UserApp1SM_Idle;
  }

} /* end UserAppSM_WaitChannelOpen() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
    /* Set timer and advance states */
    UserApp_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserAppSM_WaitChannelOpen;
 
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/