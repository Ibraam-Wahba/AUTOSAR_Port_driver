 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Ibraam Wahba 
 ******************************************************************************/

#include "Port.h"
#include "tm4c123gh6pm_registers.h"
#include "Port_reg.h"
#include "Common_Macros.h"
#include "Det.h"

#if (PORT_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"
/* AUTOSAR Version checking between Det and Dio Modules */
#if ((DET_AR_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 || (DET_AR_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 || (DET_AR_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Det.h does not match the expected version"
#endif

#endif



/*
* Service name: Port_Init 
* Service ID[hex]: 0x00 
* Sync/Async: Synchronous 
* Reentrancy: Non Reentrant 
* Parameters (in): ConfigPtr Pointer to configuration set. 
* Parameters (out): None 
* Return value: None 
* Description: Initializes the Port Driver module.
*/

STATIC uint8 PortStatus = PORT_NOT_INITIALIZED;
STATIC  const Port_ConfigPins * PortConf_Ptr = NULL_PTR;

void Port_Init(const Port_ConfigType* ConfigPtr )
{
  #if (POrt_DEV_ERROR_DETECT == STD_ON)
	/* check if the input configuration pointer is not a NULL_PTR */
	if (NULL_PTR == ConfigPtr)
	{
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,
		     PORT_E_PARAM_CONFIG);
	}
	else
 #endif
    volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
    volatile uint32 delay = 0;

        PortConf_Ptr = ConfigPtr->Pins;     /*points to the 1st address of the array of structure*/
        uint8 PinIndex = 0  ;                   /* to hold index of pin from 0 to 42*/
        while (PinIndex <= PORT_CONFIGURED_CHANNLES)
        {
    switch(PortConf_Ptr[PinIndex].PortNum)
    {
        case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
		 break;
	case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
		 break;
	case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
		 break;
	case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
		 break;
        case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
		 break;
        case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
		 break;
    }
    
    /* Enable clock for PORT and allow time for clock to start*/
    SYSCTL_REGCGC2_REG |= (1<<PortConf_Ptr[PinIndex].PortNum);
    delay = SYSCTL_REGCGC2_REG;
      
     if( ((PortConf_Ptr[PinIndex].PortNum == 3) && (PortConf_Ptr[PinIndex].PinNum == 7)) || ((PortConf_Ptr[PinIndex].PortNum == 5) && (PortConf_Ptr[PinIndex].PinNum == 0)) ) /* PD7 or PF0 */
    {
        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;                     /* Unlock the GPIOCR register */   
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_COMMIT_REG_OFFSET) , ConfigPtr->PinNum);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
    }
    else if( (PortConf_Ptr[PinIndex].PortNum == 2) && (PortConf_Ptr[PinIndex].PinNum <= 3) ) /* PC0 to PC3 */
    {
        /* Do Nothing ...  this is the JTAG pins */
        ++ PinIndex;
        continue;
    }
    else
    {
        /* Do Nothing ... No need to unlock the commit register for this pin */
    }
      if(PortConf_Ptr[PinIndex].PinMode == PORT_PIN_MODE_UART)
        { 
          CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);               /* ENable Alternative function for this pin*/
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);      /*Enable Digital*/ 
          if(PortConf_Ptr[PinIndex].PinNum == 20|| PortConf_Ptr[PinIndex].PinNum == 21) /*PC4 ,PC5*/
          {
            *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000002 << (PortConf_Ptr[PinIndex].PinNum * 4));
          }
          else
          {
            *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000001 << (PortConf_Ptr[PinIndex].PinNum * 4));
          }
           
        }
      /*************************************************/  
        
      else if(PortConf_Ptr[PinIndex].PinMode == PORT_PIN_MODE_DIO)
        {
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (PortConf_Ptr[PinIndex].PinNum * 4));     /* Clear the PMCx bits for this pin */
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */ 
        }
       /********************************************/ 
        else if(PortConf_Ptr[PinIndex].PinMode == PORT_PIN_MODE_ADC)
        {  
          
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);               /* ENable Alternative function for this pin*/
          CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);            /*SEt as an input pin*/
          CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,PortConf_Ptr[PinIndex].PinNum);  /*Disable Digital mode */      
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);   /*Enable analog functionality on this pin */
        }
         /**************************************/
        else if(PortConf_Ptr[PinIndex].PinMode == PORT_PIN_MODE_CAN)
         {        
           
           SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);               /* ENable Alternative function for this pin*/
           SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);
           if(PortConf_Ptr[PinIndex].PinNum == 38 || PortConf_Ptr[PinIndex].PinNum == 41)/*PF0,PF3*/
          {
            *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000003 << (PortConf_Ptr[PinIndex].PinNum * 4));
          }
          else
          {
           *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008 << (PortConf_Ptr[PinIndex].PinNum * 4));         
         }
         /*Tx(output) pin or Rx(input)*/
         }
         /********************************************/
        else if(PortConf_Ptr[PinIndex].PinMode == PORT_PIN_MODE_SSI)
        {    /*RCGCSSI register*/
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum); /*set as Digital*/
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) ,PortConf_Ptr[PinIndex].PinNum);           /*PULLUP */
          if(PortConf_Ptr[PinIndex].PinNum == 24 || PortConf_Ptr[PinIndex].PinNum == 25 || PortConf_Ptr[PinIndex].PinNum == 26 || PortConf_Ptr[PinIndex].PinNum == 27)/*PD0,PD1,PD2,PD3*/
          { *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000001 << (PortConf_Ptr[PinIndex].PinNum * 4));  
          }
          else
          {
         *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000002 << (PortConf_Ptr[PinIndex].PinNum * 4)); 
         
        }
        }
     /*******************************************/
       else if(PortConf_Ptr[PinIndex].PinMode == PORT_PIN_MODE_I2C)
       {
         SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);
         SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);
         SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_OPEN_DRAIN) , PortConf_Ptr[PinIndex].PinNum);  /*OPen drain select*/
         *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000003 << (PortConf_Ptr[PinIndex].PinNum * 4));
          
       }
      /****************************************/
     else if(PortConf_Ptr[PinIndex].PinMode == PORT_PIN_MODE_PWM)
     {   
       
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);
       if(PortConf_Ptr[PinIndex].PinNum == 12 || PortConf_Ptr[PinIndex].PinNum == 13 || PortConf_Ptr[PinIndex].PinNum == 14 || PortConf_Ptr[PinIndex].PinNum == 15 || PortConf_Ptr[PinIndex].PinNum == 20 ||PortConf_Ptr[PinIndex].PinNum == 21 || PortConf_Ptr[PinIndex].PinNum == 24 || PortConf_Ptr[PinIndex].PinNum == 25| PortConf_Ptr[PinIndex].PinNum == 36 || PortConf_Ptr[PinIndex].PinNum == 37)/*PB4--PB7 , PC4,5  ,PD0,1 ,PE4,5*/
       {
       *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000004 << (PortConf_Ptr[PinIndex].PinNum * 4));
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);
      }
       else
       {
         *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000005<< (PortConf_Ptr[PinIndex].PinNum * 4));
         SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);
       }}
       /**********************************/
       else 
         if(PortConf_Ptr[PinIndex].PinMode == PORT_PIN_MODE_USB)
       {
         *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008<< (PortConf_Ptr[PinIndex].PinNum * 4));
       }
  
       else 
         if(PortConf_Ptr[PinIndex].PinMode == PORT_PIN_MODE_QEI) 
         {   
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);
           *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000006<< (PortConf_Ptr[PinIndex].PinNum * 4));
             SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);
         }
  
      else 
         if(PortConf_Ptr[PinIndex].PinMode == PORT_PIN_MODE_GPT) 
         {   
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);               /* ENable Alternative function for this pin*/
           *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000007<< (PortConf_Ptr[PinIndex].PinNum * 4));
           SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);
         } 
      else
        if(PortConf_Ptr[PinIndex].PinMode == PORT_PIN_MODE_NMI) 
         {    
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);
           *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008<< (PortConf_Ptr[PinIndex].PinNum * 4));
           SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,PortConf_Ptr[PinIndex].PinNum);
         }  
  
        if(PortConf_Ptr[PinIndex].PinMode == PORT_PIN_MODE_ANALOG_COMP) 
         {    
           SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);
            if(PortConf_Ptr[PinIndex].PinNum == 20 ||PortConf_Ptr[PinIndex].PinNum == 21 || PortConf_Ptr[PinIndex].PinNum == 22 || PortConf_Ptr[PinIndex].PinNum == 23 )  /*PC4,5,6,7 analog pins  C1-,C1+,C0+,C0-*/
            { 
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);   /*Enable analog functionality on this pin */
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);  /*Disable digital mode*/
            } else
            {
           *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000009<< (PortConf_Ptr[PinIndex].PinNum * 4));
           SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);
          
            }  
         }
       if(PortConf_Ptr[PinIndex].PinMode==PORT_PIN_MODE_CORE) 
         {    
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);
           *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000000E<< (PortConf_Ptr[PinIndex].PinNum * 4));
           SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum);
         }  
      if(PortConf_Ptr[PinIndex].PinDirection == PORT_PIN_OUT)
    {
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) ,PortConf_Ptr[PinIndex].pin_num);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
        
        if(PortConf_Ptr[PinIndex].PinLevelInitValue == STD_HIGH)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , PortConf_Ptr[PinIndex].pin_num);          /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , PortConf_Ptr[PinIndex].pin_num);        /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
        }
    }
       if(PortConf_Ptr[PinIndex].PinDirection == PORT_PIN_IN)
    {
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , PortConf_Ptr[PinIndex].pin_num);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
    
     if(PortConf_Ptr[PinIndex].PullUpOrDown == PULL_UP)
     {
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , PortConf_Ptr[PinIndex].pin_num);       /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
     }
     else if(PortConf_Ptr[PinIndex].PullUpOrDown == PULL_DOWN)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , PortConf_Ptr[PinIndex].pin_num);     /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , PortConf_Ptr[PinIndex].pin_num);     /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , PortConf_Ptr[PinIndex].pin_num);   /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
        }
    }
     
      SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[PinIndex].PinNum); /* enable digital function*/
      PinIndex ++;
        }
        
     PortStatus = PORT_INITIALIZED;
}

