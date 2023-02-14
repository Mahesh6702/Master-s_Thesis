#include "Portenta_H7_TimerInterrupt.h"
#include "Portenta_H7_ISR_Timer.h"


#define TIMER_INTERVAL_MS         100
#define HW_TIMER_INTERVAL_MS      50

Portenta_H7_Timer ITimer(TIM12);  // Init timer TIM12

// Init Portenta_H7_ISR_Timer
// Each Portenta_H7_ISR_Timer can service 16 different ISR-based timers
Portenta_H7_ISR_Timer ISR_Timer;

#define TIMER_INTERVAL_0_5S           500L
#define TIMER_INTERVAL_1S             1000L
#define TIMER_INTERVAL_1_5S           1500L
int value;


void TimerHandler()
{
  ISR_Timer.run();
}

void clockfrequency()
{
  value =HAL_RCC_GetSysClockFreq() / 1000000;
  Serial.print("Clock Frequency   "); Serial.print(value); Serial.println(" MHz");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(96000);  
  // You can use up to 16 timer for each ISR_Timer
  ISR_Timer.setInterval(TIMER_INTERVAL_0_5S,  clockfrequency);

}

void loop() {
  // put your main code here, to run repeatedly:


}
