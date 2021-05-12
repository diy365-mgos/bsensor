#include "mgos.h"
#include "mg_bsensor_sdk.h"


enum MG_BTHING_STATE_RESULT mg_bsensor_getting_state_cb(struct mg_bthing_sens *sens,
                                                        mgos_bvar_t state,
                                                        void *userdata) {
  if (sens && state) {
    struct mg_bsensor_cfg *cfg = MG_BSENSOR_CFG(sens);
    if (cfg->int_cfg.pin == MGOS_BTHING_NO_PIN || cfg->int_cfg.triggered == 1) {
      /* invoke the handler only if interrupt mode is OFF or if an interrupt has been triggerd */
      return cfg->base_class.getting_state_cb(sens, state, userdata);
    }
  }
  return MG_BTHING_STATE_RESULT_ERROR;
}

bool mg_bsensor_init(struct mg_bthing_sens *sens,
                    const char *id, int type, 
                    enum mgos_bthing_notify_state notify_state) {
  if (mg_bthing_sens_init(sens, id,  (type | MGOS_BSENSOR_TYPE), notify_state)) {
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
    LOG(LL_ERROR, ("Error creating bSensor '%s': unable to allocate memory for 'mg_bsensor_cfg'", id));
    mg_bthing_sens_reset(sens);
  }
  return false; 
}

void mg_bsensor_reset(struct mg_bthing_sens *sens) {
  struct mg_bsensor_cfg *cfg = MG_BSENSOR_CFG(sens);
  mg_bthing_on_getting_state(sens, cfg->base_class.getting_state_cb);
  cfg->base_class.getting_state_cb = NULL;
  
  mg_bthing_sens_reset(sens);
}