/*
* Service name: Port_SetPinDirection 
* Service ID[hex]: 0x01 
* Sync/Async: Synchronous 
* Reentrancy: Reentrant 
* Pin Port Pin ID number Parameters (in): Direction Port Pin Direction 
* Parameters(inout): None  
* Parameters (out): None 
* Return value: None 
* Description: Sets the port pin direction 
*/
#if (PortSetPinDirectionApi == TRUE )
void Port_SetPinDirection( Port_PinType Pin, Port_PinDirectionType Direction )
{
        volatile uint32 * PortDio_Ptr = NULL_PTR; /* for pointing to the required Port Registers base address */
	boolean error = FALSE;
        
 #if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* check if the port is initialized */
	if (PORT_NOT_INITIALIZED == PortStatus)
	{
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION,
		     PORT_E_UNINIT);
                error = TRUE;
	}
	else
        {
          /* do no thing */
        }
	/* Check if the used channel is within the valid range */
	  
  if(PortConf_Ptr[Pin].PortNum == PORT_E)
  {
  if (Pin > PORT_PIN_5)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE,
		PORT_E_PARAM_PIN);
  }
  else {}
  }
  
  else if(PortConf_Ptr[Pin].PortNum == PORT_F)
  {
    if (Pin > PORT_PIN_4)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE,
		PORT_E_PARAM_PIN);
  }
  else {}
  }
  
  
  else  
	{
          if (Pin >PORT_PIN_7 ) 
          {
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE,
		PORT_E_PARAM_PIN);
          }
          else {}
	} 
             
        /* Check if the pin direction is changable */
        if (FALSE == PortConf_Ptr[Pin].DirectionChangeable)
	{
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION,
		     PORT_E_DIRECTION_UNCHANGEABLE);
                error = TRUE;
	}
	else
        {
          /* do no thing */
        }
 #endif
        
  if(FALSE == error)
  {
    switch(PortConf_Ptr[Pin].PortNum)
    {
        case  0: PortDio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
		 break; 
	case  1: PortDio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
		 break;
	case  2: PortDio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
		 break;
	case  3: PortDio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
		 break;
        case  4: PortDio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
		 break;
        case  5: PortDio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
		 break;
    }

   if(Direction == PORT_PIN_OUT)
    {
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortDio_Ptr + PORT_DIR_REG_OFFSET) , Pin);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
    }
   if(Direction == PORT_PIN_IN)
    {
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortDio_Ptr + PORT_DIR_REG_OFFSET) , Pin);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
        
    }
     
    }
  else
   {
         /* do no thing */
   }
}
/*
* Service name: Port_RefreshPortDirection 
* Service ID[hex]: 0x02 
* Sync/Async: Synchronous 
* Reentrancy: Non Reentrant 
* Parameters (in): None 
* Parameters None 
* Parameters (out): None 
* Return value: None 
* Description: Refreshes port direction. 
*/
void Port_RefreshPortDirection(void)
{
  #if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* check if the port is initialized */
	if (PORT_NOT_INITIALIZED == PortStatus)
	{
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_REFRESH_PORT_DIRECTION,
		     PORT_E_UNINIT);
                
	}
	else
        {
          /* do no thing */
        }
