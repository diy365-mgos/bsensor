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

#ifndef MGOS_BSENSOR_SDK_H_
#define MGOS_BSENSOR_SDK_H_

#include <stdbool.h>
#include "mgos_bthing_sdk.h"
#include "mgos_bsensor.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MG_BENSOR_NEW(s) MG_BTHING_SENS_NEW(s);

#define MG_BENSOR_BASE_CAST(s) ((struct mg_bthing_sens *)s)

#define MG_BENSOR_GET_CFG(s) ((struct mg_bsensor_cfg *)MG_BENSOR_BASE_CAST(s)->cfg)

struct mg_bsensor_int_cfg {
  int pin;
  enum mgos_gpio_pull_type pull_type;
  enum mgos_gpio_int_mode int_mode;
  int debounce;
};

struct mg_bsensor_poll_cfg {
  int poll_ticks;
  int timer_id;
};

struct mg_bsensor_cfg {
  struct mg_bsensor_poll_cfg poll_cfg;
  struct mg_bsensor_int_cfg int_cfg;
};

bool mg_bsensor_init(struct mg_bthing_sens *sens,
                    const char *id, int type, 
                    enum mgos_bthing_notify_state notify_state);

void mg_bsensor_reset(struct mg_bthing_sens *sens);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MGOS_BSENSOR_SDK_H_ */