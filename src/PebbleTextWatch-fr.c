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
TextLayer hello_layer;

PblTm t;

void display_time(PblTm *t)
{ 
  char * hours = "10:42";
  string_format_time(hours, 6, "%H:%M", t);
  text_layer_set_text(&hello_layer, hours);
}

void handle_init(AppContextRef ctx) {

  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorBlack);

  text_layer_init(&hello_layer, GRect(0, 65, 144, 30));
  text_layer_set_font(&hello_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_text_color(&hello_layer, GColorWhite);
  text_layer_set_background_color(&hello_layer, GColorClear);
  text_layer_set_text_alignment(&hello_layer, GTextAlignmentLeft);

  get_time(&t);
  display_time(&t);
  
  
  text_layer_set_text_alignment(&hello_layer, GTextAlignmentCenter);
  text_layer_set_font(&hello_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  layer_add_child(&window.layer, &hello_layer.layer);
}




void pbl_main(void *params) {


  PebbleAppHandlers handlers = {
    .init_handler = &handle_init
  };

  app_event_loop(params, &handlers);
}
