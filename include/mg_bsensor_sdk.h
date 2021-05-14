/*
 * Copyright (c) 2021 DIY356
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MG_BSENSOR_SDK_H_
#define MG_BSENSOR_SDK_H_

#include <stdbool.h>
#include "mg_bthing_sdk.h"
#include "mgos_bsensor.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MG_BSENSOR_NEW(s) MG_BTHING_SENS_NEW(s);

/*****************************************
 * Cast Functions
 */

// Convert (mgos_bsensor_t) into (struct mg_bthing_sens *)
struct mg_bthing_sens *MG_BSENSOR_CAST1(mgos_bsensor_t thing);
/*****************************************/

#define MG_BSENSOR_CFG(s) ((struct mg_bsensor_cfg *)MG_BSENSOR_CAST1(s)->cfg)

struct mg_bsensor_int_cfg {
  int pin;
  enum mgos_gpio_pull_type pull_type;
  enum mgos_gpio_int_mode int_mode;
  int debounce;
  unsigned char triggered;
};

struct mg_bsensor_poll_cfg {
  int poll_ticks;
  int timer_id;
};

struct mg_bsensor_base_class {
  mg_bthing_getting_state_handler_t getting_state_cb;
};

struct mg_bsensor_cfg {
  struct mg_bsensor_poll_cfg poll_cfg;
  struct mg_bsensor_int_cfg int_cfg;
  struct mg_bsensor_base_class base_class; 
};

bool mg_bsensor_init(struct mg_bthing_sens *sens);

void mg_bsensor_reset(struct mg_bthing_sens *sens);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MG_BSENSOR_SDK_H_ */