/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>

#if defined(__GNUC__)
void board_init(void) WEAK __attribute__((alias("system_board_init")));
#elif defined(__ICCARM__)
void board_init(void);
#  pragma weak board_init=system_board_init
#endif

void gpio_init(void);

void system_board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
	gpio_init();
}

void gpio_init(void)
{
	struct port_config config_EN_OUT, config_LED_DET, config_LED_ON;
	port_get_config_defaults(&config_EN_OUT);
	port_get_config_defaults(&config_LED_DET);
	port_get_config_defaults(&config_LED_ON);
	
	config_EN_OUT.direction		= PORT_PIN_DIR_OUTPUT;
	config_LED_DET.direction	= PORT_PIN_DIR_OUTPUT;
	config_LED_ON.direction		= PORT_PIN_DIR_OUTPUT;
	
	port_pin_set_config(EN_OUT,	&config_EN_OUT);
	port_pin_set_config(LED_DET,&config_LED_DET);
	port_pin_set_config(LED_ON,	&config_LED_ON);
	
	port_pin_set_output_level(EN_OUT,	false);	// Connect USB
	port_pin_set_output_level(LED_DET,	false);
	port_pin_set_output_level(LED_ON,	true);
}