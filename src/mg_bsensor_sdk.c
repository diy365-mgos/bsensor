#include "mgos.h"
#include "mg_bsensor_sdk.h"

bool mg_bsensor_init(struct mg_bthing_sens *sens,
                    const char *id, int type, 
                    enum mgos_bthing_notify_state notify_state) {
  if (mg_bthing_sens_init(sens, id,  (type | MGOS_BSENSOR_TYPE), notify_state)) {
    sens->cfg = calloc(1, sizeof(struct mg_bsensor_cfg));
    if (sens->cfg) {
      MG_BSENSOR_CFG(sens)->poll_cfg.poll_ticks = MGOS_BTHING_NO_TICKS;
      MG_BSENSOR_CFG(sens)->poll_cfg.timer_id = MGOS_INVALID_TIMER_ID;
      MG_BSENSOR_CFG(sens)->int_cfg.pin = MGOS_BTHING_NO_PIN;
      return true;
    }
    mg_bthing_sens_reset(sens);
  }
  return false; 
}

void mg_bsensor_reset(struct mg_bthing_sens *sens) {
  mg_bthing_sens_reset(sens);
}
