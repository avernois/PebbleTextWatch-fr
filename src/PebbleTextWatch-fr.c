#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0xFF, 0x9E, 0x5C, 0xAC, 0xBA, 0x01, 0x4A, 0xE5, 0x85, 0x4D, 0xA9, 0x86, 0x16, 0xCE, 0x02, 0xED }
PBL_APP_INFO(MY_UUID,
             "Simple Text Watch in french", "Crafting Labs",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

#define NUMBER_OF_LAYER 3
Window window;

TextLayer layers[NUMBER_OF_LAYER];

typedef struct {
  char hour[20];
  char minutes[20];
  char text[20];
} TimeText;


TimeText time_text;

PblTm t;

static const char* const HOURS[] = {
  "minuit",
  "un",
  "deux",
  "trois",
  "quatre",
  "cinq",
  "six",
  "sept",
  "huit",
  "neuf",
  "dix",
  "onze",
  "midi"
};

static const char* const MINUTES[] = {
  "",          "un",              "deux",           "trois",           "quatre",           "cinq",           "six",           "sept",           "huit",           "neuf",
  "dix",       "onze",            "douze",          "treize",          "quatorze",         "quinze",         "seize",         "dix-sept",       "dix-huit",       "dix-neuf",
  "vingt",     "vingt-et-une",     "vingt-deux",     "vingt-trois",     "vingt-quatre",     "vingt-cinq",     "vingt-six",     "vingt-sept",     "vingt-huit",     "vingt-neuf",
  "trente",    "trente-et-une",    "trente-deux",    "trente-trois",    "trente-quatre",    "trente-cinq",    "trente-six",    "trente-sept",    "trente-huit",    "trente-neuf",
  "quarante",  "quarante-et-une",  "quarante-deux",  "quarante-trois",  "quarante-quatre",  "quarante-cinq",  "quarante-six",  "quarante-sept",  "quarante-huit",  "quarante-neuf",
  "cinquante", "cinquante-et-une", "cinquante-deux", "cinquante-trois", "cinquante-quatre", "cinquante-cinq", "cinquante-six", "cinquante-sept", "cinquante-huit", "cinquante-neuf"
};


void time_as_time_text(PblTm *t, TimeText *text) {
  strcpy(text->hour, HOURS[t->tm_hour % 12]);
  strcpy(text->minutes, MINUTES[t->tm_min]);

  if(t->tm_hour > 1) {
    strcpy(text->text, "heures");
  } else {
    strcpy(text->text, "heure");
  }
}

void display_time(PblTm *t)
{ 
  time_as_time_text(t, &time_text);

  text_layer_set_text(&layers[0], time_text.hour);
  text_layer_set_text(&layers[1], time_text.text);
  text_layer_set_text(&layers[2], time_text.minutes);
}

void init_layer(TextLayer *layer, int position) {
  text_layer_init(layer, GRect(0, 25 * position, 144, 25));
  text_layer_set_text_color(layer, GColorWhite);
  text_layer_set_background_color(layer, GColorClear);
  text_layer_set_text_alignment(layer, GTextAlignmentLeft);
  text_layer_set_font(layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  layer_add_child(&window.layer, &layer->layer);
}

void handle_init(AppContextRef ctx) {

  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorBlack);

  for(int i=0; i< NUMBER_OF_LAYER; i++) {
    init_layer(&layers[i], i);
  }

  get_time(&t);
  display_time(&t);
}

void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)ctx;

  display_time(t->tick_time);
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
  .tick_info = {
          .tick_handler = &handle_minute_tick,
          .tick_units = MINUTE_UNIT
        }
  };
  app_event_loop(params, &handlers);
}
