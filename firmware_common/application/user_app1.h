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


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


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
static void UserApp1SM_Idle(void);    

static void UserApp1SM_Error(void);         


#endif /* __USER_APP1_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