#endif 
    uint8 PinIndex = 0 ;
    volatile uint32* PortGpio_Ptr = NULL_PTR;
    
            while (PinIndex <= PORT_CONFIGURED_CHANNLES)
            {
              
        switch(PortConf_Ptr[PinIndex].PortNum)
    {
        case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
		 break;
	case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
		 break;
	case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
		 break;
	case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
		 break;
        case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
		 break;
        case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
		 break;
    }
    
    /*exclude those port pins from refreshing that are configured as pin direction changeable during runtime*/
  if (PortConf_Ptr[PinIndex].ModeChangeable == TRUE)
  { 
    ++ PinIndex;
    continue;
  }
  
  /* check if this pin direction is output*/
  else if (BIT_IS_SET(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET),PortConf_Ptr[PinIndex].PinDirection))
  {
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET),PortConf_Ptr[PinIndex].PinDirection);  /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
  }
  
  /* check if this pin direction is output*/  
  else if (BIT_IS_CLEAR(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET),PortConf_Ptr[PinIndex].PinDirection))
  {
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET),PortConf_Ptr[PinIndex].PinDirection);  /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
  }
  
  else 
  {
     /* do no thing*/
  }  
 ++ PinIndex;
       }
}
/*
* Service name: Port_GetVersionInfo  
* Service ID[hex]: 0x03 
* Sync/Async: Synchronous 
* Reentrancy: Non Reentrant 
* Parameters (in): None 
* Parameters (inout): None 
* Parameters (out): versioninfo Pointer to where to store the version information of this module. 
* Return value: None 
* Description: Returns the version information of this module.
*/
#if (Port_GetVersionInfo ==STD_ON)

