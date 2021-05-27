#include "mgos.h"
#include "mg_bsensor_sdk.h"


/*****************************************
 * Cast Functions
 */

// Convert (mgos_bsensor_t) into (struct mg_bthing_sens *)
struct mg_bthing_sens *MG_BSENSOR_CAST1(mgos_bsensor_t thing) {
  return (struct mg_bthing_sens *)thing;
}
/*****************************************/

bool mg_bsensor_init(mgos_bsensor_t sens, struct mg_bsensor_cfg *cfg) {
  if (cfg) {
    if (mg_bthing_sens_init(sens, cfg)) {
      /* initalize polling cfg */
      cfg->poll_cfg.poll_ticks = MGOS_BTHING_NO_TICKS;
      cfg->poll_cfg.timer_id = MGOS_INVALID_TIMER_ID;
      /* initalize inerrupt cfg */
      cfg->int_cfg.pin = MGOS_BTHING_NO_PIN;
      /* initalize overrides cfg */
      cfg->overrides.getting_state_cb = NULL;

      return true;
    }
    mg_bsensor_reset(sens);
  } else {
    LOG(LL_ERROR, ("Invalid NULL 'cfg' parameter."));
  }

  LOG(LL_ERROR, ("Error initializing bSensor '%s'. See above error message for more details.'", 
    mgos_bthing_get_id(MGOS_BSENSOR_THINGCAST(sens))));
  return false; 
}

void mg_bsensor_reset(mgos_bsensor_t sens) {
  struct mg_bsensor_cfg *cfg = MG_BSENSOR_CFG(sens);

  /* clear overrides cfg */
  if (cfg->overrides.getting_state_cb) {
    mg_bthing_on_getting_state(sens, cfg->overrides.getting_state_cb);
    cfg->overrides.getting_state_cb = NULL;
  }
  /* clear polling cfg */
  cfg->poll_cfg.poll_ticks = MGOS_BTHING_NO_TICKS;
  cfg->poll_cfg.timer_id = MGOS_INVALID_TIMER_ID;
  /* clear inerrupt cfg */
  cfg->int_cfg.pin = MGOS_BTHING_NO_PIN;

  // reset sensor-base object
  mg_bthing_sens_reset(MG_BSENSOR_CAST1(sens));
}

bool mg_bsensor_update_state(mgos_bsensor_t sens) {
  return (mgos_bthing_get_state(MGOS_BSENSOR_THINGCAST(sens)) != NULL);
}