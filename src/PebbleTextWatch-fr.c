#include <pebble.h>

#include "time_constant-fr.h"

#include "resource_ids.auto.h"

#define NUMBER_OF_LAYER 5
#define MAX_CHAR_PER_LINE 12
#define LAYER_HEIGHT 168/NUMBER_OF_LAYER
#define VERTICAL_SHIFT (168 - NUMBER_OF_LAYER*LAYER_HEIGHT)/2

Window *window;
TextLayer* layers[NUMBER_OF_LAYER];

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

void time_as_time_text(struct tm *tick_time, char text[NUMBER_OF_LAYER][MAX_CHAR_PER_LINE]) {
  int current_line = 0;
  current_line = fill_lines(HOURS[tick_time->tm_hour], text, current_line);

  if(tick_time->tm_hour > 1) {
    strcpy(text[current_line + 1], "heures");
  } else {
    strcpy(text[current_line + 1], "heure");
  }
  
  current_line = fill_lines(MINUTES[tick_time->tm_min], text, current_line + 2);
}

void display_time(struct tm *tick_time)
{

  for(int i=0; i < NUMBER_OF_LAYER; i++) {
    memset(time_text[i], 0, MAX_CHAR_PER_LINE); 
  }  

  time_as_time_text(tick_time, time_text);

  for(int i=0; i < NUMBER_OF_LAYER; i++) {
    text_layer_set_text(layers[i], time_text[i]); 
  }
}

TextLayer* time_layer_create(int position, Layer* parent_layer, GFont font) {
  TextLayer *layer = text_layer_create(GRect(0, LAYER_HEIGHT * position + VERTICAL_SHIFT, 144, LAYER_HEIGHT));

  text_layer_set_text_color(layer, GColorWhite);
  text_layer_set_background_color(layer, GColorClear);
  text_layer_set_text_alignment(layer, GTextAlignmentLeft);
  text_layer_set_font(layer, font);
  layer_set_clips(text_layer_get_layer(layer), false);
  layer_add_child(parent_layer, text_layer_get_layer(layer));

  return layer;
}

Window* time_window_create() {
  Window* window = window_create();
  window_stack_push(window, true /* Animated */);
  window_set_background_color(window, GColorBlack);
  
  return window;
}

struct tm* get_current_tick_time() {
  struct tm *tick_time;
  time_t current_time = time(NULL);
  tick_time = localtime(&current_time);
  
  return tick_time;
}

void handle_init() {
  window = time_window_create();

  GFont font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_OPEN_SANS_CONDENSED_BOLD_30));
  for(int i=0; i< NUMBER_OF_LAYER; i++) {
    layers[i] = time_layer_create(i, window_get_root_layer(window), font);
  }

  display_time(get_current_tick_time());
}

void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  display_time(tick_time);
}

void handle_deinit() {
  for(int i=0; i< NUMBER_OF_LAYER; i++) {
      text_layer_destroy(layers[i]);
  } 

  window_destroy(window);
}

int main(void) {
  handle_init();
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
  app_event_loop();
  handle_deinit();
}