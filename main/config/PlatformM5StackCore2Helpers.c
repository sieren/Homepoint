#include "PlatformM5StackCore2Helpers.h"
#include "axp192.h"
#include "i2c_helper.h"
#include "ili9341.h"
#include "driver/gpio.h"
#include <disp_spi.h>

// Fix whatever M5Stack screwed up when they added the screen
void fix_m5stack_display_rotation()
{
  disp_wait_for_pending_transactions();
  gpio_set_level((gpio_num_t)ILI9341_DC, 0);	 /*Command mode*/
  uint8_t cmd = 0x36;
  disp_spi_send_data(&cmd, 1);
  disp_wait_for_pending_transactions();
  gpio_set_level((gpio_num_t)ILI9341_DC, 1);	 /*Data mode*/
  uint8_t data = 0x08;
  disp_spi_send_data(&data, 1);
}

void set_axp192_gpio_012(const axp192_t *axp, int gpio, bool low)
{
	if ((gpio < 0) || (gpio > 2)) {
		return;
	}

	uint8_t val = 0;
	axp192_read_reg(axp, AXP192_GPIO20_SIGNAL_STATUS, &val);

	uint8_t mask = (1 << gpio);
	if (low) {
		// Value of 0 activates the NMOS, pulling the pin low
		val &= ~mask;
	} else {
		// Value of 1 sets floating
		val |= mask;
	}

	axp192_write_reg(axp, AXP192_GPIO20_SIGNAL_STATUS, val);
}

void set_axp192_gpio_34(const axp192_t *axp, int gpio, bool low)
{
	if ((gpio < 3) || (gpio > 4)) {
		return;
	}

	uint8_t val = 0;
	axp192_read_reg(axp, AXP192_GPIO40_SIGNAL_STATUS, &val);

	uint8_t mask = (1 << (gpio - 3));
	if (low) {
		// Value of 0 activates the NMOS, pulling the pin low
		val &= ~mask;
	} else {
		// Value of 1 sets floating
		val |= mask;
	}

	axp192_write_reg(axp, AXP192_GPIO40_SIGNAL_STATUS, val);
}

void set_led(const axp192_t *axp, bool on)
{
	set_axp192_gpio_012(axp, 1, on);
}

void set_vibration(const axp192_t *axp, bool on)
{
	axp192_set_rail_state(axp, AXP192_RAIL_LDO3, on);
}

void set_internal_5v_bus(const axp192_t *axp, bool on)
{
	// To enable the on-board 5V supply, first N_VBUSEN needs to be pulled
	// high using GPIO0, then we can enable the EXTEN output, to enable
	// the SMPS.
	// To disable it (so either no 5V, or externally supplied 5V), we
	// do the opposite: First disable EXTEN, then leave GPIO0 floating.
	// N_VBUSEN will be pulled down by the on-board resistor.
	// Side note: The pull down is 10k according to the schematic, so that's
	// a 0.5 mA drain from the GPIO0 LDO as long as the bus supply is active.

	if (on) {
		// GPIO0_LDOIO0_VOLTAGE:
		// Bits 7-4: Voltage. 1v8-3v3 in 0.1 V increments
		// Set the GPIO0 LDO voltage to 3v3
		axp192_write_reg(axp, AXP192_GPIO0_LDOIO0_VOLTAGE, 0xf0);

		// GPIO0_CONTROL
		// Bits 7-3: Reserved
		// Bits 2-0: 000: NMOS open drain
		//           001: GPIO
		//           010: Low noise LDO
		//           011: Reserved
		//           100: ADC input
		//           101: Output low
		//           11x: Floating
		// Set to LDO (3v3)
		axp192_write_reg(axp, AXP192_GPIO0_CONTROL, (2 << 0));

		// Enable EXTEN
		axp192_set_rail_state(axp, AXP192_RAIL_EXTEN, true);
	} else {
		// Disable EXTEN
		axp192_set_rail_state(axp, AXP192_RAIL_EXTEN, false);

		// Set GPIO0 to float
		axp192_write_reg(axp, AXP192_GPIO0_CONTROL, (7 << 0));
	}
}