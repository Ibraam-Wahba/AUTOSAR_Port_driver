/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_PBcfg.c
 *
 * Description: Post Build Configuration Source file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Ibraam Wahba
 ******************************************************************************/


#include "Port.h"

/*
* SW Module Version is 1.0.0
*/
#define PORT_PBCFG_SW_MAJOR_VERSION             (1U)
#define PORT_PBCFG_SW_MINOR_VERSION             (0U)
#define PORT_PBCFG_SW_PATCH_VERSION             (0U)

/*
* AUTOSAR Version is 4.0.3 compatible
*/
#define PORT_PBCFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_PBCFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_PBCFG_AR_RELEASE_PATCH_VERSION     (3U)

/* AUTOSAR Version checking between Port_PBcfg.c and Port.h files */

#if ((PORT_PBCFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_PBCFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_PBCFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
	#error "The AR version of Port.h does not match the expected version"
#endif

/* Software Version checking between Port_PBcfg.c and Port.h files */

#if ((PORT_PBCFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_PBCFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_PBCFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
	#error "The AR version of Port.h does not match the expected version"
#endif

const Port_ConfigType PortConfigrations = {
  
 /* *********** PINS CONFIG OF PORT A ************** */
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_A,PORT_PIN_0,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_A,PORT_PIN_1,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_A,PORT_PIN_2,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_A,PORT_PIN_3,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_A,PORT_PIN_4,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_A,PORT_PIN_5,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_A,PORT_PIN_6,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_A,PORT_PIN_7,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,

 /* *********** PINS CONFIG OF PORT B ************** */
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_B,PORT_PIN_0,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_B,PORT_PIN_1,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_B,PORT_PIN_2,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_B,PORT_PIN_3,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_B,PORT_PIN_4,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_B,PORT_PIN_5,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_B,PORT_PIN_6,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_B,PORT_PIN_7,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,

 /* *********** PINS CONFIG OF PORT C ************** */
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_C,PORT_PIN_0,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_C,PORT_PIN_1,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_C,PORT_PIN_2,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_C,PORT_PIN_3,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_C,PORT_PIN_4,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_C,PORT_PIN_5,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_C,PORT_PIN_6,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_C,PORT_PIN_7,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,

 /* *********** PINS CONFIG OF PORT D ************** */
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_D,PORT_PIN_0,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_D,PORT_PIN_1,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_D,PORT_PIN_2,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_D,PORT_PIN_3,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_D,PORT_PIN_4,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_D,PORT_PIN_5,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_D,PORT_PIN_6,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_D,PORT_PIN_7,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,

 /* *********** PINS CONFIG OF PORT D ************** */
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_D,PORT_PIN_0,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_D,PORT_PIN_1,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_D,PORT_PIN_2,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_D,PORT_PIN_3,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_D,PORT_PIN_4,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_D,PORT_PIN_5,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_D,PORT_PIN_6,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_D,PORT_PIN_7,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,

 /* *********** PINS CONFIG OF PORT E ************** */
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_E,PORT_PIN_0,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_E,PORT_PIN_1,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_E,PORT_PIN_2,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_E,PORT_PIN_3,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_E,PORT_PIN_4,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_E,PORT_PIN_5,PIN_DIRECTION_IS_CHANGEABLE,PIN_MODE_IS_CHANGEABLE,

 /* *********** PINS CONFIG OF PORT F ************** */
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_F,PORT_PIN_0,PIN_DIRECTION_IS_NOT_CHANGEABLE,PIN_MODE_IS_NOT_CHANGEABLE,   /* Push button */
PORT_PIN_MODE_DIO,PORT_PIN_OUT,STD_LOW,PULL_UP,PORT_F,PORT_PIN_1,PIN_DIRECTION_IS_NOT_CHANGEABLE,PIN_MODE_IS_NOT_CHANGEABLE,  /* Red LED */
PORT_PIN_MODE_DIO,PORT_PIN_OUT,STD_LOW,PULL_UP,PORT_F,PORT_PIN_2,PIN_DIRECTION_IS_NOT_CHANGEABLE,PIN_MODE_IS_NOT_CHANGEABLE,  /* Blue LED */
PORT_PIN_MODE_DIO,PORT_PIN_OUT,STD_LOW,PULL_UP,PORT_F,PORT_PIN_3,PIN_DIRECTION_IS_NOT_CHANGEABLE,PIN_MODE_IS_NOT_CHANGEABLE,  /* Green LED */
PORT_PIN_MODE_DIO,PORT_PIN_IN,STD_LOW,PULL_UP,PORT_F,PORT_PIN_4,PIN_DIRECTION_IS_NOT_CHANGEABLE,PIN_MODE_IS_NOT_CHANGEABLE,   /* Push button */

}




