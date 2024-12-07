/*
 * rtc.cpp
 *
 *  Created on: Jun 15, 2024
 *      Author: Youssef
 */
#include <Base_Periph/Rtc.hpp>
#include <cmath>
#include <cstdint>
#include "main.hpp"


Rtc::Rtc() {
	init();
    // Constructor implementation
}

Rtc::~Rtc() {
    // Destructor implementation
    deinit();
}

bool Rtc::init() {

	hrtcP.Instance = RTC;
	hrtcP.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtcP.Init.AsynchPrediv = 127;
	hrtcP.Init.SynchPrediv = 255;
	hrtcP.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtcP.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtcP.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	if (HAL_RTC_Init(&hrtcP) != HAL_OK)
	{
		//Error_Handler();
		return false;

		    }
	return true;
}

bool Rtc::deinit() {
    if (HAL_RTC_DeInit(&hrtcP) != HAL_OK) {
        return false;
    }
    return true;
}

/** Initialize RTC and set the Time and Date
  */
uint8_t Rtc::DECtoBCD(uint8_t dec) {
    return ((dec / 10) << 4) | (dec % 10);
}

uint8_t Rtc::BCDtoDEC(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

bool Rtc::setTime(TimeInfo *T){
	sTime.Hours = DECtoBCD(T->datetime.tm_hour);
	sTime.Minutes = DECtoBCD(T->datetime.tm_min);
	sTime.Seconds = DECtoBCD(T->datetime.tm_sec);
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtcP, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
	  return false;
  }
  	sDate.WeekDay = DECtoBCD(T->datetime.tm_wday);
  	sDate.Month = DECtoBCD(T->datetime.tm_mon);
  	sDate.Date = DECtoBCD(T->datetime.tm_mday);
  	sDate.Year = DECtoBCD(T->datetime.tm_year);
  if (HAL_RTC_SetDate(&hrtcP, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
	  return false;
  }
  return true;
}

TimeInfo Rtc::getTime() {
    TimeInfo currTime;

    HAL_RTC_GetTime(&hrtcP, &sTime, RTC_FORMAT_BCD);
    HAL_RTC_GetDate(&hrtcP, &sDate, RTC_FORMAT_BCD);

    currTime.datetime.tm_year = BCDtoDEC(sDate.Year);
    currTime.datetime.tm_mday = BCDtoDEC(sDate.Date);
    currTime.datetime.tm_mon = BCDtoDEC(sDate.Month);

    currTime.datetime.tm_hour = BCDtoDEC(sTime.Hours);
    currTime.datetime.tm_min = BCDtoDEC(sTime.Minutes);
    currTime.datetime.tm_sec = BCDtoDEC(sTime.Seconds);
    currTime.s_fraction = 0.0;

    return currTime;
}

double Rtc::getTime_u() {
    TimeInfo currTime;

    HAL_RTC_GetTime(&hrtcP, &sTime, RTC_FORMAT_BCD);
    HAL_RTC_GetDate(&hrtcP, &sDate, RTC_FORMAT_BCD);

    currTime.datetime.tm_year = BCDtoDEC(sDate.Year) + 100;
    currTime.datetime.tm_mday = BCDtoDEC(sDate.Date);
    currTime.datetime.tm_mon = BCDtoDEC(sDate.Month) -1;

    currTime.datetime.tm_hour = BCDtoDEC(sTime.Hours);
    currTime.datetime.tm_min = BCDtoDEC(sTime.Minutes);
    currTime.datetime.tm_sec = BCDtoDEC(sTime.Seconds);
    currTime.s_fraction = 0.0;
    return static_cast<double>(mktime(&currTime.datetime));

}
