/****************************************************************************
 * arch/sparc/src/sparc_v8/sparc_v8_copystate.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <arch/irq.h>

#include "sparc_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sparc_copystate
 ****************************************************************************/

/* Really just a memcpy */

void sparc_copystate(uint32_t *dest, uint32_t *src)
{
  int i;

  /* The state is copied from the stack to the TCB, but only a reference is
   * passed to get the state from the TCB.  So the following check avoids
   * copying the TCB save area onto itself:
   */

  if (src != dest)
    {
      for (i = 0; i < XCPTCONTEXT_REGS; i++)
        {
          *dest++ = *src++;
        }
    }
}

void task_flush_trap(uint32_t *trap, uint32_t *task)
{
  up_set_current_regs(task);
}

void trap_flush_task(uint32_t *task, uint32_t *trap)
{
  uint32_t psr;
  int i;

  if (task != trap)
    {
      for (i = 0; i < XCPTCONTEXT_REGS; i++)
        {
          task[i] = trap[i];
        }
    }

  psr = trap[REG_PSR];

#ifdef CONFIG_SUPPRESS_INTERRUPTS
  task[REG_PSR] = psr & ~SPARC_PSR_ET_MASK;
#else
  task[REG_PSR] = psr | SPARC_PSR_ET_MASK;
#endif
}

