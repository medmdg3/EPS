/*
 * rtc.hpp
 *
 *  Created on: Jun 15, 2024
 *      Author: Youssef
 */

#ifndef INC_BASE_PERIPH_RTC_HPP_
#define INC_BASE_PERIPH_RTC_HPP_

extern "C" {
#include "stm32f3xx_hal.h"
}
#include <cmath>
#include <ctime>

typedef struct TimeInfo_s {
    struct tm datetime;
    double s_fraction;
} TimeInfo;



class Rtc{
public:
    Rtc();
    Rtc(Rtc &&) = delete;
    Rtc(const Rtc &) = delete;
    Rtc &operator=(Rtc &&) = delete;
    Rtc &operator=(const Rtc &) = delete;
    virtual ~Rtc();

    bool init();
    virtual bool deinit();
    TimeInfo getTime() ;
    bool setTime(TimeInfo *timeInfo);
    /**
	 * @brief Get the time.
	 *
	 * @return double the time in seconds since the epoch.
	 */
    double getTime_u();


private:

    uint8_t DECtoBCD(uint8_t dec);
    uint8_t BCDtoDEC(uint8_t bcd);
    RTC_HandleTypeDef hrtcP;
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};
};

#endif /* INC_BASE_PERIPH_RTC_HPP_ */
