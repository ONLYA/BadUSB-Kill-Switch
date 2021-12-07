/*
 * uhi_hid_keyboard.c
 *
 * Created: 30/11/2021 23:59:24
 *  Author: anxia
 */ 

#include "conf_usb_host.h"
#include "usb_protocol.h"
#include "uhd.h"
#include "uhc.h"
#include "uhi_hid_keyboard.h"
#include <string.h>
#include <stdlib.h>

#ifdef USB_HOST_HUB_SUPPORT
# error USB HUB support is not implemented on UHI keyboard
#endif

#define UHI_HID_KEYBOARD_MODIFIERS   0
#define UHI_HID_KEYBOARD_RESERVED    1
#define UHI_HID_KEYBOARD_KEY_0       2
#define UHI_HID_KEYBOARD_KEY_1       3
#define UHI_HID_KEYBOARD_KEY_2       4
#define UHI_HID_KEYBOARD_KEY_3       5
#define UHI_HID_KEYBOARD_KEY_4       6
#define UHI_HID_KEYBOARD_KEY_5       7

typedef struct {
	uhc_device_t *dev;
	usb_ep_t ep_in;
	uint8_t report_size;
	uint8_t *report;
}uhi_hid_keyboard_dev_t;

static uhi_hid_keyboard_dev_t uhi_hid_keyboard_dev = {
	.dev = NULL,
	.report = NULL,
};

static void uhi_hid_keyboard_start_trans_report(usb_add_t add);
static void uhi_hid_keyboard_report_reception(
usb_add_t add,
usb_ep_t ep,
uhd_trans_status_t status,
iram_size_t nb_transfered);

uhc_enum_status_t uhi_hid_keyboard_install(uhc_device_t* dev)
{
	bool b_iface_supported;
	uint16_t conf_desc_lgt;
	usb_iface_desc_t *ptr_iface;

	if (uhi_hid_keyboard_dev.dev != NULL) {
		return UHC_ENUM_SOFTWARE_LIMIT; // Device already allocated
	}
	conf_desc_lgt = le16_to_cpu(dev->conf_desc->wTotalLength);
	ptr_iface = (usb_iface_desc_t*)dev->conf_desc;
	b_iface_supported = false;
	while(conf_desc_lgt) {
		switch (ptr_iface->bDescriptorType) {

			case USB_DT_INTERFACE:
			if ((ptr_iface->bInterfaceClass   == HID_CLASS)
			&& (ptr_iface->bInterfaceProtocol == HID_PROTOCOL_KEYBOARD) ) {
				// USB HID Keyboard interface found
				// Start allocation endpoint(s)
				b_iface_supported = true;
				} else {
				// Stop allocation endpoint(s)
				b_iface_supported = false;
			}
			break;

			case USB_DT_ENDPOINT:
			//  Allocation of the endpoint
			if (!b_iface_supported) {
				break;
			}
			if (!uhd_ep_alloc(dev->address, (usb_ep_desc_t*)ptr_iface, dev->speed)) {
				return UHC_ENUM_HARDWARE_LIMIT; // Endpoint allocation fail
			}
			Assert(((usb_ep_desc_t*)ptr_iface)->bEndpointAddress & USB_EP_DIR_IN);
			uhi_hid_keyboard_dev.ep_in = ((usb_ep_desc_t*)ptr_iface)->bEndpointAddress;
			uhi_hid_keyboard_dev.report_size =
			le16_to_cpu(((usb_ep_desc_t*)ptr_iface)->wMaxPacketSize);
			uhi_hid_keyboard_dev.report = malloc(uhi_hid_keyboard_dev.report_size);
			if (uhi_hid_keyboard_dev.report == NULL) {
				Assert(false);
				return UHC_ENUM_MEMORY_LIMIT; // Internal RAM allocation fail
			}
			uhi_hid_keyboard_dev.dev = dev;
			// All endpoints of all interfaces supported allocated
			return UHC_ENUM_SUCCESS;

			default:
			// Ignore descriptor
			break;
		}
		Assert(conf_desc_lgt>=ptr_iface->bLength);
		conf_desc_lgt -= ptr_iface->bLength;
		ptr_iface = (usb_iface_desc_t*)((uint8_t*)ptr_iface + ptr_iface->bLength);
	}
	return UHC_ENUM_UNSUPPORTED; // No interface supported
}

void uhi_hid_keyboard_enable(uhc_device_t* dev)
{
	if (uhi_hid_keyboard_dev.dev != dev) {
		return;  // No interface to enable
	}

	// Init value
	uhi_hid_keyboard_start_trans_report(dev->address);
	UHI_HID_KEYBOARD_CHANGE(dev, true);
}

void uhi_hid_keyboard_uninstall(uhc_device_t* dev)
{
	if (uhi_hid_keyboard_dev.dev != dev) {
		return; // Device not enabled in this interface
	}
	uhi_hid_keyboard_dev.dev = NULL;
	Assert(uhi_hid_keyboard_dev.report!=NULL);
	free(uhi_hid_keyboard_dev.report);
	UHI_HID_KEYBOARD_CHANGE(dev, false);
}

static void uhi_hid_keyboard_start_trans_report(usb_add_t add)
{
	// Start transfer on interrupt endpoint IN
	uhd_ep_run(add, uhi_hid_keyboard_dev.ep_in, true, uhi_hid_keyboard_dev.report,
	uhi_hid_keyboard_dev.report_size, 0, uhi_hid_keyboard_report_reception);
}

static void uhi_hid_keyboard_report_reception(
usb_add_t add,
usb_ep_t ep,
uhd_trans_status_t status,
iram_size_t nb_transfered)
{
	UNUSED(ep);

	if ((status == UHD_TRANS_NOTRESPONDING) || (status == UHD_TRANS_TIMEOUT)) {
		uhi_hid_keyboard_start_trans_report(add);
		return; // HID keyboard transfer restart
	}

	if ((status != UHD_TRANS_NOERROR) || (nb_transfered < 4)) {
		return; // HID keyboard transfer aborted
	}

	// decode modifiers and keys
	uint8_t modifiers = uhi_hid_keyboard_dev.report[UHI_HID_KEYBOARD_MODIFIERS];
	uint8_t key0 = uhi_hid_keyboard_dev.report[UHI_HID_KEYBOARD_KEY_0];

	// TODO -- replace with your code to handle the keyboard key presses and releases
	//printf ("%02x %02x\n\r", modifiers, key0);
	
	uhi_hid_keyboard_start_trans_report(add);
}
//@}

//@}