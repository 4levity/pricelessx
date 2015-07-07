#include <pebble.h>
#include "eventtext.h"
#include "eventdata.h"

static GBitmap* current_hype=0;

void short_time(time_t* tim,char buffer[SHORT_TIME_LEN]) {
  struct tm *event_time = localtime(tim);
  char m;
  int hr;
  
  hr = event_time->tm_hour;
  if(hr==0 || hr==12)
    hr=12;
  else
    hr=hr%12;
  
  m = (event_time->tm_hour>11?'p':'a');
  
  if(event_time->tm_min == 0) {
    snprintf(buffer,SHORT_TIME_LEN,"%d%c",hr,m);
  } else {
    snprintf(buffer,SHORT_TIME_LEN,"%d:%02d%c",hr,event_time->tm_min,m);
  }
}

// returns true if there might be space left
static bool append(char* buffer,int size,char* more) {
  int current_len=strlen(buffer);
  int more_len=strlen(more);
  int chars_to_append=more_len;
  if(current_len+more_len>=size) {
    buffer[size-1]=0;
    buffer[size-2]='.';
    buffer[size-3]='.';
    buffer[size-4]='.';

    chars_to_append=size-current_len-4;
    if(chars_to_append>0) {
      //APP_LOG(APP_LOG_LEVEL_DEBUG, "appending %d of %d chars of %s to %s (implicit term)", chars_to_append, more_len, more, buffer);
      strncpy(&buffer[current_len],more,chars_to_append);
    } else {
      //APP_LOG(APP_LOG_LEVEL_DEBUG, "appending NO chars of %s to %s", more, buffer);
    }
    return false;
  } //else
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "appending %d chars of %s to %s", chars_to_append, more, buffer);
  strncpy(&buffer[current_len],more,chars_to_append);
  buffer[current_len+chars_to_append]=0;
  return true;
}

#define EVENT_TEXT_MAX 100
#define EVENTS_MAX 8
static void describe_events(char* caller_buffer,int size,slot** events,int num_events) {
  slot* event;
  int i;
  bool space_left;
  static char tbuffer[SHORT_TIME_LEN];
  static char buf[EVENT_TEXT_MAX];
  time_t last_start_time=0;
  
  caller_buffer[0]=0;
  if(num_events > 0) {
    space_left=true;
    for(i=0;i<num_events && space_left;i++) {
      event = events[i];
      if(event->start != last_start_time) {
        short_time(&(event->start),tbuffer);
        snprintf(buf,EVENT_TEXT_MAX,"%s%s-%s",(caller_buffer[0]?"\n":""),tbuffer,event->description);
      } else {
        snprintf(buf,EVENT_TEXT_MAX,", %s",event->description);
      }
      space_left=append(caller_buffer,size,buf);
      last_start_time=event->start;
    }
  }
}

char* describe_current_events(GBitmap** hype,time_t* now) {
  slot* current_events[EVENTS_MAX];
  int num_current,num_upcoming;
  static char buf[EVENT_TEXT_MAX];  
  slot* event;
  int i;
  
  num_current = get_current_events(current_events,EVENTS_MAX,now);
  *hype=0;

  buf[0]=0;
  if(num_current > 0) {
    for(i=0;i<num_current;i++) {
      event = current_events[i];
      if(event->hype_id) {
        if(current_hype)
          gbitmap_destroy(current_hype);
        current_hype=gbitmap_create_with_resource(event->hype_id);
        *hype=current_hype;
        break;
      }
    }
  }
  
  num_upcoming = get_upcoming_events(&current_events[num_current],EVENTS_MAX-num_current,now);
  if(num_current == 0 && num_upcoming == 0) {
    strcpy(buf,"lovelovelovelovelovelovelovelovelovelovelovelovelovelovelovelovelovelovelove");
  } else {
    describe_events(buf,EVENT_TEXT_MAX,current_events,num_current+num_upcoming);
  }
  
  return buf;
}

