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

#ifndef MGOS_BSENSOR_H_
#define MGOS_BSENSOR_H_

#include <stdbool.h>
#include "mgos_bthing.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MGOS_BSENSOR_TYPE 4

struct mg_bthing_sens;
typedef struct mg_bthing_sens *mgos_bsensor_t;

inline mgos_bthing_t MGOS_BSENSOR_DOWNCAST(mgos_bsensor_t sensor) { return (mgos_bthing_t)sensor; }

mgos_bsensor_t mgos_bsensor_create(const char *id, enum mgos_bthing_notify_state notify_state);

bool mgos_bsensor_polling_set(mgos_bsensor_t sensor, int poll_ticks);

bool mgos_bsensor_interrupt_set(mgos_bsensor_t sensor, int pin,
                                enum mgos_gpio_pull_type pull_type,
                                enum mgos_gpio_int_mode int_mode,
                                int debounce);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MGOS_BSENSOR_H_ */