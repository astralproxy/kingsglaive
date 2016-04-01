#include "main.h"


static void render(Layer *layer, GContext* ctx) 
{
    if (mAltText == NULL)
    {
        // Set colors
        graphics_context_set_text_color(ctx, GColorWhite);
        graphics_context_set_stroke_color(ctx, GColorWhite);
        graphics_context_set_fill_color(ctx, GColorWhite);
     
        APP_LOG(APP_LOG_LEVEL_INFO, "draw date and time");
        
        int timeTextY = 0;
        int dateTextY = 0;
        if (mTimeOnTop)
        {
            timeTextY = TOP_TEXT_Y;
            dateTextY = BOTTOM_TEXT_Y;
        }
        else
        {
            dateTextY = TOP_TEXT_Y;
            timeTextY = BOTTOM_TEXT_Y;
        }

        // Draw the dividing line
        graphics_draw_text(ctx, "--------------------", mTinyFont, GRect(0, DIV_Y, SCREEN_WIDTH, FONT_SIZE_SM),
                           GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);

        graphics_draw_text(ctx, mHourText, mTimeFontBold, GRect(TEXT_X1, timeTextY, SCREEN_WIDTH, FONT_SIZE),
                           GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        graphics_draw_text(ctx, ":", mTimeFont, GRect(TEXT_XD, timeTextY, SCREEN_WIDTH, FONT_SIZE),
                           GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        graphics_draw_text(ctx, mMinuteText, mTimeFont, GRect(TEXT_X2, timeTextY, SCREEN_WIDTH, FONT_SIZE),
                           GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        graphics_draw_text(ctx, mSecondText, mSmallFont, GRect(TEXT_X3, timeTextY+SMALL_TEXT_OFFSET, SCREEN_WIDTH, FONT_SIZE_SM),
                           GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);

        graphics_draw_text(ctx, mMonthText, mTimeFontBold, GRect(TEXT_X1, dateTextY, SCREEN_WIDTH, FONT_SIZE),
                           GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        graphics_draw_text(ctx, ".", mTimeFontBold, GRect(TEXT_XD, dateTextY, SCREEN_WIDTH, FONT_SIZE),
                           GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        graphics_draw_text(ctx, mDayText, mTimeFont, GRect(TEXT_X2, dateTextY, SCREEN_WIDTH, FONT_SIZE),
                           GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        graphics_draw_text(ctx, mYearText, mSmallFont, GRect(TEXT_X3, dateTextY+SMALL_TEXT_OFFSET, SCREEN_WIDTH, FONT_SIZE_SM),
                           GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);

        // Display the current battery state.
        const int batteryWidthMod = (float)mChargePercent / 100.0f * (float)BATTERY_PERCENT_WIDTH;
        APP_LOG(APP_LOG_LEVEL_INFO, "draw battery - charge: %d, width (%d/%d)", mChargePercent, batteryWidthMod, BATTERY_PERCENT_WIDTH);
        GRect barRect = GRect(BATTERY_X, BATTERY_BAR_OFFSET, BATTERY_PERCENT_WIDTH, BATTERY_BAR_HEIGHT);
        graphics_draw_rect(ctx, barRect);
        barRect = GRect(BATTERY_X, BATTERY_BAR_OFFSET, batteryWidthMod, BATTERY_BAR_HEIGHT);
        graphics_fill_rect(ctx, barRect, 0, GCornerNone);
        
        // Draw circles
        const int circleStartX = SCREEN_WIDTH / 2 - ((CIRCLE_RADIUS+CIRCLE_OFFSET) * CIRCLE_COUNT / 2);
        for (int x=0; x < CIRCLE_COUNT; ++x)
        {
            const int circleX = circleStartX + (x * (CIRCLE_RADIUS+CIRCLE_OFFSET));
            
            if (x==0)
            {
                // Fill the first circle
                graphics_fill_circle(ctx, GPoint(circleX, CIRCLES_Y), CIRCLE_RADIUS);
            }
            else
            {
                graphics_draw_circle(ctx, GPoint(circleX, CIRCLES_Y), CIRCLE_RADIUS);
            }
        }
    }
    else
    {
        // Draw alt screen
    }
}

static void set_time_display(struct tm *t)
{
    const int mTwoDigitTextSize = sizeof("00:00 ");
    
    // Time strings from here:
    // http://www.cplusplus.com/reference/ctime/strftime/

    strftime(mDayText, mTwoDigitTextSize, "%d", t); //day
    strftime(mMonthText, mTwoDigitTextSize, "%m", t); //month
    strftime(mYearText, mTwoDigitTextSize, "%y", t); //year
    strftime(mHourText, mTwoDigitTextSize, "%I", t); //hour (24 hr format: %H)
    strftime(mMinuteText, mTwoDigitTextSize, "%M", t); //minute
    strftime(mSecondText, mTwoDigitTextSize, "%S", t); //second
}

static void tick_handler(struct tm* time, TimeUnits units_changed)
{
	set_time_display(time);
}

static void battery_handler(BatteryChargeState charge_state)
{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "battery_handler - charging: %d, percent: %d", charge_state.is_charging, charge_state.charge_percent);
    
    mIsCharging = charge_state.is_charging;

    if (mChargePercent != charge_state.charge_percent)
    {
        mChargePercent = charge_state.charge_percent;
    
        APP_LOG(APP_LOG_LEVEL_DEBUG, "battery_handler - mark canvas dirty");
        layer_mark_dirty(canvas);
    }
}

static void window_load(Window *window)
{
	window_set_background_color(window, GColorBlack);
	canvas = layer_create(GRect(0, 0, SCREEN_WIDTH , SCREEN_HEIGHT));
	layer_set_update_proc(canvas, (LayerUpdateProc) render);
	layer_add_child(window_get_root_layer(window), canvas);
    
    mTimeFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TREBUCHET_40));
    mTimeFontBold = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TREBUCHET_BOLD_40));
    mSmallFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TREBUCHET_BOLD_24));
    mTinyFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TREBUCHET_16));

	// Init all text fields
    const int mTwoDigitTextSize = sizeof("00:00 ");
    mDayText = malloc(mTwoDigitTextSize);
    mMonthText = malloc(mTwoDigitTextSize);
    mYearText = malloc(mTwoDigitTextSize);
    mHourText = malloc(mTwoDigitTextSize);
    mMinuteText = malloc(mTwoDigitTextSize);
    mSecondText = malloc(mTwoDigitTextSize);
    
	// Set initial time so display isn't blank
    struct tm* t;
	time_t temp;
	temp = time(NULL);
	t = localtime(&temp);
	set_time_display(t);
    
    battery_handler(battery_state_service_peek());
}

static void window_unload(Window *window) 
{
    app_timer_cancel(timer);
    fonts_unload_custom_font(mTimeFont);
    fonts_unload_custom_font(mTimeFontBold);
    fonts_unload_custom_font(mSmallFont);
    fonts_unload_custom_font(mTinyFont);
    layer_destroy(canvas);
}

static void init(void)
{
    window = window_create();
    window_set_window_handlers(window, (WindowHandlers) 
    {
		.load = window_load,
		.unload = window_unload,
    });

	tick_timer_service_subscribe(SECOND_UNIT, (TickHandler)tick_handler);

    battery_state_service_subscribe(battery_handler);

	window_stack_push(window, true);
}

static void deinit(void) 
{
    battery_state_service_unsubscribe();
	tick_timer_service_unsubscribe();
	window_destroy(window);
}

int main(void) 
{
	init();
	app_event_loop();
	deinit();
}