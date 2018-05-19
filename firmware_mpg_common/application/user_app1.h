/**********************************************************************************************************************
File: user_app1.h                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
1. Follow the instructions at the top of user_app1.c
2. Use ctrl-h to find and replace all instances of "user_app1" with "yournewtaskname"
3. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
4. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
5. Add #include yournewtaskname.h" to configuration.h
6. Add/update any special configurations required in configuration.h (e.g. peripheral assignment and setup values)
7. Delete this text (between the dashed lines)
----------------------------------------------------------------------------------------------------------------------

Description:
Header file for user_app1.c

**********************************************************************************************************************/

#ifndef __USER_APP1_H
#define __USER_APP1_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
/* Required constants for ANT channel configuration */
#define ANT_CHANNEL_TYPE_USERAPP        CHANNEL_TYPE_MASTER   // Device is a MASTER
#define ANT_CHANNEL_USERAPP             ANT_CHANNEL_0         // Channel 0 - 7
#define ANT_DEVICEID_LO_USERAPP         (u8)0x34              // Low byte of two-byte Device #
#define ANT_DEVICEID_HI_USERAPP         (u8)0x12              // High byte of two-byte Device #
#define ANT_DEVICE_TYPE_USERAPP         (u8)1                 // 1 - 255
#define ANT_TRANSMISSION_TYPE_USERAPP   (u8)1                 // 1-127 (MSB is pairing bit)
#define ANT_CHANNEL_PERIOD_LO_USERAPP   (u8)0x00              // Low byte of two-byte channel period 0x0001 - 0x7fff
#define ANT_CHANNEL_PERIOD_HI_USERAPP   (u8)0x20              // High byte of two-byte channel period 
#define ANT_FREQUENCY_USERAPP           (u8)50                // 2400MHz + this number 0 - 99
#define ANT_TX_POWER_USERAPP            RADIO_TX_POWER_4DBM   // RADIO_TX_POWER_0DBM, RADIO_TX_POWER_MINUS5DBM, RADIO_TX_POWER_MINUS10DBM, RADIO_TX_POWER_MINUS20DBM


/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/



#define G_u8LedDisplayScreenCharUnits  (u8)14 

#define P_29_A4_SCS1 0x00000008
#define P_29_A4_CCS1 0x00000008
#define P_30_A6_SDAO 0x00000010
#define P_30_A6_CDAO 0x00000010
#define P_31_A7_SLAT 0x00000020
#define P_31_A7_CLAT 0x00000020
#define P_32_A8_SCLK 0x00000040
#define P_32_A8_CCLK 0x00000040

#define SET_LED_CS1()   ( AT91C_BASE_PIOA->PIO_SODR |= 0x08)//CS1
#define CLR_LED_CS1()   ( AT91C_BASE_PIOA->PIO_CODR |= 0x08)
#define SET_LED_DRI()   ( AT91C_BASE_PIOA->PIO_SODR |= 0x10)//DAO
#define CLR_LED_DRI()   ( AT91C_BASE_PIOA->PIO_CODR |= 0x10)
#define SET_LED_LAT()   ( AT91C_BASE_PIOA->PIO_SODR |= 0x20)//LAT
#define CLR_LED_LAT()   ( AT91C_BASE_PIOA->PIO_CODR |= 0x20)
#define SET_LED_CLK()   ( AT91C_BASE_PIOA->PIO_SODR |= 0x40)//CLK
#define CLR_LED_CLK()   ( AT91C_BASE_PIOA->PIO_CODR |= 0x40)


#define PA0_SODR()      ( AT91C_BASE_PIOA->PIO_SODR |= 0x01)
#define PA0_CODR()      ( AT91C_BASE_PIOA->PIO_CODR |= 0x01)
#define PA1_SODR()      ( AT91C_BASE_PIOA->PIO_SODR |= 0x02)
#define PA1_CODR()      ( AT91C_BASE_PIOA->PIO_CODR |= 0x02)
#define PA2_SODR()      ( AT91C_BASE_PIOA->PIO_SODR |= 0x04)
#define PA2_CODR()      ( AT91C_BASE_PIOA->PIO_CODR |= 0x04)

#define SCAN_LINE_MODE                		(u8)8 

#define P1_SIGN_DATA_LINES_OFFSET  0

#define SMISO_PA13() (AT91C_BASE_PIOA->PIO_SODR |= 0x2000)
#define SMOSI_PA14() (AT91C_BASE_PIOA->PIO_SODR |= 0x4000)
#define SSCK_PA15()  (AT91C_BASE_PIOA->PIO_SODR |= 0x8000)
#define SCS_PA16()   (AT91C_BASE_PIOA->PIO_SODR |= 0x10000)

