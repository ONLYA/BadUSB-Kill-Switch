/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>

//#ifdef __cplusplus
//extern "C" {
//#endif
//void __cxa_pure_virtual();
//void __cxa_pure_virtual() {
	//// Add custom handler here
	//while(1);
//}
//#ifdef __cplusplus
//}
//#endif

void usb_initialise(void);

volatile bool hid_flag = false;
volatile bool uhc_flag = false;

int main (void)
{
	system_init();	// On init, Warning Indicator (RED) off, OK Indicator (FREEN) on
	irq_initialize_vectors();
	cpu_irq_enable();
	board_init();
	usb_initialise();
	delay_init();
	//sleepmgr_init(); // Optional

	/* Insert application code here, after the board has been initialized. */
	while (1)
	{
		//sleepmgr_enter_sleep(); // Optional
		if (hid_flag)
		{
			// A plugged keyboard BadUSB is detected ...
			// Turn off the USB switch
			port_pin_set_output_level(EN_OUT, true);
			
			// Warning Indicator (RED) on, OK Indicator (GREEN) off
			port_pin_set_output_level(LED_DET, true);
			port_pin_set_output_level(LED_ON, false);
		} 
		else
		{
			if (uhc_flag)
			{
				// If the keyboard is unplugged
				// Warning Indicator (RED) off, OK Indicator (GREEN) on
				port_pin_set_output_level(LED_DET, false);
				port_pin_set_output_level(LED_ON, true);
				
				// Turn on the USB switch
				port_pin_set_output_level(EN_OUT, false);
				uhc_suspend(false);
			} 
			else
			{
				port_pin_set_output_level(LED_DET, false);
				if (uhc_is_suspend())
				{
					// If nothing is connected
					// Blink OK Indicator (GREEN)
					port_pin_toggle_output_level(LED_ON);
					delay_ms(500);
				}
			}
		}
	}
	return 0;
}

void usb_initialise(void)
{
	uhc_start();
}

void my_callback_keyboard_change(uhc_device_t* dev, bool b_plug)
{
	// Do Something
	hid_flag = b_plug;
}

void my_callback_mouse_change(uhc_device_t* dev, bool b_plug)
{
	// Do Something
	hid_flag = b_plug;
}

void usb_host_connection_event(uhc_device_t* dev, bool b_present)
{
	uhc_flag = b_present;
}