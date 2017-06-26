#include <pebble.h>
#include <sys/time.h>
#include "eventdata.h"
#include "win_watchface.h"
  
void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();

  // every 5 minutes
//  if(tick_time->tm_min % 5 == 0) {
    update_events();
//  }
}
 
void init() {
  init_event_data();

  show_win_watchface();
  update_time();
  update_events();

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);  
}
 
void deinit() {
  // Destroy Window
  hide_win_watchface();
  //window_destroy(win_main);
  destroy_event_data();
}
 
int main(void) {
  APP_LOG(APP_LOG_LEVEL_INFO, "pricelessx starting");
  init();
  app_event_loop();
  deinit();
}