#define CMISO_PA13() (AT91C_BASE_PIOA->PIO_CODR |= 0x2000)
#define CMOSI_PA14() (AT91C_BASE_PIOA->PIO_CODR |= 0x4000)
#define CSCK_PA15()  (AT91C_BASE_PIOA->PIO_CODR |= 0x8000)
#define CCS_PA16()   (AT91C_BASE_PIOA->PIO_CODR |= 0x10000)

 
//#define IO_STATE     (AT91C_BASE_PIOA->PIO_PDSR &=0xffffffff)

////////////////////////////////////

#define IO_STATE     (AT91C_BASE_PIOA->PIO_PDSR)

////////////////////////////////////

/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
#define ASC0808D2HZ_ADDR ( 0x66c0  ) 		//7*8 ascii code
#define ASC0812M2ZF_ADDR ( 0x66d40 )   		//6*12 ascii code
#define GBEX0816ZF_ADDR   243648   	   		//8*16 ascii code
#define ZF1112B2ZF_ADDR  ( 0x3cf80 )	   	//12*12 12µã×Ö·û 
#define HZ1112B2HZ_ADDR  ( 0x3cf80+376*24 )	//12*12 12µãºº×Ö 
#define CUTS1516ZF_ADDR  0x00  				//16*16 16µã×Ö·û 
#define JFLS1516HZ_ADDR  27072  			//16*16 16µãºº×Ö 

//---------------------------------------------
// ±£Áô 
//--------------------------------------------
#define ASCII0507ZF_ADDR        245696
#define ARIAL_16B0_ADDR         246464
#define ARIAL_12B0_ADDR         422720
#define SPAC1616_ADDR           425264
#define GB2311ToUnicode_addr    (12032)
#define UnicodeToGB2311_addr    (425328)
//------------------------------------------------
// ÒªÏÔÊ¾µÄºº×ÖµÄÀàÐÍ 
//------------------------------------------------
#define   TYPE_8  	0 //8  µã×Ö·û 
#define   TYPE_12  	1 //12 µãºº×Ö×Ö·û 
#define   TYPE_16  	2 //16 µãºº×Ö×Ö·û 

/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

#define G_u8LedDisplayScreenCharUnits  (u8)14 

#define P_29_A4_SCS1 0x00000008
#define P_29_A4_CCS1 0x00000008
#define P_30_A6_SDAO 0x00000010
#define P_30_A6_CDAO 0x00000010
#define P_31_A7_SLAT 0x00000020
#define P_31_A7_CLAT 0x00000020
#define P_32_A8_SCLK 0x00000040
#define P_32_A8_CCLK 0x00000040

#define SET_LED_CS1()   ( AT91C_BASE_PIOA->PIO_SODR |= 0x08)//CS1
#define CLR_LED_CS1()   ( AT91C_BASE_PIOA->PIO_CODR |= 0x08)
#define SET_LED_DRI()   ( AT91C_BASE_PIOA->PIO_SODR |= 0x10)//DAO
#define CLR_LED_DRI()   ( AT91C_BASE_PIOA->PIO_CODR |= 0x10)
#define SET_LED_LAT()   ( AT91C_BASE_PIOA->PIO_SODR |= 0x20)//LAT
#define CLR_LED_LAT()   ( AT91C_BASE_PIOA->PIO_CODR |= 0x20)
#define SET_LED_CLK()   ( AT91C_BASE_PIOA->PIO_SODR |= 0x40)//CLK
#define CLR_LED_CLK()   ( AT91C_BASE_PIOA->PIO_CODR |= 0x40)


#define PA0_SODR()      ( AT91C_BASE_PIOA->PIO_SODR |= 0x01)
#define PA0_CODR()      ( AT91C_BASE_PIOA->PIO_CODR |= 0x01)
#define PA1_SODR()      ( AT91C_BASE_PIOA->PIO_SODR |= 0x02)
#define PA1_CODR()      ( AT91C_BASE_PIOA->PIO_CODR |= 0x02)
#define PA2_SODR()      ( AT91C_BASE_PIOA->PIO_SODR |= 0x04)
#define PA2_CODR()      ( AT91C_BASE_PIOA->PIO_CODR |= 0x04)

#define SCAN_LINE_MODE                		(u8)8 

#define P1_SIGN_DATA_LINES_OFFSET  0







/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void UserApp1Initialize(void);
void UserApp1RunActiveState(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void UserApp1SM_AntChannelAssign(void);
static void UserApp1SM_Idle(void);    

static void UserApp1SM_Error(void);         


#endif /* __USER_APP1_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
