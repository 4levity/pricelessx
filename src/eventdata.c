#include <pebble.h>
#include <sys/time.h>
#include "eventdata.h"

static int year0=2015;
static int month0=7;
static int day0=1;

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

void create(unsigned char location_id,char* description,unsigned char day, unsigned char hour, unsigned char minute,int duration,int hype_id) {
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
  if(hype_id) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "loading image for %s",new_slot->description);
    new_slot->hype=gbitmap_create_with_resource(hype_id);
  } else {
    new_slot->hype=0;
  }
  insert(new_slot);
}

void init_event_data() {
  time_t now;
  time (&now);
  memcpy(&time_maker,localtime ( &now ),sizeof(struct tm));
  
  /* "dirt","forest","beach","outpost","elsewhere" */
  create(0,"XPRXO",1,21,0,60,0);
  create(0,"Bodhi",1,22,0,60,0);
  create(0,"yMe",1,23,0,60,0);
  create(0,"Griff",2,0,0,60,0);
  create(0,"SugarBeats",2,1,0,60,0);
  create(0,"Undercat",2,2,0,60,0);
  create(0,"Elevated Mind",2,3,0,60,0);
  create(0,"Tom Lattanand",2,20,0,60,0);
  create(0,"Palindrome",2,21,0,30,0);
  create(0,"Devin Kroes",2,21,30,60,0);
  create(0,"Ruxpin",2,22,30,75,0);
  create(0,"ALXDNR",2,23,0,60,0);
  create(0,"Mr. Bill",3,0,45,75,0);
  create(0,"Digital Rust",3,2,0,60,0);
  create(0,"influence & Galen (Dr. Whiskers)",3,3,0,60,0);
  create(0,"ill.gates",3,4,0,60,0);
  create(0,"Kid Kameleon",3,5,0,60,0);
  create(0,"GKEN-E",3,6,0,60,0);
  create(0,"Piotron",3,7,0,60,0);
  create(0,"Party within a Party",3,19,0,105,0);
  create(0,"Tumbledown House",3,20,45,60,0);
  create(0,"B. Bravo",3,21,45,60,0);
  create(0,"Obi-J",3,22,45,60,RESOURCE_ID_IMAGE_HYPE_OBI_J);
  create(0,"Tinker",3,23,45,45,0);
  create(0,"Troyboi",4,0,30,60,0);
  create(0,"AMB",4,1,30,60,0);
  create(0,"Sixis",4,2,30,60,0);
  create(0,"Knowa Lusion",4,3,30,60,0);
  create(0,"Spoken Bird",4,4,30,60,/*RESOURCE_ID_IMAGE_HYPE_SPOKEN_BIRD*/0);
  create(0,"send-off",4,10,0,120,0);

  create(4,"setup",1,10,0,600,0);
  create(4,"volunteers",1,11,00,30,0);
  create(4,"strike",4,12,0,480,0);
  create(4,"planners",2,11,00,30,0);
  create(4,"WM training",1,19,30,15,0);
  create(4,"WM training",2,11,30,15,0);
  create(4,"WM training",3,11,30,15,0);
  
  create(4,"sound meditation",2,10,0,60,0);
  create(1,"AcroYoga",2,11,0,60,0);
  create(4,"BeldenPutt",2,12,0,30,0);
  create(1,"New Games",2,13,0,30,0);
  create(1,"Juggling",2,14,0,60,0);
  create(1,"Yoga",2,15,0,90,0);
  create(4,"Miasma",2,15,0,90,0);
  create(4,"Integrate peak experience",2,16,0,60,0);
  create(1,"Bondage",2,17,0,60,0);
  create(4,"Sous Vide",2,17,0,60,0);
  create(4,"Airbrush",2,17,0,60,0);
  create(1,"Dreamhacking",2,18,0,60,0);
  create(4,"Frontier",2,23,0,60,0);
  create(4,"WordsWordsWords",3,0,0,60,0);
  create(4,"sound meditation",3,10,0,60,0);
  create(4,"yoga/clown",3,10,0,60,0);
  create(1,"pewter casting",3,10,0,60,0);
  create(2,"Breakfast+Bass",3,10,45,45,0);
  create(3,"meditation",3,11,0,30,0);
  create(4,"Bocce",3,13,0,60,0);
  create(1,"Baseworks",3,14,0,60,0);
  create(3,"painting",3,15,0,60,0);
  create(1,"meditation",3,15,30,30,0);
  create(4,"improvisation",3,16,30,60,0);
  create(4,"Airbrush",3,17,0,60,0);
  create(4,"Bucket marching band",3,17,30,45,0);

  // forest
  create(1,"Evolutionista",1,21,30,60,0);
  create(1,"watsonix",1,22,30,60,0);
  create(1,"Mashimon",1,23,30,60,0);
  create(1,"AMB",2,0,30,60,0);
  create(1,"Edamame",2,1,30,60,0);
  create(1,"1974",2,2,30,60,RESOURCE_ID_IMAGE_HYPE_1974);
  create(1,"sleep party",2,3,30,150,0);
  create(1,"Broxtronix",2,21,30,60,0);
  create(1,"Tinker",2,22,30,60,RESOURCE_ID_IMAGE_HYPE_TINKER);
  create(1,"set in sand",2,23,30,60,0);
  create(1,"Barrio Lindo",3,0,30,60,0);
  create(1,"Nicola Cruz",3,1,30,60,0);
  create(1,"Ametsub",3,2,30,60,0);
  create(1,"Ganucheau",3,3,30,60,RESOURCE_ID_IMAGE_HYPE_GANUCHEAU);
  create(1,"James Fonda w/ Saideh Eftekhari",3,4,30,60,0);
  create(1,"sleep party",3,5,30,150,0);
  create(1,"Daniel Berkman",3,17,0,60,0);
  create(1,"Dr. Toast",3,21,30,60,RESOURCE_ID_IMAGE_HYPE_TOAST);
  create(1,"Tones",3,22,30,60,0);
  create(1,"Michna",3,23,30,90,0);
  create(1,"David Last",4,1,0,60,0);
  create(1,"Kid Kameleon",4,2,0,60,0);
  create(1,"Contact Field Orchestra",4,3,0,60,0);
  create(1,"sleep party",4,4,0,180,0);
  
  // beach
  create(2,"JK47",2,12,0,60,0);
  create(2,"BK Willy",2,13,0,60,0);
  create(2,"Orphic",2,14,0,60,0);
  create(2,"CloZee",2,15,0,60,/*RESOURCE_ID_IMAGE_HYPE_CLOZEE*/0);
  create(2,"Defunk",2,16,0,60,0);
  create(2,"Neptune",2,17,0,60,0);
  create(2,"SheRa",3,11,0,60,0);
  create(2,"Sharps",3,12,0,60,0);
  create(2,"Dov1",3,13,0,60,0);
  create(2,"J*Labs",3,14,0,60,0/*RESOURCE_ID_IMAGE_HYPE_J_LABS*/);
  create(2,"benchun",3,15,0,60,0);
  create(2,"Troyboi",3,16,0,60,0);

  // outpost
  create(3,"Troubadours",1,20,0,240,0);
  create(3,"acoustic",2,20,0,330,0);
  create(3,"Sonotropic",3,11,30,60,0);
  create(3,"Troubadours",3,20,0,240,0);
}

void destroy_event_data() {
  slot *next;
  while(first != NULL) {
    next=first->next;
    if(first->hype) {
      gbitmap_destroy(first->hype);
    }
    free(first);
    first=next;
  }
}
