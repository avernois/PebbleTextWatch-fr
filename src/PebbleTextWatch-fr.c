#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0xFF, 0x9E, 0x5C, 0xAC, 0xBA, 0x01, 0x4A, 0xE5, 0x85, 0x4D, 0xA9, 0x86, 0x16, 0xCE, 0x02, 0xED }
PBL_APP_INFO(MY_UUID,
             "Simple Text Watch in french", "Crafting Labs",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;
TextLayer time_layer;
TextLayer hour_layer;
TextLayer minute_layer;

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
  "",          "un",              "deux",           "trois",          "quatre",           "cinq",           "six",           "sept",           "huit",           "neuf",
  "dix",       "onze",            "douze",          "treize",         "quatorze",         "quinze",         "seize",         "dix-sept",       "dix-huit",       "dix-neuf",
  "vingt",     "vingt-et-un",     "ving-deux",      "vingt-trois",    "vingt-quatre",     "vingt-cinq",     "vingt-six",     "vingt-sept",     "vingt-huit",     "vingt-neuf",
  "trente",    "trente-et-un",    "trente-deux",    "trente-trois",   "trente-quatre",    "trente-cinq",    "trente-six",    "trente-sept",    "trente-huit",    "trente-neuf",
  "quarante",  "quarante-et-un",  "quarante-deux",  "quarante-trois", "quarante-quatre",  "quarante-cinq",  "quarante-six",  "quarante-sept",  "quarante-huit",  "quarante-neuf",
  "cinquante", "cinquante-et-un", "cinquante-deux", "cinquante-trois","cinquante-quatre", "cinquante-cinq", "cinquante-six", "cinquante-sept", "cinquante-huit", "cinquante-neuf"
};

void display_time(PblTm *t)
{ 
  // Need to be static because they're used by the system later.
  static char time_text[6] = "00:00";
  string_format_time(time_text, sizeof(time_text), "%R", t);
  text_layer_set_text(&time_layer, time_text);
  text_layer_set_text(&hour_layer, HOURS[t->tm_hour % 12]);
  text_layer_set_text(&minute_layer, MINUTES[t->tm_min]);
}

void handle_init(AppContextRef ctx) {

  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorBlack);

  text_layer_init(&hour_layer, GRect(0, 0, 144, 25));
  text_layer_set_text_color(&hour_layer, GColorWhite);
  text_layer_set_background_color(&hour_layer, GColorClear);
  text_layer_set_text_alignment(&hour_layer, GTextAlignmentLeft);
  text_layer_init(&minute_layer, GRect(0, 25, 144, 50));
  text_layer_set_text_color(&minute_layer, GColorWhite);
  text_layer_set_background_color(&minute_layer, GColorClear);
  text_layer_set_text_alignment(&minute_layer, GTextAlignmentLeft);



  text_layer_init(&time_layer, GRect(0, 65, 144, 30));
  text_layer_set_text_color(&time_layer, GColorWhite);
  text_layer_set_background_color(&time_layer, GColorClear);
  text_layer_set_text_alignment(&time_layer, GTextAlignmentCenter);
  
  
  text_layer_set_font(&hour_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_font(&minute_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));  
  text_layer_set_font(&time_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  
  layer_add_child(&window.layer, &time_layer.layer);
  layer_add_child(&window.layer, &hour_layer.layer);
  layer_add_child(&window.layer, &minute_layer.layer);
  

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
