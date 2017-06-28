#include <pebble.h>
#include "prideface.h"
#include "messaging/messaging.h"
#include "config/settings.h"

static Window *main_window;
static Layer *flag;
static TextLayer *time_layer, *date_layer;
static GFont time_font;

extern int *flags[];
extern int flag_segments[];

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  static char time_buffer[] = "00:00";
  static char date_buffer[] = "WWW MMM DD";

  if (clock_is_24h_style()) {
    strftime(time_buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    strftime(time_buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  text_layer_set_text(time_layer, time_buffer);

  strftime(date_buffer, sizeof("WWW MMM DD"), "%a %b %d", tick_time);
  text_layer_set_text(date_layer, date_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void draw_flag(int segments, int colors[], GContext *ctx) {
  GRect bounds = layer_get_bounds(window_get_root_layer(main_window));
  int segment_height = bounds.size.h / segments + (bounds.size.h % segments != 0);
  int segment_width = bounds.size.w;

  for (int i = 0; i < segments; i++) {

    int current_y = segment_height * i;
    GRect current_rect = GRect(0, current_y, segment_width, segment_height);

    graphics_context_set_fill_color(ctx, GColorFromHEX(colors[i]));
    graphics_fill_rect(ctx, current_rect, 0, GCornerNone);

  }
}

static void flag_update_proc(Layer *layer, GContext *ctx) {
  draw_flag(flag_segments[current_flag], flags[current_flag], ctx);
}

void update_flag() {
  layer_mark_dirty(flag);
}

static void size_text_layers() {
  GRect bounds = layer_get_bounds(window_get_root_layer(main_window));
  GSize time_size = text_layer_get_content_size(time_layer);

  layer_set_frame(text_layer_get_layer(time_layer), GRect(0, ((bounds.size.h / 2) - (time_size.h / 2) - 22), bounds.size.w, time_size.h));
  GRect time_bounds = layer_get_frame(text_layer_get_layer(time_layer));


  GSize date_size = text_layer_get_content_size(date_layer);

  layer_set_frame(text_layer_get_layer(date_layer), GRect(0, time_bounds.origin.y + time_size.h, bounds.size.w, date_size.h));
}

static void main_window_load(Window *window) {
  GRect bounds = layer_get_bounds(window_get_root_layer(window));

  flag = layer_create(bounds);
  layer_set_update_proc(flag, flag_update_proc);
  layer_add_child(window_get_root_layer(window), flag);

  time_layer = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text_color(time_layer, GColorBlack);
  text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS));
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));

  date_layer = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_text_color(date_layer, GColorBlack);
  text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));

  update_time();
  size_text_layers();
}

static void main_window_unload(Window *window) {}

static void init() {
  main_window = window_create();

  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  init_messaging();
  load_settings();

  window_stack_push(main_window, true);
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  window_destroy(main_window);
  tick_timer_service_unsubscribe();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
