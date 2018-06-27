#include <pebble.h>
#include "win_watchface.h"
#include "eventdata.h"
#include "eventtext.h"
#include "browse.h"
  
static GBitmap *s_res_image_logo_belden;
static GBitmap *s_res_image_logo_portal;

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_image_logo_baroot;
static GFont s_res_droid_serif_28_bold;
static GFont s_res_gothic_24_bold;
static BitmapLayer *layer_logo;
static TextLayer *layer_time;
static TextLayer *layer_banner;
static TextLayer *layer_current;
static BitmapLayer *layer_hype;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
  s_res_image_logo_baroot = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_LOGO_BAROOT);
  s_res_droid_serif_28_bold = fonts_get_system_font(FONT_KEY_DROID_SERIF_28_BOLD);
  s_res_gothic_24_bold = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  // layer_logo
  layer_logo = bitmap_layer_create(GRect(88, 120, 56, 48));
  bitmap_layer_set_bitmap(layer_logo, s_res_image_logo_baroot);
  layer_add_child(window_get_root_layer(s_window), (Layer *)layer_logo);
  
  // layer_time
  layer_time = text_layer_create(GRect(0, 126, 92, 32));
  text_layer_set_background_color(layer_time, GColorClear);
  text_layer_set_text_color(layer_time, GColorWhite);
  text_layer_set_text(layer_time, "88:88p");
  text_layer_set_text_alignment(layer_time, GTextAlignmentCenter);
  text_layer_set_font(layer_time, s_res_droid_serif_28_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)layer_time);
  
  // layer_banner
  layer_banner = text_layer_create(GRect(0, 0, 144, 28));
  text_layer_set_background_color(layer_banner, GColorClear);
  text_layer_set_text_color(layer_banner, GColorWhite);
  text_layer_set_text(layer_banner, "priceless");
  text_layer_set_text_alignment(layer_banner, GTextAlignmentCenter);
  text_layer_set_font(layer_banner, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)layer_banner);
  
  // layer_current
  layer_current = text_layer_create(GRect(0, 24, 144, 98));
  text_layer_set_background_color(layer_current, GColorClear);
  text_layer_set_text_color(layer_current, GColorWhite);
  text_layer_set_text(layer_current, "abcd");
  text_layer_set_font(layer_current, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)layer_current);
  
  // layer_hype
  layer_hype = bitmap_layer_create(GRect(0, 3, 144, 120));
  layer_add_child(window_get_root_layer(s_window), (Layer *)layer_hype);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  bitmap_layer_destroy(layer_logo);
  text_layer_destroy(layer_time);
  text_layer_destroy(layer_banner);
  text_layer_destroy(layer_current);
  bitmap_layer_destroy(layer_hype);
  gbitmap_destroy(s_res_image_logo_baroot);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
  gbitmap_destroy(s_res_image_logo_belden);
  gbitmap_destroy(s_res_image_logo_portal);
}

/*
static void banner_draw_proc(Layer *layer, GContext *ctx) {
  graphics_draw_line(ctx, GPoint(0, 15), GPoint(144, 15));
}
*/

static void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  show_browse();
}

static void win_config_provider(Window *window) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
}


void show_win_watchface(void) {
  APP_LOG(APP_LOG_LEVEL_INFO, "showing pricelessx watchface");
  initialise_ui();
  s_res_image_logo_belden = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_LOGO_BELDEN);
  s_res_image_logo_portal = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_LOGO_PORTAL);
  text_layer_set_text_color(layer_banner, GColorChromeYellow);
  text_layer_set_overflow_mode(layer_current,GTextOverflowModeTrailingEllipsis);
  //layer_set_update_proc((Layer *)layer_banner,banner_draw_proc);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_set_click_config_provider(s_window, (ClickConfigProvider)win_config_provider);
  window_stack_push(s_window, true);
}

Window* get_win_watchface() {
  return s_window;
}

void hide_win_watchface(void) {
  window_stack_remove(s_window, true);
}

static char lowercase(char c) {
  if(c>='A' && c<='Z') {
    return 'a'+(c-'A');
  }
  return c;
}

void update_time() {
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
 
  static char time_buffer[] = "00:00a"; 
  strftime( time_buffer, sizeof("00:00a"), "%l:%Ma", tick_time);
  if(tick_time->tm_hour >= 12) {
    time_buffer[5]='p';
  }
  text_layer_set_text(layer_time,  time_buffer);
  
  static char day_buffer[4]; 
  static char banner_buffer[20];
  strftime(day_buffer, 4, "%A", tick_time);
  day_buffer[0]=lowercase(day_buffer[0]);
  snprintf(banner_buffer, 20, "priceless - %s", day_buffer);
  text_layer_set_text(layer_banner, banner_buffer);
  
  switch(tick_time->tm_min % 3) {
  case 0:
    bitmap_layer_set_bitmap(layer_logo, s_res_image_logo_baroot);
    break;
  case 1:
    bitmap_layer_set_bitmap(layer_logo, s_res_image_logo_portal);
    break;
  default:
    bitmap_layer_set_bitmap(layer_logo, s_res_image_logo_belden);
  }
}

void update_events() {
  GBitmap* hype=0;
  time_t now=time(NULL);
  char* current_text;

  //APP_LOG(APP_LOG_LEVEL_DEBUG, "about to update events");
  current_text=describe_current_events(&hype, &now);
  
  text_layer_set_text(layer_current, current_text);
  
  if(hype) {
    // hype event, hide text
    light_enable_interaction(); // flash!
    bitmap_layer_set_bitmap(layer_hype, hype);
    layer_set_hidden((Layer*) layer_hype,false);
    
  } else if(!current_text[0]) {
    // current + upcoming events to display
    layer_set_hidden((Layer*) layer_hype,true);
  } else {
    // nothing going on right now, maybe upcoming
    layer_set_hidden((Layer*) layer_hype,true);
  }
}

