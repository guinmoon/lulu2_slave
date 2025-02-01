///////////////////////////////////////////////////////////////////////////////
// pico_rtc_utils.cpp
//
// RTC utility functions for RP2040
//
// Sleep/wakeup scheme based on
// https://github.com/lyusupov/SoftRF/tree/master/software/firmware/source/libraries/RP2040_Sleep
// by Linar Yusupov
//
// Using code from pico-extras:
// https://github.com/raspberrypi/pico-extras/blob/master/src/rp2_common/pico_sleep/include/pico/sleep.h
// https://github.com/raspberrypi/pico-extras/blob/master/src/rp2_common/pico_sleep/sleep.c
// https://github.com/raspberrypi/pico-extras/blob/master/src/rp2_common/hardware_rosc/include/hardware/rosc.h
// https://github.com/raspberrypi/pico-extras/blob/master/src/rp2_common/hardware_rosc/rosc.c
//
// created: 10/2023
//
//
// MIT License
//
// Copyright (c) 2023 Matthias Prinke
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//
// History:
//
// 20231006 Created
// 20240905 Removed clocks_init() - not available/not required
//          in pico-sdk v2.0.0
//
// ToDo:
// -
//
///////////////////////////////////////////////////////////////////////////////
#if defined(ARDUINO_ARCH_RP2040)

#include "pico_rtc_utils.h"
#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "pico/runtime_init.h"

uint scb_orig = scb_hw->scr;
uint clock0_orig = clocks_hw->sleep_en0;
uint clock1_orig = clocks_hw->sleep_en1;

struct tm *datetime_to_tm(datetime_t *dt, struct tm *ti)
{
  ti->tm_sec = dt->sec;
  ti->tm_min = dt->min;
  ti->tm_hour = dt->hour;
  ti->tm_mday = dt->day;
  ti->tm_mon = dt->month - 1;
  ti->tm_year = dt->year - 1900;
  ti->tm_wday = dt->dotw;

  return ti;
}

datetime_t *tm_to_datetime(struct tm *ti, datetime_t *dt)
{
  dt->sec = ti->tm_sec;
  dt->min = ti->tm_min;
  dt->hour = ti->tm_hour;
  dt->day = ti->tm_mday;
  dt->month = ti->tm_mon + 1;
  dt->year = ti->tm_year + 1900;
  dt->dotw = ti->tm_wday;

  return dt;
}

void print_dt(datetime_t dt)
{
  log_i("%4d-%02d-%02d %02d:%02d:%02d", dt.year, dt.month, dt.day, dt.hour, dt.min, dt.sec);
}

void print_tm(struct tm ti)
{
  log_i("%4d-%02d-%02d %02d:%02d:%02d", ti.tm_year + 1900, ti.tm_mon + 1, ti.tm_mday, ti.tm_hour, ti.tm_min, ti.tm_sec);
}

time_t datetime_to_epoch(datetime_t *dt, time_t *epoch)
{
  struct tm ti;
  datetime_to_tm(dt, &ti);

  // Apply daylight saving time according to timezone and date
  ti.tm_isdst = -1;

  // Convert to epoch
  time_t _epoch = mktime(&ti);

  if (epoch)
  {
    *epoch = _epoch;
  }

  return _epoch;
}

datetime_t *epoch_to_datetime(time_t *epoch, datetime_t *dt)
{
  struct tm ti;

  // Apply daylight saving time according to timezone and date
  ti.tm_isdst = -1;

  // Convert epoch to struct tm
  localtime_r(epoch, &ti);

  // Convert struct tm to datetime_t
  tm_to_datetime(&ti, dt);

  return dt;
}

void recover_from_sleep(uint scb_orig, uint clock0_orig, uint clock1_orig)
{

  // Re-enable ring Oscillator control
  rosc_write(&rosc_hw->ctrl, ROSC_CTRL_ENABLE_BITS);

  // // reset procs back to default
  scb_hw->scr = scb_orig;
  clocks_hw->sleep_en0 = clock0_orig;
  clocks_hw->sleep_en1 = clock1_orig;
  // //востанавливает USB 
  user_irq_unclaim(31);
  // //Восстанавливает частоты
  runtime_init_clocks();
  rosc_enable();
  // // return USB pll to 48mhz
  pll_init(pll_usb, 1, 1440000000, 6, 5);
  //  clocks_init();

  // reset clocks
  //  clocks_init();
  //  stdio_init_all();

  return;
}

static void sleep_callback(void)
{
  // rp2040.restart();
  recover_from_sleep(scb_orig, clock0_orig, clock1_orig);
  Serial.begin(115200);
  Serial.print("RTC woke us up\n");
  // awake = true;
}

datetime_t t = {
    .year = 2020,
    .month = 06,
    .day = 05,
    .dotw = 5, // 0 is Sunday, so 5 is Friday
    .hour = 15,
    .min = 45,
    .sec = 00};

// Alarm 10 seconds later
datetime_t t_alarm = {
    .year = 2020,
    .month = 06,
    .day = 05,
    .dotw = 5, // 0 is Sunday, so 5 is Friday
    .hour = 15,
    .min = 45,
    .sec = 5};

// Sleep for <duration> seconds
void pico_sleep(unsigned duration)
{
  // datetime_t dt;
  // rtc_get_datetime(&dt);
  // Serial.print("RTC time:");
  // print_dt(dt);

  // time_t now;
  // datetime_to_epoch(&dt, &now);

  // // Add sleep_duration
  // time_t wakeup = now + duration;

  // epoch_to_datetime(&wakeup, &dt);

  // log_i("Wakeup time:");
  // print_dt(dt);
  // gpio_put(16, 1);
  rtc_init();
  rtc_set_datetime(&t);
  Serial.flush();
  Serial1.end();
  Serial2.end();
  Serial.end();

// From
// https://github.com/lyusupov/SoftRF/tree/master/software/firmware/source/libraries/RP2040_Sleep
// also see src/pico_rtc
// --8<-----
#if defined(USE_TINYUSB)
  // Disable USB
  USBDevice.detach();
#endif /* USE_TINYUSB */

  sleep_run_from_xosc();

  sleep_goto_sleep_until(&t_alarm, sleep_callback);

  // back from dormant state
  // recover_from_sleep(scb_orig, clock0_orig, clock1_orig);
  // rosc_enable();

  // pll_init(pll_usb, 1, 1440000000, 6, 5); // return USB pll to 48mhz
  // tusb_init();
  // --8<-----
}
#endif
