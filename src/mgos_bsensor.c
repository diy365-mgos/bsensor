#include "mgos.h"
#include "mg_bsensor_sdk.h"
#include "mgos_bsensor.h"

#ifdef MGOS_HAVE_MJS
#include "mjs.h"
#endif

mgos_bthing_t MGOS_BSENSOR_DOWNCAST(mgos_bsensor_t sensor) {
  return (mgos_bthing_t)sensor;
}

static void mg_bsensor_poll_cb(void *sens) {
  mgos_bthing_t thing = MGOS_BSENSOR_DOWNCAST((mgos_bsensor_t)sens);
  if (!mgos_bthing_get_state(thing)) {
    LOG(LL_DEBUG, ("Error retrieving the state of bSensor '%s' during the polling callback.",
      mgos_bthing_get_id(thing)));
  }
}

static void mg_bsensor_int_cb(int pin, void *sens) {
  if (sens) {
    struct mg_bsensor_cfg *cfg = MG_BSENSOR_CFG(sens);
    if (cfg->int_cfg.pin == pin) {
      cfg->int_cfg.triggered = 1;
      mgos_bthing_t thing = MGOS_BSENSOR_DOWNCAST((mgos_bsensor_t)sens);
      if (!mgos_bthing_get_state(thing)) {
        LOG(LL_DEBUG, ("Error retrieving the state of bSensor '%s' during the interrupt callback.",
          mgos_bthing_get_id(thing)));
      }
      cfg->int_cfg.triggered = 0;
    }
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
  if (!sensor) return false;
  if (poll_ticks > 0) {
    if (MG_BSENSOR_CFG(sensor)->int_cfg.pin == MGOS_BTHING_NO_PIN) {
      struct mg_bsensor_poll_cfg *cfg = &(MG_BSENSOR_CFG(sensor)->poll_cfg);
      if (cfg->timer_id == MGOS_INVALID_TIMER_ID) {
        cfg->timer_id = mgos_set_timer(poll_ticks, MGOS_TIMER_REPEAT, mg_bsensor_poll_cb, sensor);
        if (cfg->timer_id != MGOS_INVALID_TIMER_ID) {
          cfg->poll_ticks = poll_ticks;
          return true;
        } else {
          LOG(LL_ERROR, ("bSensor polling timer initialization failed."));
        }

      } else {
        LOG(LL_ERROR, ("bSensor polling mode already configured (every %d ms).", cfg->poll_ticks));
      }

    } else {
      LOG(LL_ERROR, ("The bSensor is in interrupt mode."));
    }

  } else {
    LOG(LL_ERROR, ("Invalid NULL bSensor instance or not allowed 'poll_ticks=(%d)' parameter.", poll_ticks));
  }

  LOG(LL_ERROR, ("Unable to set polling for bSensor '%s'. See previous error for more details.",
    mgos_bthing_get_id(MGOS_BSENSOR_DOWNCAST(sensor))));
  return false;
}

bool mgos_bsensor_interrupt_set(mgos_bsensor_t sensor, int pin,
                                enum mgos_gpio_pull_type pull_type,
                                enum mgos_gpio_int_mode int_mode,
                                int debounce) {
  if (!sensor) return false;
  #if MGOS_BTHING_HAVE_ACTUATORS
  if (mgos_bthing_is_typeof(MGOS_BSENSOR_DOWNCAST(sensor), MGOS_BTHING_TYPE_ACTUATOR)) {
    LOG(LL_ERROR, ("Interrupt mode cannot be activated for bSensor '%s' because it is 'MGOS_BTHING_TYPE_ACTUATOR'.",
      mgos_bthing_get_id(MGOS_BSENSOR_DOWNCAST(sensor))));
    return false;
  }
  #endif
  if (pin > MGOS_BTHING_NO_PIN && int_mode != MGOS_GPIO_INT_NONE) {
    if (MG_BSENSOR_CFG(sensor)->poll_cfg.poll_ticks == MGOS_BTHING_NO_TICKS) {
      struct mg_bsensor_int_cfg *cfg = &(MG_BSENSOR_CFG(sensor)->int_cfg);
      if (cfg->pin == MGOS_BTHING_NO_PIN) {
        if (mgos_gpio_set_button_handler(pin, pull_type, int_mode, debounce, mg_bsensor_int_cb, sensor)) {
          cfg->pin = pin;
          cfg->int_mode = int_mode;
          cfg->pull_type = pull_type;
          cfg->debounce = debounce;
          return true;
        } else {
          LOG(LL_ERROR, ("Error setting the interrupt handler on pin %d (pull %d, mode %d, debounce %d).",
            pin, pull_type, int_mode, debounce));
        }

      } else {
        LOG(LL_ERROR, ("bSensor interrupt mode already configured on pin %d.).", cfg->pin));
      }

    } else {
      LOG(LL_ERROR, ("The bSensor is in polling mode."));
    }

  } else {
    LOG(LL_ERROR, ("Invalid NULL bSensor instance or not allowed 'pin=%d' or 'int_mode=%d' parametes.",
      pin, int_mode));
  }

  LOG(LL_ERROR, ("Unable to set interrupt for bSensor '%s'. See previous error for more details.",
    mgos_bthing_get_id(MGOS_BSENSOR_DOWNCAST(sensor))));
  return false;
}

bool mgos_bsensor_init() {
  return true;
}