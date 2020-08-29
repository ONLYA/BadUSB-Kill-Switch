/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file or main.c
 * to avoid loosing it when reconfiguring.
 */
#include "atmel_start.h"
#include "usb_start.h"

/** USB Host Core driver instance */
struct usbhc_driver USB_HOST_CORE_0_inst;

/** Ctrl requests process buffer size */
#define CTRL_BUFFER_SIZE CONF_USBH_CTRL_BUF_SIZE
/** Ctrl requests process buffer for enumeration */
static uint32_t USB_HOST_CORE_0_ctrl_buf[CTRL_BUFFER_SIZE / 4];

/** Flag to indicate request done */
static volatile bool USB_HOST_CORE_0_req_done;

/**
 * Enumeration callback to handle device state changes
 */
static void USB_HOST_CORE_0_enum_cb(struct usbhc_driver *c, int32_t status, void *param)
{
	if (status == USBHC_ENUM_DISCONNECTED) {
		/* A device is disconnected,
		 * param is pointer to device instance */
		(void)USBHD_PTR(param);
	} else if (status == USBHC_ENUM_SUCCESS) {
		/* A device is connected and enumeration OK,
		 * param is pointer to device instance */
		(void)USBHD_PTR(param);
	}
	(void)c;
}

/**
 * Request end callback invoked when request is done
 */
static void USB_HOST_CORE_0_req_cb(struct usbhd_driver *d, struct usb_h_pipe *pipe)
{
	USB_HOST_CORE_0_req_done = true;
}

/** Enumeration state change handler */
static struct usbhc_handler USB_HOST_CORE_0_enum_h = {NULL, (FUNC_PTR)USB_HOST_CORE_0_enum_cb};
/** Request done handler */
static struct usbhc_handler USB_HOST_CORE_0_req_h = {NULL, (FUNC_PTR)USB_HOST_CORE_0_req_cb};

/**
 * Example of using USB Host Core.
 */
void USB_HOST_CORE_0_example(void)
{
	usbhc_register_handler(&USB_HOST_CORE_0_inst, USBHC_HDL_ENUM, &USB_HOST_CORE_0_enum_h);
	usbhc_register_handler(&USB_HOST_CORE_0_inst, USBHC_HDL_REQ, &USB_HOST_CORE_0_req_h);
	usbhc_start(&USB_HOST_CORE_0_inst);
	while (1) {
		if (!usbhc_is_dev_usable(USBHD_PTR(&USB_HOST_CORE_0_inst))) {
			/* Wait until device connected */
			continue;
		}
		/* Request device descriptor */
		USB_HOST_CORE_0_req_done = false;
		while (ERR_BUSY == usbhc_get_dev_desc(USBHD_PTR(&USB_HOST_CORE_0_inst)))
			;
		while (!USB_HOST_CORE_0_req_done)
			;
		/* Now request is done.
		 *
		 * Device descriptor is at offset USBHC_CTRL_DATA_OFFSET of Ctrl buffer.
		 *
		 * Note that if there are multiple device sharing the control buffer
		 * the data could be changed since the other device may use the buffer.
		 * In that case, create a copy of descriptor in request done callback if
		 * it should be used somewhere.
		 */

		/* Keep waiting until device detach */
		while (usbhc_is_dev_usable(USBHD_PTR(&USB_HOST_CORE_0_inst)))
			;
	}
}

void usb_init(void)
{

	usbhc_init(&USB_HOST_CORE_0_inst, &USB_HOST_INSTANCE_inst, (uint8_t *)USB_HOST_CORE_0_ctrl_buf, CTRL_BUFFER_SIZE);
}
