#include <pebble.h>
#include <sys/time.h>
#include "eventdata.h"

static int year0=2018;
static int month0=6;
static int day0=29;
#define FRI 0
#define SAT 1
#define SUN 2
#define MON 3

static char* locations[]={"dirt","forest","beach","outpost","other"};
#define DIRT 0
#define FOREST 1
#define BEACH 2
#define OUTPOST 3
#define OTHER 4

static slot* first=NULL;
static slot* last=NULL;
//static slot* current_or_next=NULL;
static struct tm time_maker;

unsigned char get_num_locations() {
  return 5;
}

char* get_location(unsigned char location_id) {
  return locations[location_id];
}

slot* get_last() {
  return last;
}
/*
slot* get_nearest_event_at(int location_id,time_t* time) {
  slot* e=first;
  slot* found_any=0;
  while(e) {
    if (e->location==locations[location_id]) {
      if(e->start < *time)
        return e;
      if(!found_any)
        found_any=e;
    }
    e=e->next;
  }
  return found_any;
}
*/
unsigned char get_current_events(slot** event_list,unsigned char max_events,time_t* now) {
  unsigned char events = 0;
  slot* e = first;
  while(e && events<max_events && e->start <= *now) {
    if ((e->start <= *now) && (e->end > *now)) {
      event_list[events++]=e;
    }
    e = e->next;
  }
  return events;
}

unsigned char get_upcoming_events(slot** event_list,unsigned char max_events,time_t* now) {
  unsigned char events = 0;
  slot* e = first;
  while(e && events<max_events) {
    if (e->start > *now) {
      event_list[events++]=e;
    }
    e = e->next;
  }  
  return events;
}

void insert(slot* new_slot) {
  if(first==NULL) {
    new_slot->previous=NULL;
    new_slot->next=NULL;
    first=new_slot;
    last=new_slot;
  } else if(new_slot->start>last->start || (new_slot->start==last->start && new_slot->location>=last->location)) {
    new_slot->next=NULL;
    new_slot->previous=last;
    last->next=new_slot;
    last=new_slot;
  } else if(new_slot->start<first->start || (new_slot->start==first->start && new_slot->location<=first->location)) {
    first->previous=new_slot;
    new_slot->next=first;
    new_slot->previous=NULL;
    first=new_slot;
  } else {
    // somewhere in the middle
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "insert middle %s",new_slot->description);
    slot* s=first;
    while(s->next!=NULL && (s->next->start<new_slot->start || (s->next->start==new_slot->start && s->next->location<new_slot->location))) {
      //APP_LOG(APP_LOG_LEVEL_DEBUG, "after %s",s->next->description);
      s=s->next;
    }
    if(s->next==NULL) {
      APP_LOG(APP_LOG_LEVEL_ERROR, "failed to insert slot %s , broken list",new_slot->description);
    } else {
      new_slot->next=s->next;
      new_slot->previous=s;
      s->next=new_slot;
      new_slot->next->previous=new_slot;
    }
  }
}

void create(unsigned char location_id,char* description,unsigned char day, unsigned char hour, unsigned char minute,int duration,unsigned int hype_id) {
  slot* new_slot = (slot*)malloc(sizeof(slot));
  new_slot->location=locations[location_id];
  new_slot->description=description;
  struct tm t;
  memcpy(&t,&time_maker,sizeof(struct tm));
  t.tm_year=year0-1900;
  if (month0 == 6 && day0+day > 30) {
    t.tm_mon=month0;
    t.tm_mday=day0+day-30;
  } else {
    t.tm_mon=month0-1;
    t.tm_mday=day0+day;
  }
  t.tm_hour=hour;
  t.tm_min=minute;
  new_slot->start = mktime(&t);
  new_slot->end = (new_slot->start) + (60 * (int)duration);
  new_slot->hype_id=hype_id;
/*
  if(hype_id) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "loading image for %s",new_slot->description);
    new_slot->hype=gbitmap_create_with_resource(hype_id);
  } else {
    new_slot->hype=0;
  }
*/
  insert(new_slot);
}

