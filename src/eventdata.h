#pragma once
#include <sys/time.h>
  
typedef struct _slot {
  char* location;
  char* description;
  time_t start;
  time_t end;
  struct _slot* previous;
  struct _slot* next;
  unsigned int hype_id;
} slot;

void init_event_data();
void destroy_event_data();
unsigned char get_current_events(slot** event_list,unsigned char max_events,time_t* now);
unsigned char get_upcoming_events(slot** event_list,unsigned char max_events,time_t* now);
unsigned char get_num_locations();
//slot* get_nearest_event_at(int location_id,time_t* time);
slot* get_last();
char* get_location(unsigned char location_id);
