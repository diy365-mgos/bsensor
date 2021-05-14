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

enum MG_BTHING_STATE_RESULT mg_bsensor_getting_state_cb(struct mg_bthing_sens *sens,
                                                        mgos_bvar_t state,
                                                        void *userdata) {
  if (sens && state) {
    struct mg_bsensor_cfg *cfg = MG_BSENSOR_CFG(sens);
    if (cfg->int_cfg.pin == MGOS_BTHING_NO_PIN || cfg->int_cfg.triggered == 1) {
      /* invoke the handler only if interrupt mode is OFF or if an interrupt has been triggerd */
      return cfg->base_class.getting_state_cb(sens, state, userdata);
    }
    LOG(LL_DEBUG, ("Getting state for bSensor '%s' when interrupt mode is active is not allowed.",
      mgos_bthing_get_id(MG_BTHING_SENS_CAST4(sens))));
  }
  return MG_BTHING_STATE_RESULT_ERROR;
}

bool mg_bsensor_init(struct mg_bthing_sens *sens) {
  if (mg_bthing_sens_init(sens)) {
    struct mg_bsensor_cfg *cfg = sens->cfg = calloc(1, sizeof(struct mg_bsensor_cfg));
    if (sens->cfg) {
      /* initalize polling cfg */
      cfg->poll_cfg.poll_ticks = MGOS_BTHING_NO_TICKS;
      cfg->poll_cfg.timer_id = MGOS_INVALID_TIMER_ID;
      /* initalize inerrupt cfg */
      cfg->int_cfg.pin = MGOS_BTHING_NO_PIN;
      /* initalize base-class cfg */
      cfg->base_class.getting_state_cb = mg_bthing_on_getting_state(sens, mg_bsensor_getting_state_cb);
      return true;
    }
    LOG(LL_ERROR, ("Error creating bSensor '%s': unable to allocate memory for 'mg_bsensor_cfg'",
      MG_BTHING_SENS_CAST3(sens)->id));
    mg_bsensor_reset(sens);
  }
  return false; 
}

void mg_bsensor_reset(struct mg_bthing_sens *sens) {
  struct mg_bsensor_cfg *cfg = MG_BSENSOR_CFG(sens);
  if (cfg->base_class.getting_state_cb) {
    mg_bthing_on_getting_state(sens, cfg->base_class.getting_state_cb);
    cfg->base_class.getting_state_cb = NULL;
  }
  mg_bthing_sens_reset(sens);
}
