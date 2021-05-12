# bSensors Library
## Overview
*Coming soon...*

## Get Started
Include the library into your `mos.yml` file.
```yaml
libs:
  - origin: https://github.com/diy365-mgos/bsensor
```
**C/C++ firmware samples** - Copy and paste one of following exaples into your `main.c` project file.

Example #1: Create a *bSensor* for reading the system uptime.
```c
#include "mgos.h"
#include "mgos_bsensor.h"

static bool sensor_get_state_cb(mgos_bthing_t thing, mgos_bvar_t state, void *userdata) {
  mgos_bvar_set_decimal(state, mgos_uptime());
  return true;
}

static void sensor_state_updated_cb(int ev, void *ev_data, void *userdata) {
  mgos_bsensor_t sens = (mgos_bsensor_t)ev_data;
  mgos_bthing_t thing = MGOS_BSENSOR_DOWNCAST(sens);
  mgos_bvarc_t state = mgos_bthing_get_state(thing);

  LOG(LL_INFO, ("Sensor '%s' state changed: %f.",
    mgos_bthing_get_id(thing), mgos_bvar_get_decimal(state)));
}

enum mgos_app_init_result mgos_app_init(void) {

  mgos_event_add_handler(MGOS_EV_BTHING_STATE_UPDATED, sensor_state_updated_cb, NULL);

  /* create the sensor */
  mgos_bsensor_t s = mgos_bsensor_create("sens1", MGOS_BTHING_NOTIFY_STATE_ON_CHANGE);
  /* set the get-state hadnler */
  mgos_bthing_on_get_state(MGOS_BSENSOR_DOWNCAST(s), sensor_get_state_cb, NULL);
  /* set sensor read polling every 2 secs. */
  mgos_bsensor_polling_set(s, 2000);
  
  return MGOS_APP_INIT_SUCCESS;
}
```