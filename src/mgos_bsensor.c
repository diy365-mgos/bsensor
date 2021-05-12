#include "mgos.h"
#include "mg_bsensor_sdk.h"
#include "mgos_bsensor.h"

#ifdef MGOS_HAVE_MJS
#include "mjs.h"
#endif

static void mg_bsensor_poll_cb(void *arg) {
  mgos_bthing_get_state(MGOS_BSENSOR_DOWNCAST((mgos_bsensor_t)arg));
}

static void mg_bsensor_int_cb(int pin, void *arg) {
  if (arg != NULL && MG_BSENSOR_CFG(arg)->int_cfg.pin == pin) {
    mgos_bthing_get_state(MGOS_BSENSOR_DOWNCAST((mgos_bsensor_t)arg));
  }
}

mgos_bsensor_t mgos_bsensor_create(const char *id, enum mgos_bthing_notify_state notify_state) {
  mgos_bsensor_t MG_BSENSOR_NEW(sens);
  if (mg_bsensor_init(sens, id,  MGOS_BSENSOR_TYPE, notify_state)) {
    LOG(LL_INFO, ("bSensor '%s' successfully created.", id));
    return sens;
  }
  free(sens);
  return NULL; 
}

bool mgos_bsensor_polling_set(mgos_bsensor_t sensor, int poll_ticks) {
  if (sensor && poll_ticks > 0) {
    struct mg_bsensor_poll_cfg *cfg = &(MG_BSENSOR_CFG(sensor)->poll_cfg);
    if (cfg) {
      if (cfg->timer_id != MGOS_INVALID_TIMER_ID) mgos_clear_timer(cfg->timer_id);
      cfg->timer_id = mgos_set_timer(poll_ticks, MGOS_TIMER_REPEAT, mg_bsensor_poll_cb, sensor);
      if (cfg->timer_id != MGOS_INVALID_TIMER_ID) {
        cfg->poll_ticks = poll_ticks;
        return true;
      }
    }
  }
  return false;
}

bool mgos_bsensor_interrupt_set(mgos_bsensor_t sensor, int pin,
                                enum mgos_gpio_pull_type pull_type,
                                enum mgos_gpio_int_mode int_mode,
                                int debounce) {
  if (!sensor) return false;
  struct mg_bsensor_int_cfg *cfg = &(MG_BSENSOR_CFG(sensor)->int_cfg);
  if (cfg->pin != MGOS_BTHING_NO_PIN) {
    LOG(LL_ERROR, ("Sensor '%s' already configured to use interrupt on pin %d.",
      mgos_bthing_get_id(MGOS_BSENSOR_DOWNCAST(sensor)), cfg->pin));
    return false;
  }
  if (pin != MGOS_BTHING_NO_PIN && int_mode != MGOS_GPIO_INT_NONE) {
    if (mgos_gpio_set_button_handler(pin, pull_type, int_mode, debounce, mg_bsensor_int_cb, sensor)) {
      cfg->pin = pin;
      cfg->int_mode = int_mode;
      cfg->pull_type = pull_type;
      cfg->debounce = debounce;
      LOG(LL_INFO, ("Interrupt pin %d successfully set for sensor '%s'.", pin,
        mgos_bthing_get_id(MGOS_BSENSOR_DOWNCAST(sensor))));
      return true;
    }
    LOG(LL_ERROR, ("Unable to set interrupt on pin %d for sensor '%s'.",
      pin, mgos_bthing_get_id(MGOS_BSENSOR_DOWNCAST(sensor))));
  }
  LOG(LL_ERROR, ("Unable to set interrupt. Wrong int_pin (%d) or int_mode (%d) parameters.", pin, int_mode));
  return false;
}

bool mgos_bsensor_init() {
  return true;
}