void Port_GetVersionInfo(Std_VersionInfoType* versioninfo )     
{
 #if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* check for null pointer */
	if (NULL_PTR == versioninfo)
	{
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GET_VERSION_INFO,
		     PORT_E_PARAM_POINTER);  
	}
	else
        {
          /* do no thing */
        }
#endif 

        versioninfo->moduleID = (uint16) PORT_MODULE_ID;
        versioninfo->vendorID = (uint16) PORT_VENDOR_ID;
        versioninfo->sw_major_version = (uint8) PORT_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = (uint8) PORT_SW_MINOR_VERSION_VERSION;
        versioninfo->sw_patch_version = (uint8) PORT_SW_PATCH_VERSION_VERSION;
}

#endif
/* 
* Service Name: Port_SetPinMode
* Sync/Async: Synchronous
* Service ID[hex]:0x04
* Reentrancy: reentrant
* Parameters (in): Pin > Port Pin ID number
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin mode.
*/
void Port_SetPinMode( Port_PinType Pin, Port_PinModeType Mode )

{ 
  if (PortStatus == PORT_NOT_INITIALIZED)
	{
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE,
		PORT_E_UNINIT);
                
	}
	else
	{
/* do no thing */
	}
 
  /* Incorrect Port Pin ID passed*/ 
  if(PortConf_Ptr[Pin].PortNum == PORT_E)
  {
  if (Pin > PORT_PIN_5)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE,
		PORT_E_PARAM_PIN);
  }
  else {}
  }
  
  else if(PortConf_Ptr[Pin].PortNum == PORT_F)
  {
    if (Pin > PORT_PIN_4)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE,
		PORT_E_PARAM_PIN);
  }
  else {}
  }
  
  
  else  
	{
          if (Pin >PORT_PIN_7 ) 
          {
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE,
		PORT_E_PARAM_PIN);
          }
          else {}
	} 
         /* Incorrect Port Pin Mode passed*/
    if ((Mode > PORT_PIN_MODE_CORE) || (Mode < PORT_PIN_MODE_ADC ))
	{
		
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE ,
		     PORT_E_PARAM_INVALID_MODE);
        } 
    else
        {
        }
  
    /* check if Port Pin mode not configured as changeable */
	if( PortConf_Ptr[Pin].ModeChangeable == PIN_DIRECTION_IS_NOT_CHANGEABLE)
        {
          Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE ,
		     PORT_E_MODE_UNCHANGEABLE);
        }
   else
   {}
  
  volatile uint32 * PortGpio_Ptr = NULL_PTR;
  switch( PortConf_Ptr[Pin].PortNum)
    {
        case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
		 break; 
	case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
		 break;
	case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
		 break;
	case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
		 break;
        case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
		 break;
        case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
		 break;
    }


        
       if(Mode == PORT_PIN_MODE_UART)
        { 
          CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);               /* ENable Alternative function for this pin*/
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);      /*Enable Digital*/ 
          if(Pin==20||Pin==21) /*PC4 ,PC5*/
          {
            *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000002 << (PortConf_Ptr[Pin].PinNum * 4));
          }
          else
          {
            *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000001 << (PortConf_Ptr[Pin].PinNum * 4));
          }
           
        }
      /*************************************************/  
        
      else if(Mode==PORT_PIN_MODE_DIO)
        {
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (PortConf_Ptr[Pin].PinNum * 4));     /* Clear the PMCx bits for this pin */
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */ 
        }
       /********************************************/ 
        else if(Mode==PORT_PIN_MODE_ADC)
        {  
          /*Should i enable RCGCADC and PLL ??*/
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);               /* ENable Alternative function for this pin*/
          CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);            /*SEt as an input pin*/
          CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,PortConf_Ptr[Pin].PinNum);  /*Disable Digital mode */      
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);   /*Enable analog functionality on this pin */
        }
         /**************************************/
        else if(Mode==PORT_PIN_MODE_CAN)
         {        
           
           SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);               /* ENable Alternative function for this pin*/
           SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);
           if(Pin==38 ||Pin==41)/*PF0,PF3*/
          {
            *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000003 << (PortConf_Ptr[Pin].PinNum * 4));
          }
          else
          {
           *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008 << (PortConf_Ptr[Pin].PinNum * 4));         
         }
         /*Tx(output) pin or Rx(input)*/
         }
         /********************************************/
        else if(Mode==PORT_PIN_MODE_SSI)
        {    /*RCGCSSI register???*/
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[Pin].PinNum); /*set as Digital*/
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) ,PortConf_Ptr[Pin].PinNum);           /*PULLUP */
          if(Pin==24 ||Pin==25 ||Pin==26 ||Pin==27)/*PD0,PD1,PD2,PD3*/
          { *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000001 << (PortConf_Ptr[Pin].PinNum * 4));  
          }
          else
          {
         *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000002 << (PortConf_Ptr[Pin].PinNum * 4)); 
         
        }
        }
     /*******************************************/
       else if(Mode==PORT_PIN_MODE_I2C)
       {
         SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);
         SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);
         SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_OPEN_DRAIN) , PortConf_Ptr[Pin].PinNum);  /*OPen drain select*/
         *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000003 << (PortConf_Ptr[Pin].PinNum * 4));
          
       }
      /****************************************/
     else if(Mode==PORT_PIN_MODE_PWM)
     {   
       
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);
       if(Pin==12 ||Pin==13 ||Pin==14 ||Pin==15||Pin==20 ||Pin==21 ||Pin==24||Pin==25|Pin==36 ||Pin==37)/*PB4--PB7 , PC4,5  ,PD0,1 ,PE4,5*/
       {
       *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000004 << (PortConf_Ptr[Pin].PinNum * 4));
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);
      }
       else
       {
         *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000005<< (PortConf_Ptr[Pin].PinNum * 4));
         SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);
       }}
       /**********************************/
       else 
         if(Mode==PORT_PIN_MODE_USB)
       {
         *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008<< (PortConf_Ptr[Pin].PinNum * 4));
       }
  
       else 
         if(Mode==PORT_PIN_MODE_QEI) 
         {   
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);
           *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000006<< (PortConf_Ptr[Pin].PinNum * 4));
             SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);
         }
  
      else 
         if(Mode==PORT_PIN_MODE_GPT) 
         {   
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);               /* ENable Alternative function for this pin*/
           *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000007<< (PortConf_Ptr[Pin].PinNum * 4));
           SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);
         } 
      else
        if(Mode==PORT_PIN_MODE_NMI) 
         {    
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);
           *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008<< (PortConf_Ptr[Pin].PinNum * 4));
           SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,PortConf_Ptr[Pin].PinNum);
         }  
  
        if(Mode==PORT_PIN_MODE_ANALOG_COMP) 
         {    
           SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);
            if(Pin==20 ||Pin==21 ||Pin==22 ||Pin==23 )  /*PC4,5,6,7 analog pins  C1-,C1+,C0+,C0-*/
            { 
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);   /*Enable analog functionality on this pin */
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);  /*Disable digital mode*/
            } else
            {
           *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000009<< (PortConf_Ptr[Pin].PinNum * 4));
           SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);
          
            }  
         }
       if(Mode==PORT_PIN_MODE_CORE) 
         {    
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);
           *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000000E<< (PortConf_Ptr[Pin].PinNum * 4));
           SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf_Ptr[Pin].PinNum);
         }  
  
        
}


#endif