void init_event_data() {
  time_t now;
  time (&now);
  memcpy(&time_maker,localtime ( &now ),sizeof(struct tm));
  
  /* "dirt","forest","beach","outpost","elsewhere" */
  
  // dirt friday night
  create(DIRT, "roflcactus", FRI, 21, 0, 60, 0);
  create(DIRT, "DJJD", FRI, 22, 0, 60, 0);
  create(DIRT, "Undercat", FRI, 23, 0, 60, 0);
  create(DIRT, "Naughty Princess", SAT, 00, 0, 60, 0);
  create(DIRT, "Kid Kameleon", SAT, 01, 0, 60, RESOURCE_ID_IMAGE_HYPE_KIDK);
  create(DIRT, "Supertask", SAT, 02, 0, 60, 0);
  create(DIRT, "Shramp", SAT, 03, 0, 60, 0);
  // dirt saturday night
  create(DIRT, "TasstiK", SAT, 21, 0, 60, 0);
  create(DIRT, "HMU", SAT, 22, 0, 60, 0);
  create(DIRT, "Morillo", SAT, 23, 0, 60, 0);
  create(DIRT, "MEMBA", SUN, 0, 0, 60, 0);
  create(DIRT, "X&G", SUN, 1, 0, 60, 0);
  create(DIRT, "GREAZUS", SUN, 2, 0, 60, 0);
  create(DIRT, "Sigrah", SUN, 3, 0, 60, 0);
  create(DIRT, "Aesthetic Kid", SUN, 4, 0, 60, 0);
  create(DIRT, "Spoken Bird", SUN, 5, 0, 60, 0);
  // dirt sunday night
  create(DIRT, "PwP: PANTONE", SUN, 19, 0, 120, 0);
  create(DIRT, "Kyrian Bobeerian", SUN, 21, 0, 60, 0);
  create(DIRT, "JK-47", SUN, 22, 0, 60, 0);
  create(DIRT, "Tinker", SUN, 23, 0, 60, RESOURCE_ID_IMAGE_HYPE_TINKER);
  create(DIRT, "Of The Trees", MON, 0, 0, 60, 0);
  create(DIRT, "ill.Gates", MON, 1, 0, 60, 0);
  create(DIRT, "Pedestrian Tactics", MON, 2, 0, 60, 0);
  create(DIRT, "A Hundred Drums", MON, 3, 0, 60, 0);
  create(DIRT, "Bodhi McBoatface", MON, 4, 0, 60, RESOURCE_ID_IMAGE_HYPE_BODHI);
  create(DIRT, "Djedi", MON, 5, 0, 60, 0);
  
  // forest friday night
  create(FOREST, "Migaloo", FRI, 21, 0, 75, 0);
  create(FOREST, "Broxtronix", FRI, 22, 15, 75, 0);
  create(FOREST, "Watsonix", FRI, 23, 30, 75, 0);
  create(FOREST, "Mr. Moo", SAT, 0, 45, 90, 0);
  create(FOREST, "Ganucheau", SAT, 2, 15, 75, RESOURCE_ID_IMAGE_HYPE_GANUCHEAU);
  create(FOREST, "Sleep Party", SAT, 3, 30, 120, 0);
  // forest saturday night
  create(FOREST, "Tumbledown House", SAT, 21, 0, 90, 0);
  create(FOREST, "Jyun Jyun", SAT, 22, 30, 75, 0);
  create(FOREST, "Dr. Toast", SAT, 23, 45, 75, RESOURCE_ID_IMAGE_HYPE_TOAST);
  create(FOREST, "Sidirum", SUN, 1, 0, 90, 0);
  create(FOREST, "Ground", SUN, 2, 30, 90, 0);
  create(FOREST, "Christina Chatfield", SUN, 4, 0, 75, 0);
  // forest sunday night
  create(FOREST, "Griffin Crafts", SUN, 21, 0, 75, 0);
  create(FOREST, "El Suchi", SUN, 22, 15, 90, 0);
  create(FOREST, "Keerd", SUN, 23, 45, 75, 0);
  create(FOREST, "Rumpistol", MON, 1, 0, 90, 0);
  create(FOREST, "Lola Villa", MON, 2, 30, 90, 0);
  create(FOREST, "7e", MON, 4, 0, 90, 0);

  // outpost friday night
  create(OUTPOST, "Brother Kris (all vinyl set)", FRI, 21, 0, 120, 0);
  // outpost saturday night
  create(OUTPOST, "Alexandra Dwight", SAT, 20, 0, 45, 0);
  create(OUTPOST, "Oddjob Ensemble", SAT, 21, 0, 60, 0);
  create(OUTPOST, "Rachel Lark", SAT, 22, 15, 45, 0);
  create(OUTPOST, "Whistlin' Dyl and the Wyld Cowboyz", SAT, 23, 15, 60, 0);
  create(OUTPOST, "John Brothers Piano Company", SUN, 0, 30, 90, 0);
  create(OUTPOST, "Daniel Berkman", SUN, 2, 15, 90, 0);
  // outpost sunday night
  create(OUTPOST, "Rachele Eve", SUN, 21, 0, 45, 0);
  create(OUTPOST, "Moonshine Jelly", SUN, 22, 0, 45, 0);
  create(OUTPOST, "Mad Noise", SUN, 23, 0, 75, 0);
  create(OUTPOST, "Duchess Betsy", MON, 0, 30, 75, 0);
  create(OUTPOST, "Brother Kris (all vinyl set)", MON, 2, 0, 75, 0);

  // beach saturday
  create(BEACH, "Kev/null", SAT, 11, 0, 60, 0);
  create(BEACH, "MissTANGQ", SAT, 12, 0, 60, 0);
  create(BEACH, "Crayleb", SAT, 13, 0, 60, 0);
  create(BEACH, "Erica Dee", SAT, 14, 0, 60, 0);
  create(BEACH, "Doctor Jeep", SAT, 15, 0, 60, 0);
  create(BEACH, "Megan Hamilton & The Bermudas", SAT, 16, 0, 60, 0);
  create(BEACH, "Urple Eeple", SAT, 17, 0, 60, 0);
  create(BEACH, "sidcox", SAT, 18, 0, 60, 0);
  // beach sunday
  create(BEACH, "Tom Lattanand", SUN, 11, 0, 60, 0);
  create(BEACH, "Mischa", SUN, 12, 0, 60, 0);
  create(BEACH, "Tones", SUN, 13, 0, 60, 0);
  create(BEACH, "SOOHAN", SUN, 14, 0, 60, 0);
  create(BEACH, "Obj-J", SUN, 15, 0, 60, RESOURCE_ID_IMAGE_HYPE_OBI_J);
  create(BEACH, "Maddy O'Neal", SUN, 16, 0, 60, 0);
  create(BEACH, "Seapora", SUN, 17, 0, 60, 0);
  
  // friday activities 
  create(OTHER,"Open Decks & DIYBBQ (lawn)",FRI, 15, 0, 360, 0);
  // saturday activities 
  create(OTHER,"Coffee Service with Sasha (lawn)",SAT, 8, 30, 60, 0);
  create(FOREST,"Baby Movement Workshop",SAT, 9, 0, 60, 0);
  create(FOREST,"Neurohacking through Movement",SAT, 10, 0, 60, 0);
  create(OUTPOST,"Slow Vinyasa",SAT, 11, 0, 60, 0);
  create(OTHER,"Bad Tattoos (lodge)",SAT, 12, 0, 60, 0);
  create(OUTPOST,"Get Your Consent On",SAT, 13, 0, 60, 0);
  create(FOREST,"Storytelling",SAT, 14, 0, 60, 0);
  create(OTHER,"PANTONE prep Paper Play (picnic tables)",SAT, 14, 0, 60, 0);
  create(FOREST,"Rock this Life!",SAT, 15, 0, 60, 0);
  create(DIRT,"Parkour Through Physics",SAT, 15, 0, 60, 0);
  create(FOREST,"Improv for Novices",SAT, 16, 0, 60, 0);
  create(OUTPOST,"Play with Your Head",SAT, 16, 0, 60, 0);
  create(FOREST,"Hacking the Brain",SAT, 17, 0, 60, 0);
  create(OUTPOST,"Knots & Tea",SAT, 17, 0, 60, 0);
  create(FOREST,"Ignite Priceless 3",SAT, 18, 0, 60, 0);
  create(DIRT, "Doc Brown's Sunset Poetry Picnic", SAT, 20, 0, 60, 0);
  create(BEACH,"Stargazing & Astronomy",SAT, 23, 0, 60, 0);
  // sunday activities
  create(OTHER,"Coffee Service with Sasha (lawn)", SUN, 8, 30, 60, 0);
  create(DIRT,"Buugeng 101 with Rion",SUN, 10, 0, 60, 0);
  create(FOREST,"Art of Falling Down",SUN, 11, 0, 60, 0);
  create(OUTPOST,"Storytime, Snacks & Nap",SUN, 11, 0, 120, 0);
  create(FOREST,"Go with the Flow",SUN, 12, 0, 60, 0);
  create(DIRT,"Shake Your Dragon Booty",SUN, 13, 0, 60, 0);
  create(OUTPOST,"Work That Reconnects w Tibet",SUN, 14, 0, 60, 0);
  create(OUTPOST,"Sake Brewing",SUN, 16, 0, 60, 0);
  create(FOREST,"Hot Salsa Dancing!", SUN, 17, 0, 60, 0);
  create(OUTPOST,"Gregorian Chant",SUN, 18, 0, 60, 0);
  create(FOREST,"Power of Play",SUN, 18, 0, 60, 0);
  create(OTHER,"Werewolf Poetry (tracks)",MON, 0, 0, 60, 0);
  // monday activities 
  create(OTHER, "Sendoff", MON, 10, 0, 120, 0);

}

void destroy_event_data() {
  slot *next;
  while(first != NULL) {
    next=first->next;
 //   if(first->hype) {
 //     gbitmap_destroy(first->hype);
 //   }
    free(first);
    first=next;
  }
}
