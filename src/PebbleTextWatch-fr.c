#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "time_constant-fr.h"

#include "resource_ids.auto.h"

#define MY_UUID { 0xFF, 0x9E, 0x5C, 0xAC, 0xBA, 0x01, 0x4A, 0xE5, 0x85, 0x4D, 0xA9, 0x86, 0x16, 0xCE, 0x02, 0xED }
PBL_APP_INFO(MY_UUID,
             "Simple Text Watch in french", "Crafting Labs",
             0, 1, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

#define NUMBER_OF_LAYER 5
#define MAX_CHAR_PER_LINE 12
#define LAYER_HEIGHT 168/NUMBER_OF_LAYER
#define VERTICAL_SHIFT (168 - NUMBER_OF_LAYER*LAYER_HEIGHT)/2

Window window;
TextLayer layers[NUMBER_OF_LAYER];
GFont font;


PblTm t;
char time_text[NUMBER_OF_LAYER][MAX_CHAR_PER_LINE];

int fill_lines(const char * line, char text[NUMBER_OF_LAYER][MAX_CHAR_PER_LINE], int start_position) {

  if (strlen(line) < MAX_CHAR_PER_LINE) {
    strcpy(text[start_position], line);
  } else {
    const char * search = line;
    char * pch = strstr (search, "-");
    if (pch!=NULL) {
      strncpy(text[start_position], search, pch - search);
      start_position++;
      search += pch - search + 1;
    }
    strcpy(text[start_position], search);
  }
  return start_position;
}

void time_as_time_text(PblTm *t, char text[NUMBER_OF_LAYER][MAX_CHAR_PER_LINE]) {

  int current_line = 0;
  current_line = fill_lines(HOURS[t->tm_hour], text, current_line);

  if(t->tm_hour > 1) {
    strcpy(text[current_line + 1], "heures");
  } else {
    strcpy(text[current_line + 1], "heure");
  }
  
  current_line = fill_lines(MINUTES[t->tm_min], text, current_line + 2);
}

void display_time(PblTm *t)
{
  for(int i=0; i < NUMBER_OF_LAYER; i++) {
    memset(time_text[i], 0, MAX_CHAR_PER_LINE); 
  }  

  time_as_time_text(t, time_text);

  for(int i=0; i < NUMBER_OF_LAYER; i++) {
    text_layer_set_text(&layers[i], time_text[i]); 
  }
}

void init_layer(TextLayer *layer, int position) {
  text_layer_init(layer, GRect(0, LAYER_HEIGHT * position + VERTICAL_SHIFT, 144, LAYER_HEIGHT));
  text_layer_set_text_color(layer, GColorWhite);
  text_layer_set_background_color(layer, GColorClear);
  text_layer_set_text_alignment(layer, GTextAlignmentLeft);
  text_layer_set_font(layer, font);
  layer_set_clips((Layer *) layer, false);
  layer_add_child(&window.layer, &layer->layer);
}

void handle_init(AppContextRef ctx) {

  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorBlack);

  resource_init_current_app(&RESOURCES_TEXTWATCH_FR);
  font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_OPEN_SANS_CONDENSED_BOLD_30));


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
