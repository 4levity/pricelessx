#include <pebble.h>
#include "browse.h"
#include "eventdata.h"
#include "eventtext.h"
  
static slot* selected_event;
static int selected_location_id;
static GBitmap* selected_bitmap;

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_icon_up;
static GBitmap *s_res_icon_etc;
static GBitmap *s_res_icon_down;
static GFont s_res_gothic_18_bold;
static ActionBarLayer *layer_actionbar;
static TextLayer *layer_location;
static TextLayer *layer_details;
static BitmapLayer *layer_logo;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
  s_res_icon_up = gbitmap_create_with_resource(RESOURCE_ID_ICON_UP);
  s_res_icon_etc = gbitmap_create_with_resource(RESOURCE_ID_ICON_ETC);
  s_res_icon_down = gbitmap_create_with_resource(RESOURCE_ID_ICON_DOWN);
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  // layer_actionbar
  layer_actionbar = action_bar_layer_create();
  action_bar_layer_add_to_window(layer_actionbar, s_window);
  action_bar_layer_set_background_color(layer_actionbar, GColorBlack);
  action_bar_layer_set_icon(layer_actionbar, BUTTON_ID_UP, s_res_icon_up);
  action_bar_layer_set_icon(layer_actionbar, BUTTON_ID_SELECT, s_res_icon_etc);
  action_bar_layer_set_icon(layer_actionbar, BUTTON_ID_DOWN, s_res_icon_down);
  layer_add_child(window_get_root_layer(s_window), (Layer *)layer_actionbar);
  
  // layer_location
  layer_location = text_layer_create(GRect(0, 0, 122, 22));
  text_layer_set_background_color(layer_location, GColorBlack);
  text_layer_set_text_color(layer_location, GColorWhite);
  text_layer_set_text(layer_location, "Text layer");
  text_layer_set_font(layer_location, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)layer_location);
  
  // layer_details
  layer_details = text_layer_create(GRect(0, 22, 124, 146));
  text_layer_set_text(layer_details, "Text layer");
  text_layer_set_font(layer_details, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)layer_details);
  
  // layer_logo
  layer_logo = bitmap_layer_create(GRect(0, 47, 144, 120));
  layer_add_child(window_get_root_layer(s_window), (Layer *)layer_logo);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  action_bar_layer_destroy(layer_actionbar);
  text_layer_destroy(layer_location);
  text_layer_destroy(layer_details);
  bitmap_layer_destroy(layer_logo);
  gbitmap_destroy(s_res_icon_up);
  gbitmap_destroy(s_res_icon_etc);
  gbitmap_destroy(s_res_icon_down);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

////

void show_event() {
  static char desc[80];
  static char t1[SHORT_TIME_LEN];
  static char t2[SHORT_TIME_LEN];
  if(selected_event) {
    short_time(&(selected_event->start),t1);
    short_time(&(selected_event->end),t2);
    struct tm *tick_time = localtime(&selected_event->start);
    snprintf(desc,80,"%s-%s:\n%s",t1,t2,selected_event->description);
    
    if(selected_event->hype_id) {
      if(selected_bitmap) {
        gbitmap_destroy(selected_bitmap);
        selected_bitmap=0;
      }
      selected_bitmap=gbitmap_create_with_resource(selected_event->hype_id);
      bitmap_layer_set_bitmap(layer_logo, selected_bitmap);
      layer_set_hidden((Layer*) layer_logo,false);
    } else {
      layer_set_hidden((Layer*) layer_logo,true);
    }
    text_layer_set_text(layer_details, desc);

    // day
    static char banner[30];
    static char day_buffer[10]; 
    strftime(day_buffer, 10, "%A", tick_time);
    snprintf(banner,30,"%s/%s",day_buffer,get_location(selected_location_id));
    text_layer_set_text(layer_location,banner);
  } else {
    text_layer_set_text(layer_details,"no event selected");
    text_layer_set_text(layer_location,get_location(selected_location_id));
  }
}

void scroll_up() {
  slot* new_event = selected_event;
  while(new_event->previous) {
    //APP_LOG(APP_LOG_LEVEL_INFO, "searching up for event at location %d..",selected_location_id);
    new_event = new_event->previous;
    if(new_event->location == get_location(selected_location_id)) {
      selected_event=new_event;
      //APP_LOG(APP_LOG_LEVEL_INFO, "found event %s..",selected_event->description);
      show_event();
      return;
    }
  }
}

void scroll_down() {
  slot* new_event = selected_event;
  while(new_event->next) {
    new_event = new_event->next;
    if(new_event->location == get_location(selected_location_id)) {
      selected_event=new_event;
      show_event();
      return;
    }
  }
}

void auto_select_event() {
  static time_t now;
  time(&now);
  get_upcoming_events(&selected_event,1,&now);
  if(!selected_event) {
    selected_event=get_last();
  }
  APP_LOG(APP_LOG_LEVEL_INFO, "auto selected event %s. location=%d %s"
	,selected_event?selected_event->description:"NULL",
 	selected_location_id,get_location(selected_location_id));
  
  scroll_up(); 
  scroll_down();
  APP_LOG(APP_LOG_LEVEL_INFO, "scrolled up to event %s",selected_event?selected_event->description:"NULL");
  //selected_event = get_nearest_event_at(selected_location_id,&now);
  //APP_LOG(APP_LOG_LEVEL_INFO, "auto selected event %s",selected_event?selected_event->description:"NULL");
  //show_event();
}

static void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(selected_event) {
    scroll_down();
  } else {
    auto_select_event();
  }
}

void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(selected_event) {
    scroll_up();
  } else {
    auto_select_event();
  }
}

void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  selected_location_id = (selected_location_id+1) % get_num_locations();
  auto_select_event();
}
void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  auto_select_event();
}
void select_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
}

void browse_config_provider(Window *window) {
 // single click / repeat-on-hold config:
  window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
  window_long_click_subscribe(BUTTON_ID_SELECT, 700, select_long_click_handler, select_long_click_release_handler);
}

void show_browse(void) {
  APP_LOG(APP_LOG_LEVEL_INFO, "showing pricelessx browser");
  initialise_ui();
  selected_location_id=0;
  selected_event=0;
  selected_bitmap=0;
  auto_select_event();
  
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_set_click_config_provider(s_window, (ClickConfigProvider)browse_config_provider);
  window_stack_push(s_window, true);
}

void hide_browse(void) {
  window_stack_remove(s_window, true);
}


