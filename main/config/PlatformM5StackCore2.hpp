
extern "C"
{
  #include "lvgl.h"
  #include "lvgl_helpers.h"
  #include "axp192.h"
  #include "i2c_helper.h"
  #include <PlatformM5StackCore2Helpers.h>
}

#include <M5TouchAdapter.h>

// Global for convenience
const axp192_t axp = {
  .read = &i2c_read,
  .write = &i2c_write,
};

auto InitializePlatform = []()
{
  lv_init();
	lvgl_driver_init();
 // i2c_init();
  // Voltage configuration
	{

		// don't axp192_init() because we don't wan't to run the initial commands
		// axp192_init(&axp);

		// DCDC1: MCU_VDD  (3v3)
		// DCDC2: Not connected
		// DCDC3: LCD_BL   (2v8)
		// LDO1: RTC, non-configured
		// LDO2: PERI_VDD  (3v3)
		// LDO3: VIB_MOTOR (2v0)
		struct rail_entry {
			const char *name;
			axp192_rail_t rail;
			uint16_t millivolts;
		};

		// The voltages are based on the Arduino demo code.
		struct rail_entry rails[] = {
			{ "DCDC1 (MCU_VDD)",  AXP192_RAIL_DCDC1, 3300 },
			// DCDC2 not connected
			{ "DCDC3 (LCD_BL)",   AXP192_RAIL_DCDC3, 2800 },
			// LDO1 not configurable
			{ "LDO2 (PERI_VDD)",  AXP192_RAIL_LDO2,  3300 },
			{ "LDO3 (VIB_MOTOR)", AXP192_RAIL_LDO3,  2000 },
		};

		for (int i = 0; i < sizeof(rails) / sizeof(rails[0]); i++) {
			bool enabled;
			uint16_t millivolts;
			axp192_err_t err = axp192_get_rail_millivolts(&axp, rails[i].rail, &millivolts);
			if (err != AXP192_ERROR_OK) {
				printf("%s: get failed\n", rails[i].name);
				continue;
			}

			enabled = false;
			err = axp192_get_rail_state(&axp, rails[i].rail, &enabled);
			if (err != AXP192_ERROR_OK) {
				printf("%s: get state failed\n", rails[i].name);
				continue;
			}

			printf("%s: get %d mV (%s)\n", rails[i].name, millivolts, enabled ? "enabled" : "disabled");

			err = axp192_set_rail_millivolts(&axp, rails[i].rail, rails[i].millivolts);
			if (err != AXP192_ERROR_OK) {
				printf("%s: set failed\n", rails[i].name);
				continue;
			}

			printf("%s: set %d mV\n", rails[i].name, rails[i].millivolts);
		}
	}
  // Battery
	{
		// Configure charger, 4.2V 190mA
		axp192_write_reg(&axp, AXP192_CHARGE_CONTROL_1, 0xC1);
		// Default values - 40 min precharge, 8 hour constant current
		axp192_write_reg(&axp, AXP192_CHARGE_CONTROL_2, 0x41);

		float volts;
		axp192_read(&axp, AXP192_BATTERY_VOLTAGE, &volts);
		printf("Battery voltage: %.2f volts\n", volts);
	}

	{
		// GPIO1_CONTROL and GPIO2_CONTROL
		// (Direct translations from the datasheet)
		// Bits 7-3: Reserved
		// Bits 2-0: 000: NMOS open-drain output
		//           001: Universal input function
		//           010: PWM1 output, high level is VINT, no pull down less than 100k can be added
		//           011: Reserved
		//           100: ADC input
		//           101: Low output
		//           11X: Floating

		// Open-drain outputs
		// GPIO1: LED sink
		axp192_write_reg(&axp, AXP192_GPIO1_CONTROL, 0);
		// GPIO2: SPK_EN
		axp192_write_reg(&axp, AXP192_GPIO2_CONTROL, 0);

		// GPIO[4:3]_CONTROL
		// (Direct translations from the datasheet)
		// Bit    7: 1: GPIO function
		// Bits 6-4: Reserved
		// Bits 3-2: GPIO4 Function
		//           00: External charging control
		//           01: NMOS open drain
		//           10: GPIO
		//           11: Reserved
		// Bits 1-0: GPIO3 Function
		//           00: External charging control
		//           01: NMOS open drain
		//           10: GPIO
		//           11: ADC Input
		// Set GPIO4 (LCD_RST) and GPIO3 (NC) to NMOS OD
		axp192_write_reg(&axp, AXP192_GPIO40_FUNCTION_CONTROL, (1 << 7) | (1 << 2) | (1 << 0));

		// PEK (power enable key)
		// Bits 7-6: Boot time
		//           00: 128 ms
		//           01: 512 ms
		//           10: 1 s
		//           11: 2 s
		// Bits 5-4: Long press time
		//           00: 1 s
		//           01: 1.5 s
		//           10: 2 s
		//           11: 2.5 s
		// Bit 3: Shutdown on long press
		// Bit 2: PWROK delay
		//           0: 32 ms
		//           1: 64 ms
		// Bit 1-0: Shutdown button duration
		//           00: 4 s
		//           01: 6 s
		//           10: 8 s
		//           11: 10 s
		// Copy setting from Arduino (0x4c)
		axp192_write_reg(&axp, AXP192_PEK, (1 << 6) | (0 << 4) | (1 << 3) | (1 << 2) | (0 << 0));

		// Arduino code enables all ADC channels
		//axp192_write_reg(&axp, AXP192_ADC_ENABLE_1, 0xff);

		// BATTERY_CHARGE_CONTROL
		// Bit    7: Backup battery charging enable
		// Bits 6-5: Backup battery target voltage
		//           00: 3.1 V
		//           01: 3.0 V
		//           10: 3.0 V (duplicated?)
		//           11: 2.5 V
		// Bits 4-2: Reserved
		// Bits 1-0: Backup battery charge current
		//           00: 50 uA
		//           01: 100 uA
		//           10: 200 uA
		//           11: 400 uA
		axp192_write_reg(&axp, AXP192_BATTERY_CHARGE_CONTROL, (1 << 7) | (1 << 5) | (0 << 0));
	}

  // Start UI ---------------------------------

  // Backlight
  axp192_set_rail_state(&axp, AXP192_RAIL_DCDC3, true);

  // Logic
  axp192_set_rail_state(&axp, AXP192_RAIL_LDO2, true);

  // Wait a bit for everything to settle
  vTaskDelay(100 / portTICK_PERIOD_MS);

  // Reset and re-initialise the LCD and touch controller
  // (see comment above lvgl_driver_init())
  set_axp192_gpio_34(&axp, 4, true);
  vTaskDelay(100 / portTICK_PERIOD_MS);
  set_axp192_gpio_34(&axp, 4, false);
  vTaskDelay(100 / portTICK_PERIOD_MS);
  touch_driver_init();
  disp_driver_init();
  fix_m5stack_display_rotation();
  Serial.println("Started Platform");
};
