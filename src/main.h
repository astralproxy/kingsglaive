#pragma once

#include <pebble.h>


// CONSTS
#define SCREEN_WIDTH          144
#define SCREEN_HEIGHT         168
#define SCREEN_CENTER_X       72
#define SCREEN_CENTER_Y       84

#define FONT_SIZE             40
#define FONT_SIZE_SM          24
#define FONT_SIZE_TINY        16

#define TEXT_X1               -FONT_SIZE
#define TEXT_X2               (FONT_SIZE * 0.25 + 4)
#define TEXT_X3               (FONT_SIZE + FONT_SIZE_SM / 2)
#define TEXT_XD               (-FONT_SIZE * 0.25)
#define TIME_TEXT_OFFSET      4
#define SMALL_TEXT_OFFSET     7
#define TOP_TEXT_Y            (SCREEN_CENTER_Y - FONT_SIZE - TIME_TEXT_OFFSET)
#define BOTTOM_TEXT_Y         (SCREEN_CENTER_Y + TIME_TEXT_OFFSET)
#define DIV_Y                 (BOTTOM_TEXT_Y - (FONT_SIZE_TINY / 2))

#define BATTERY_BAR_OFFSET    10
#define BATTERY_BAR_HEIGHT    10
#define BATTERY_PERCENT_WIDTH (SCREEN_WIDTH / 3)
#define BATTERY_X             (BATTERY_PERCENT_WIDTH*2 - BATTERY_BAR_OFFSET)

#define CIRCLE_COUNT          5
#define CIRCLES_Y             (SCREEN_HEIGHT - BATTERY_BAR_OFFSET)
#define CIRCLE_RADIUS         4
#define CIRCLE_OFFSET         (CIRCLE_RADIUS + 4)

    
Window* window;
Layer* canvas;
AppTimer* timer;

GFont mTimeFont;
GFont mTimeFontBold;
GFont mSmallFont;
GFont mTinyFont;

char* mDayText;
char* mMonthText;
char* mYearText;
char* mHourText;
char* mMinuteText;
char* mSecondText;
char* mAltText;

int mChargePercent = 100;

bool mIsCharging = false;
bool mTimeOnTop = true;