 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Ibraam Wahba
 ******************************************************************************/

#ifndef PORT_H
#define PORT_H

/* Id for the company in the AUTOSAR
 * for example Ibraam Wahba's ID = 1001 :) */
#define PORT_VENDOR_ID    (1001U)

/* Port Module Id */
#define PORT_MODULE_ID    (124U)

/* Port Instance Id */
#define PORT_INSTANCE_ID  (0U)

/*
 * Module Version 1.0.0
 */
#define PORT_SW_MAJOR_VERSION           (1U)
#define PORT_SW_MINOR_VERSION           (0U)
#define PORT_SW_PATCH_VERSION           (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_AR_RELEASE_MAJOR_VERSION   (4U)
#define PORT_AR_RELEASE_MINOR_VERSION   (0U)
#define PORT_AR_RELEASE_PATCH_VERSION   (3U)

/*
 * Macros for Port Status
 */
#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)

/* Standard AUTOSAR types */
#include "Std_Types.h"

/* AUTOSAR checking between Std Types and Dio Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != DIO_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != DIO_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != DIO_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif

 /*Port Pre-Compile Configuration Header file*/ 
#include "Port_Cfg.h"

 /*AUTOSAR Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Port_Cfg.h does not match the expected version"
#endif*/

/* Software Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
  #error "The SW version of Port_Cfg.h does not match the expected version"
#endif 
    
/******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/
/* Service ID for Port init */
#define PORT_INIT_SID               (uint8)0x00
/* Service ID for Port set pin direction */
#define PORT_SET_PIN_DIRECTION      (uint8)0x01
/*Service ID for Port_RefreshPortDirection*/
#define PORT_REFRESH_PORT_DIRECTION (uint8)0x02
/*Service ID for Port get version info*/
#define PORT_GET_VERSION_INFO       (uint8)0x03
/*Service ID for Port set pin mode*/
#define PORT_SET_PIN_MODE           (uint8)0x04

/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/
/* DET code to report Invalid Port Pin ID requested */
#define PORT_E_PARAM_PIN                 (uint8)0x0A
/* DET code to report Port Pin not configured as changeable */
#define PORT_E_DIRECTION_UNCHANGEABLE    (uint8)0x0B
/* DET code to report API Port_Init service called with wrong parameter */
#define PORT_E_PARAM_CONFIG              (uint8)0x0C
/* DET code to report when mode is invalid */
#define PORT_E_PARAM_INVALID_MODE        (uint8)0x0D
/* DET code to report when mode is unchangeable */
#define PORT_E_MODE_UNCHANGEABLE         (uint8)0x0E
/* DET code to report module is not initialized */
#define PORT_E_UNINIT                    (uint8)0x0F
/* DET code to report Null Pointer  */
#define PORT_E_PARAM_POINTER             (uint8)0x10
   
/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/
   
 /* Type definition for Port_PinType used by the PORT APIs */
typedef uint8 Port_PinType;  
 
 /* Type definition for Port_PinModeType used by the PORT APIs */
typedef uint8 Port_PinModeType;

typedef struct 
{
 /* Member contains the mode of the channel*/
 Port_PinModeType PinMode;
#if(PortSetPinDirectionApi == TRUE )
 /* Member contains the Direction of the channel*/
 Port_PinType PinDirection;
#endif
 /* Member contains the Init Value of the channel*/
 Port_PinType PinLevelInitValue;
 /* Member contains kind of RES of the channel*/
 Port_PinType PullUpOrDown;
  /* Member contains the Port number of the channel*/
 Port_PinType PortNum;
  /* Member contains the number the channel*/
 Port_PinType PinNum;
  /* Member contains the ability of change for the channel direction*/
 Port_PinType DirectionChangeable;
  /* Member contains the ability of change for the channel mode*/
 Port_PinType ModeChangeable;
 }Port_ConfigPins;

/* Description: Enum to hold pin direction */
typedef enum 
{
 PORT_PIN_IN,
 PORT_PIN_OUT
 }Port_PinDirectionType ;

/* Description: Enum to hold internal resistor type for PIN */
typedef enum
{
    OFF,PULL_UP,PULL_DOWN
}Port_InternalResistor;

/* Data Structure required for initializing the port Driver */
typedef struct
{
	Port_ConfigPins Pins[PORT_NUMBER_OF_PORT_PINS ];       
}Port_ConfigType;


*******************************************************************************
 *                      Function Prototypes                                    *
*******************************************************************************/   
/* Function Initializes the Port Driver module */
void Port_Init( 
 const Port_ConfigType* ConfigPtr 
);

#if (PortSetPinDirectionApi == TRUE )
/* Function Sets the port pin direction */
void Port_SetPinDirection( 
 Port_PinType Pin, 
 Port_PinDirectionType Direction 
);
#endif

/* Function Refreshes port direction */
void Port_RefreshPortDirection(void);

#if (Port_GetVersionInfo ==STD_ON)
/* Function returns the version information of this module */
void Port_GetVersionInfo( 
 Std_VersionInfoType* versioninfo 
);
#endif

/* Function Sets the port pin mode*/
void Port_SetPinMode( 
 Port_PinType Pin, 
 Port_PinModeType Mode 
);

/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/
extern const Port_ConfigType PortConfigrations;

#endif