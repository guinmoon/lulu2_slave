
#include "pico/multicore.h"
#include <Wire.h>
#include "commands.h"
#include "global_def.h"
#include "servos.h"
#include "character.h"
#include "pico/stdlib.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
// #include <stdlib.h>
#include "hardware/clocks.h"
#include "pico_rtc_utils.h"
#include <hardware/rtc.h>
#include "hardware/structs/scb.h"
// #include <ESP32Time.h>
// #include <TaskScheduler.h>

#define SLEEP_INTERVAL 5
// bool runtimeExpired; //!< flag indicating if runtime has expired at least once
// bool longSleep;      //!< last sleep interval; 0 - normal / 1 - long
// time_t rtcLastClockSync;
// bool rtcSyncReq = false;
// ESP32Time rtc;
// Scheduler runner;


void sleepInit()
{

    // rtc_init();

    // Restore variables and RTC after reset
    // time_t time_saved = watchdog_hw->scratch[0];
    // datetime_t dt;
    // epoch_to_datetime(&time_saved, &dt);

    // // Set HW clock (only used in sleep mode)
    // rtc_set_datetime(&dt);

    // // Set SW clock
    // rtc.setTime(time_saved);

    // runtimeExpired = ((watchdog_hw->scratch[1] & 1) == 1);
    // longSleep = ((watchdog_hw->scratch[1] & 2) == 2);
    // rtcLastClockSync = watchdog_hw->scratch[2];
}


/// Determine sleep duration and enter Deep Sleep Mode
void prepareSleep(void)
{
    unsigned long sleep_interval = SLEEP_INTERVAL;

    // If the real time is available, align the wake-up time to the
    // next non-fractional multiple of sleep_interval past the hour
    // if (rtcLastClockSync)
    // {
    //     struct tm timeinfo;
    //     time_t t_now = rtc.getLocalEpoch();
    //     localtime_r(&t_now, &timeinfo);
    //     sleep_interval = sleep_interval - ((timeinfo.tm_min * 60) % sleep_interval + timeinfo.tm_sec);
    //     if (sleep_interval < 60)
    //     {
    //         sleep_interval = 60;
    //     }
    // }
    // rtc_init();
    // rtc_set_datetime(&t);
    Serial.printf("Shutdown() - sleeping for %lu s\n", sleep_interval);

    // Set HW clock from SW clock
    // time_t t_now = rtc.getLocalEpoch();
    // datetime_t dt;
    // epoch_to_datetime(&t_now, &dt);
    // rtc_set_datetime(&dt);
    sleep_us(64);
    pico_sleep(sleep_interval);
    // recover_from_sleep(scb_orig, clock0_orig, clock1_orig);
    Serial.println("Awake");
    // rp2040.restart();
}

void dogInit()
{
    // setTailSpeed(4);
    layDown(3);
    waitForServoPos(SER_LEFT_FRONT, SERVO_180, 1000);
    delay(1000);
    stand(4);
    waitForServoPos(SER_LEFT_FRONT, SERVO_90, 1000);
    // leftHand(4);
    // sitDown(5);
    // delay(1000);
    // stand(2);
    // delay(1000);
    // dance1(5);
    // halfLayDown(4);    
    // leftHand(4,false);
    delay(2000);
    // setTailSpeed(0);
    // delay(1000);
    // happy(4);
}

void setup()
{

    // Настройка сервоприводов
    initServos();
    multicore_launch_core1(core1_update_servos);

    dogInit();
    // stepForward(4,3);
    // tailLegsStand(4);
    // fullLayDown(4);
    // delay(2000);
    
    // layDown(4);
    // leftHand(5);
    // Настройка I2C
    // Serial.println("Go to sleep after 5 sec");
    // delay(5000);
    // prepareSleep();
    Wire.begin(8);
    Wire.onReceive(receiveEvent);
}

void loop()
{
    // Основной петля на первом ядре может быть использована для других задач
    // Например, для получения и обработки данных через I2C

    // setTargetPosAndSpeed(SER_RIGHT_FRONT,90,5);
    // setTargetPosAndSpeed(SER_RIGHT_BACK,90,5);
    // setTargetPosAndSpeed(SER_LEFT_FRONT,90,5);
    // setTargetPosAndSpeed(SER_LEFT_BACK,90,5);
    //

    // setTargetPosAndSpeed(SER_TAIL,90,5);
    // setTailSpeed(4);
    Serial.println("loop");
    delay(1000);
}
