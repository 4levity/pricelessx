#include <pebble.h>
#include <sys/time.h>
#include "eventdata.h"

static int year0=2016;
static int month0=7;
static int day0=0;

static char* locations[]={"dirt","forest","beach","outpost","other"};
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
  t.tm_mon=month0-1;
  t.tm_mday=day0+day; /* works because event is within same month..*/
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
  
  // dirt
  create(0,"Secret Recipe",1,22,0,60,0);
  create(0,"Labrat",1,23,0,60,0);
  create(0,"Obi-J",2,0,0,60,RESOURCE_ID_IMAGE_HYPE_OBI_J);
  create(0,"Elevated Mind",2,1,0,60,0);
  create(0,"Devin Kroes",2,2,0,60,0);
  create(0,"Big Joe Daddy",2,21,0,60,0);
  create(0,"Fungineers",2,22,0,60,0);
  create(0,"Tinker",2,23,0,60,RESOURCE_ID_IMAGE_HYPE_TINKER);
  create(0,"Sorrow",3,0,0,60,0);
  create(0,"Psymbionic",3,1,0,60,0);
  create(0,"Spoken Bird",3,2,0,60,0);
  create(0,"SteLouse",3,3,0,60,0);
  create(0,"X&G",3,4,0,60,0);
  create(0,"Charlesthefirst",3,5,0,60,0);
  create(0,"Dailon",3,6,0,60,0);
  create(0,"JK47",3,21,0,60,0);
  create(0,"Space Geisha",3,22,0,60,0);
  create(0,"Boudoir Research",3,23,0,60,0);
  create(0,"Greazus",4,0,0,60,0);
  create(0,"Ill Gates",4,1,0,60,0);
  create(0,"kLL sMTH",4,2,0,60,0);
  create(0,"Shramp",4,3,0,60,0);
  create(0,"Bogtrotter",4,4,0,60,0);
  create(0,"Tones",4,5,0,60,0);

  // events
  create(1,"Hoops with Leila",2,10,0,60,0);
  create(3,"Inner Power Animal",2,10,0,60,0);
  create(1,"AcroYoga",2,11,0,90,0);
  create(4,"Slackline",2,12,0,60,0);
  create(1,"Peak Experience",2,12,30,60,0);
  create(4,"Portrait Test",2,13,0,60,0);
  create(1,"Clown Customs",2,14,0,60,0);
  create(4,"Bad Idea Party Games",2,14,0,120,0);
  create(1,"Priceless in a Bottle",2,15,0,60,0);
  create(4,"Ice Cream Dance Party",2,15,0,60,0);
  create(3,"Spa Belden",2,16,0,60,0);
  create(1,"Flow Arts",2,17,0,90,0);
  create(4,"Improv with Moldover",2,18,0,60,0);
  create(2,"Port & Poetry",3,6,0,60,0);
  create(1,"Meditation",3,9,0,60,0);
  create(4,"Acro Games",3,10,0,60,0);
  create(1,"Gyrokinesis",3,11,0,60,0);
  create(0,"Japanese Sword Fighting",3,11,0,60,0);
  create(4,"Molkky",3,13,0,60,0);
  create(1,"Hug Workship with Issac",3,14,0,60,0);
  create(1,"Juggling Workshop",3,15,0,60,0);
  create(3,"Herbal Refresh",3,15,0,60,0);
  create(4,"Inner Tube Craft",3,15,0,60,0);
  create(1,"Couples Massage",3,16,0,60,0);
  create(3,"Transformation with Bisi",3,16,0,60,0);
  create(4,"Buugeng 101",3,16,0,60,0);
  create(1,"Ignite Talks",3,17,0,60,0);
  create(3,"Sprezzatura",3,17,0,60,0);
  create(1,"Remembering the Magic Moment",3,18,0,60,0);
  create(0,"Save Mos Eisley Cantina",3,19,0,120,0);
  create(4,"Midnight Poetry at the Tracks",4,0,0,60,0);

  // forest
  create(1,"Griffin Crafts",1,21,30,60,0);
  create(1,"Shipwrek",1,22,30,60,0);
  create(1,"Rumpistol ft Tobias Buch",1,23,30,90,0);
  create(1,"Olaf Stuut",2,1,00,60,0);
  create(1,"Broxtronix",2,2,00,60,0);
  create(1,"yMe",2,3,00,60,0);
  create(1,"Sleep Party",2,4,00,240,0);

  create(1,"1974",2,21,00,60,RESOURCE_ID_IMAGE_HYPE_1974);
  create(1,"Arcus Tenebrae",2,22,00,60,0);
  create(1,"Dr. Toast",2,23,00,60,RESOURCE_ID_IMAGE_HYPE_TOAST);
  create(1,"Barrio Lindo",3,0,00,60,0);
  create(1,"El Buho",3,1,00,60,0);
  create(1,"Erin E",3,2,00,75,0);
  create(1,"David Last & Lucas Vesely",3,3,15,75,0);
  create(1,"Watsonix",3,4,30,60,0);
  create(1,"Sleep Party",3,5,30,150,0);
  
  create(1,"Daniel Berkman & Tom Lattanand",3,21,00,60,0);
  create(1,"Bolo",3,22,00,75,0);
  create(1,"History of Color",3,23,15,75,0);
  create(1,"Lusine",4,0,30,75,0);
  create(1,"Olaf Stuut live",4,1,45,75,0);
  create(1,"Last/Toast",4,3,00,60,0);
  create(1,"Ganucheau",4,4,00,60,RESOURCE_ID_IMAGE_HYPE_GANUCHEAU);
  create(1,"Sleep Party",4,5,00,180,0);
  
  // beach
  create(2,"Bodhi McBoatface",2,11,0,60,0);
  create(2,"El Papachanga",2,12,0,60,0);
  create(2,"AfroQBen",2,13,0,60,0);
  create(2,"SheRa",2,14,0,60,0);
  create(2,"The OriGinALz",2,15,0,60,0);
  create(2,"Electric Mantis",2,16,0,60,0);
  create(2,"Imagined Herbal Flows",2,17,0,60,0);
  create(2,"kudo",3,11,0,60,0);
  create(2,"MAMA CROW",3,12,0,60,0);
  create(2,"DJJD",3,13,0,60,0);
  create(2,"Kid Kameleon",3,14,0,60,0);
  create(2,"J*Labs",3,15,0,60,0);
  create(2,"perkulat0r",3,16,0,60,0);
  create(2,"Urple Eeple",3,17,0,60,0);

  // outpost
  create(3,"Bisi",2,18,0,45,0);
  create(3,"Tom Lattanand and Daniel Berkman",2,18,45,120,0);
  create(3,"Tumbledown House",2,20,45,65,0);
  create(3,"Noam Smooha",2,21,50,50,0);
  create(3,"Graham Patzner",2,22,40,50,0);
  create(3,"Olga Nunes",2,23,30,65,0);
  create(3,"Brent Bishop",3,0,35,65,0);
  create(3,"Bolo w/ Evan Frasier",3,1,40,60,0);
  create(3,"Troubadours",3,18,30,150,0);
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
