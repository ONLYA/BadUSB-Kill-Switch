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

int main (void)
{
	system_init();	// On init, Warning Indicator (RED) off, OK Indicator (FREEN) on
	irq_initialize_vectors();
	cpu_irq_enable();
	sleepmgr_init(); // Optional

	/* Insert application code here, after the board has been initialized. */
	while (1)
	{
		sleepmgr_enter_sleep(); // Optional
	}
}

bool my_callback_keyboard_change(uhc_device_t* dev, bool b_plug)
{
	// Do Something
	if (b_plug)
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
		// If the keyboard is unplugged
		// Warning Indicator (RED) off, OK Indicator (FREEN) on
		port_pin_set_output_level(LED_DET, false);
		port_pin_set_output_level(LED_ON, true);
		
		// Turn on the USB switch
		port_pin_set_output_level(EN_OUT, false);
	}
}

bool my_callback_mouse_change(uhc_device_t* dev, bool b_plug)
{
	// Do Something
	if (b_plug)
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
		// If the keyboard is unplugged
		// Warning Indicator (RED) off, OK Indicator (FREEN) on
		port_pin_set_output_level(LED_DET, false);
		port_pin_set_output_level(LED_ON, true);
		
		// Turn on the USB switch
		port_pin_set_output_level(EN_OUT, false);
	}
}