#include <pebble.h>
#include <sys/time.h>
#include "eventdata.h"

static int year0=2017;
static int month0=6;
static int day0=29;

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
  
  // dirt
  create(0,"Tasstik",1,21,0,60,0);
  create(0,"roflcactus",1,22,0,60,0);
  create(0,"Kid Kameleon",1,23,0,60,0);
  create(0,"Obi-J",2,0,0,60,RESOURCE_ID_IMAGE_HYPE_OBI_J);
  create(0,"Mansion",2,1,0,60,0);
  create(0,"Shramp",2,2,0,60,0);
  
  create(0,"SheRa",2,21,0,60,0);
  create(0,"COPYCATT",2,22,0,60,0);
  create(0,"Elevated Mind",2,23,0,60,0);
  create(0,"Of The Trees",3,0,0,60,0);
  create(0,"Tinker",3,1,0,60,RESOURCE_ID_IMAGE_HYPE_TINKER);
  create(0,"ill.Gates",3,2,0,60,0);
  create(0,"Doctor Jeep",3,3,0,60,0);
  create(0,"GREAZUS",3,4,0,60,0);
  create(0,"Zenotope",3,5,0,60,0);
  
  create(0,"Jonny Ha$h",3,21,0,60,0);
  create(0,"Spoken Bird",3,22,0,60,0);
  create(0,"CharlestheFirst",3,23,0,60,0);
  create(0,"CloZee",4,0,0,60,0);
  create(0,"SOOHAN",4,1,0,60,0);
  create(0,"Perkulat0r",4,2,0,60,0);
  create(0,"Bodhi McBoatface",4,3,0,60,0);
  create(0,"XPRXO",4,4,0,60,0);

  // pwp
  create(0,"Birds of Distinction",3,19,0,120,0);
  // events - forest sat
  create(1,"Breathwork w/ Domini",2,9,0,60,0);
  create(1,"Capoeira w/ Megha",2,10,0,60,0);
  create(1,"AcroYoga",2,11,0,90,0);
  create(1,"Sewing with Sena",2,16,0,60,0);
  // events - forest sun
  create(1,"Moving the Expressive Body",3,10,0,60,0);
  create(1,"Parkour Through Physics",3,11,0,60,0);
  create(1,"Journaling with Stina",3,12,0,60,0);
  create(1,"Sensory Depriviation Massage",3,16,0,60,0);
  create(1,"Ignite Talks",3,18,0,60,0);
  // events - outpost fri
  create(3,"Aura Cleansing & Fluffing 101 with Eva",1,19,0,60,0);
  // events - outpost sat
  create(3,"No Complaining w/ Cianna",2,12,0,60,0);
  create(3,"Connecting Through Questions",2,15,0,60,0);
  create(3,"Play: Spontaneous Interaction",2,16,0,60,0);
  create(3,"Single Origin Cacao",2,17,0,60,0);
  // events - outpost sun
  create(3,"Knot Workshop",3,11,0,60,0);
  create(3,"Bird Costuming & Masks",3,12,0,60,0);
  create(3,"Earthly Delighted Exploration",3,15,0,60,0);
  create(3,"Yoga Nidra w/ Victoria",3,17,0,60,0);
  // events - other sat
  create(4,"Coffee Service w/ Sasha (lawn)",2,8,0,60,0);
  create(4,"Rock Appreciation w/ Gates (lodge)",2,9,0,60,0);
  create(0,"Kenjutsu with Joey",2,11,0,60,0);
  create(4,"Bedazzling (picnic tables)",2,13,0,60,0);
  create(4,"Liquid Nitrogen Ice Cream (picnic tables)",2,14,0,60,0);
  create(4,"Wife Carrying Competition (lodge)",2,15,0,60,0);
  create(4,"Cosmic Cosmetics w/ Hannah (lodge)",2,18,0,60,0);
  create(4,"Poetry Club w/ David (tracks)",3,0,0,60,0);
  // events - other sun
  create(4,"Coffee Service w/ Sasha (lawn)",3,8,0,60,0);
  create(4,"Marco Polo w/ EJ (water)",3,13,0,60,0);
  create(4,"Let's Make a Pop Up Book (picnic tables)",3,14,0,60,0);
  create(4,"Frankenstuffer (picnic tables)",3,15,0,60,0);

  // forest
  create(1,"The Naturalist",1,21,30,60,0);
  create(1,"Hickory",1,22,30,60,0);
  create(1,"1974",1,23,30,60,RESOURCE_ID_IMAGE_HYPE_1974);
  create(1,"Formanter",2,0,30,60,0);
  create(1,"Ganucheau",2,1,30,60,RESOURCE_ID_IMAGE_HYPE_GANUCHEAU);
  create(1,"Deru",2,2,30,90,0);
  create(1,"Sleep Party",2,4,00,240,0);

  create(1,"Daniel Berkman & Tom Lattanand",2,17,30,90,0);
  create(1,"Licorice",2,21,00,60,0);
  create(1,"Jyun Jyun",2,22,00,90,0);
  create(1,"Barda",2,23,30,90,0);
  create(1,"M.RUX",3,1,00,90,0);
  create(1,"Ramona Wouters",3,2,30,90,0);
  create(1,"Stephanie Luz",3,4,00,60,0);
  
  create(1,"Contact Field Orchestra",3,21,00,90,0);
  create(1,"Dr. Toast",3,22,30,60,RESOURCE_ID_IMAGE_HYPE_TOAST);
  create(1,"Broxtronix",3,23,30,60,0);
  create(1,"Tor",4,0,30,90,0);
  create(1,"Boudoir Research",4,2,0,60,0);
  create(1,"Griffin Crafts",4,3,00,60,0);
  
  // beach
  create(2,"Crayleb",2,11,0,60,0);
  create(2,"Medisin",2,12,0,60,0);
  create(2,"Neptune",2,13,0,60,0);
  create(2,"JK-47",2,14,0,60,0);
  create(2,"The Hood Internet",2,15,0,60,0);
  create(2,"J*Labs",2,16,0,60,0);
  create(2,"Tones",2,17,0,60,0);
  
  create(2,"Dov1",3,11,0,60,0);
  create(2,"MissTANGQ",3,12,0,60,0);
  create(2,"Undercat",3,13,0,60,0);
  create(2,"Knowa",3,14,0,60,0);
  create(2,"Megan Hamilton",3,15,0,60,0);
  create(2,"Devin Kroes",3,16,0,60,0);

  // outpost
  create(3,"Brother Kris Gospel/Blues",1,21,0,120,0);

  create(3,"Cosmopolis",2,20,00,45,0);
  create(3,"Bisi",2,21,00,45,0);
  create(3,"The Sam Chase",2,22,00,45,0);
  create(3,"Tumbledown House",2,23,15,75,0);
  create(3,"El Elle",3,0,45,60,0);
  create(3,"The Crux",3,2,00,75,0);
  
  create(3,"Tom Lattanand",3,21,00,45,0);
  create(3,"Brent Bishop",3,22,00,60,0);
  create(3,"Olga Nunes",3,23,15,75,0);
  create(3,"John Brothers Piano Company",4,1,00,75,0);
  create(3,"Daniel Berkman",4,2,30,60,0);
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
