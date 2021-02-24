#ifndef __M5PLT_H
#define __M5PLT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "axp192.h"

void fix_m5stack_display_rotation();
void set_axp192_gpio_012(const axp192_t *axp, int gpio, bool low);

void set_axp192_gpio_34(const axp192_t *axp, int gpio, bool low);

void set_led(const axp192_t *axp, bool on);

void set_vibration(const axp192_t *axp, bool on);
void set_internal_5v_bus(const axp192_t *axp, bool on);

#ifdef __cplusplus
}
#endif

#endif /* __M5PLT_H */


