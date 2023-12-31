#ifndef WS2812_NEW_H
#define WS2812_NEW_H

/*
 * Copyright (c) 2023 Erik Bosman <erik@minemu.org>
 *
 * Permission  is  hereby  granted,  free  of  charge,  to  any  person
 * obtaining  a copy  of  this  software  and  associated documentation
 * files (the "Software"),  to deal in the Software without restriction,
 * including  without  limitation  the  rights  to  use,  copy,  modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the
 * Software,  and to permit persons to whom the Software is furnished to
 * do so, subject to the following conditions:
 *
 * The  above  copyright  notice  and this  permission  notice  shall be
 * included  in  all  copies  or  substantial portions  of the Software.
 *
 * THE SOFTWARE  IS  PROVIDED  "AS IS", WITHOUT WARRANTY  OF ANY KIND,
 * EXPRESS OR IMPLIED,  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY,  FITNESS  FOR  A  PARTICULAR  PURPOSE  AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM,  DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT,  TORT OR OTHERWISE,  ARISING FROM, OUT OF OR IN
 * CONNECTION  WITH THE SOFTWARE  OR THE USE  OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * (http://opensource.org/licenses/mit-license.html)
 *
 */

#include "config.h"

/* static constants */

#define MASK_LED_DATA (PIN_LED_DATA)

/* derived constants */

#define N_VALUES_PER_STRIP (N_LEDS_PER_STRIP*N_VALUES_PER_LED)
#define N_LEDS (N_LEDS_PER_STRIP*N_STRIPS)
#define N_VALUES (N_VALUES_PER_STRIP*N_STRIPS)
#define N_BITS_PER_STRIP (N_VALUES_PER_STRIP*8)

#if F_CPU == 48000000

#define T0H           (13)
#define T1H           (31)
#define T_PULSE       (44)
#define T_LATCH       (13440)

#else
#error "timings not specified for CPU speed"
#endif

#define FRAME_CYCLES (T_PULSE*N_BITS_PER_STRIP)
#define SYSTICK_DIV (F_CPU/F_SYS_TICK_CLK)
#define SYSTICK_PERIOD ( (FRAME_CYCLES+SYSTICK_DIV-1)/SYSTICK_DIV )

#ifndef __ASSEMBLER__

#include <stdint.h>
#include <stddef.h>

typedef struct
{
	uint16_t data[N_VALUES];

} frame_t;

void ws2812_dma_start(volatile uint16_t buf[], uint32_t length);
void ws2812_half_transfer(void);
void ws2812_full_transfer(void);
void ws2812_dma_init(uintptr_t gpio, uint16_t mask, int t0h, int t1h, int t_pulse);


void ws2812_init(void);
void ws2812_swap_frame(void);
/* returns next frame when ready, NULL otherwise */
frame_t *ws2812_get_frame(void);
void ws2812_write(void);

#endif

#endif // WS2812_NEW_H
