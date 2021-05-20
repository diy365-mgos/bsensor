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

/* enum MG_BTHING_STATE_RESULT mg_bsensor_getting_state_cb(struct mg_bthing_sens *sens,
                                                        mgos_bvar_t state,
                                                        void *userdata) {
  if (sens && state) {
    struct mg_bsensor_cfg *cfg = MG_BSENSOR_CFG(sens);
    if (cfg->int_cfg.pin == MGOS_BTHING_NO_PIN || cfg->int_cfg.triggered == 1) {
      // invoke the handler only if interrupt mode is OFF or if an interrupt has been triggerd
      return cfg->overrides.getting_state_cb(sens, state, userdata);
    }
    LOG(LL_DEBUG, ("Getting state for bSensor '%s' when interrupt mode is active is not allowed.",
      mgos_bthing_get_id(MG_BTHING_SENS_CAST4(sens))));
  }
  return MG_BTHING_STATE_RESULT_ERROR;
} */

bool mg_bsensor_init(mgos_bsensor_t sens) {
  if (mg_bthing_sens_init(sens)) {
    struct mg_bsensor_cfg *cfg = sens->cfg = calloc(1, sizeof(struct mg_bsensor_cfg));
    if (sens->cfg) {
      /* initalize polling cfg */
      cfg->poll_cfg.poll_ticks = MGOS_BTHING_NO_TICKS;
      cfg->poll_cfg.timer_id = MGOS_INVALID_TIMER_ID;
      /* initalize inerrupt cfg */
      cfg->int_cfg.pin = MGOS_BTHING_NO_PIN;
      /* initalize base-class cfg */
      cfg->overrides.getting_state_cb = NULL;
      //cfg->overrides.getting_state_cb = mg_bthing_on_getting_state(sens, mg_bsensor_getting_state_cb);
      return true;
    }
    LOG(LL_ERROR, ("Unable to allocate memory for 'mg_bsensor_cfg'"));
    mg_bsensor_reset(sens);
  }
  LOG(LL_ERROR, ("Error creating bSensor '%s'. See above error message for more details.'", 
    mgos_bthing_get_id(MGOS_BSENSOR_THINGCAST(sens))));
  return false; 
}

void mg_bsensor_reset(mgos_bsensor_t sens) {
  struct mg_bsensor_cfg *cfg = MG_BSENSOR_CFG(sens);

  /* clear base-class cfg */
  if (cfg->overrides.getting_state_cb) {
    mg_bthing_on_getting_state(sens, cfg->overrides.getting_state_cb);
    cfg->overrides.getting_state_cb = NULL;
  }
  /* clear polling cfg */
  cfg->poll_cfg.poll_ticks = MGOS_BTHING_NO_TICKS;
  cfg->poll_cfg.timer_id = MGOS_INVALID_TIMER_ID;
  /* clear inerrupt cfg */
  cfg->int_cfg.pin = MGOS_BTHING_NO_PIN;

  mg_bthing_sens_reset(MG_BSENSOR_CAST1(sens));
}
