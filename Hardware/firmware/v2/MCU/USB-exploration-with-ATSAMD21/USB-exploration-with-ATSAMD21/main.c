/*
 * USB-exploration-with-ATSAMD21.c
 *
 * Created: 05/12/2021 18:21:30
 * Author : anxia
 */ 


#include "sam.h"
#include "usb_host_register_def.h"

/* Definitions */
#define LED_HID			1 // HID device inserted LED Indicator
#define LED_OTHER		2 // Other device inserted LED Indicator
#define LED_DETECTED	3 // Dangerous device detected LED Indicator
#define LED_TRUST_OUT	4 // Output /w Trusted status LED Indicator

#define BUTTON_TRUST	5 // Trust Button Input

#define SPI_MISO		6
#define SPI_MOSI		7
#define SPI_SCLK		8
#define SPI_CS			9

#define SDA				10 // For EEPROM AT24C01D-STUM-T or AT24CSW020
#define SCL				11

struct USB_Dev_t
{
	int id;
	};

typedef void (*usb_callback_function)(struct USB_Dev_t *dev);

/* Initialisation functions */
void init_sysclock(void);
void init_system(void);
void init_PM(void);
void init_IO(void);
void init_NVIC(void);
void init_USB_host(void);

/* Interrupt functions */
static void _callback_CONNECTED(struct USB_Dev_t *dev);
static void _callback_DISCONNECTED(struct USB_Dev_t *dev);
static void _callback_SOF(struct USB_Dev_t *dev);			// Start of Frame
static void _callback_RESET(struct USB_Dev_t *dev);

int main(void)
{
	/* Initialisation */
	init_sysclock();
	init_system();
	init_PM();
	init_IO();
	init_NVIC();
	init_USB_host();
	
	/* Enable Global interrupt */
	//HWREG32(0xff) = 0;
    /* Replace with your application code */
    while (1) 
    {
		/* The USB management is entirely managed by interrupts.
		 * As a consequence, the user application does only have to play with the power modes. */
		//
    }
}

/************************************************************************/
/* Initialise the system clock                                          */
/************************************************************************/
void init_sysclock()
{
	/* Choose the Clock sources */
	/* Config Generic Clock Controller (GCLK) */
}

/************************************************************************/
/* Initialise the system controller                                     */
/************************************************************************/
void init_system()
{
	/* Brown-Out Detector */
	/* Voltage Ref System for IC temperature control */
}

/************************************************************************/
/* Initialise the power management system                               */
/************************************************************************/
void init_PM()
{
	/**/
}

/************************************************************************/
/* Initialise the IO lines                                              */
/************************************************************************/
void init_IO()
{
	/* Multiplex the required IOs */
	/* Set IO directions */
	/* Init GPIO outputs */
}

/************************************************************************/
/* Initialise the Nested Vector Interrupt Controller                    */
/************************************************************************/
void init_NVIC()
{
	/**/
}

/************************************************************************/
/* Initialise the USB Host Controller                                   */
/************************************************************************/
void init_USB_host()
{
	/* USB clock init */
	
	/* Enable Interrupts */
}