#include <pebble.h>
#include <pebble-effect-layer/pebble-effect-layer.h>
#include "prideface.h"
#include "messaging/messaging.h"
#include "config/settings.h"

static Window *main_window;
static Layer *flag, *time_band;
static TextLayer *time_layer;
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

}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void draw_flag(int segments, int colors[], GContext *ctx) {

  GRect bounds = layer_get_bounds(window_get_root_layer(main_window));
  int segment_height = bounds.size.h / segments + (bounds.size.h % segments != 0);
  int segment_width = bounds.size.w;

  for (int i = 0; i < segments; i++) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Loop index now %d", i);

    int current_y = segment_height * i;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Current y value %d", current_y);
    GRect current_rect = GRect(0, current_y, segment_width, segment_height);

    graphics_context_set_fill_color(ctx, GColorFromHEX(colors[i]));
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Current color %d", colors[i]);
    graphics_fill_rect(ctx, current_rect, 0, GCornerNone);

  }

}

static void flag_update_proc(Layer *layer, GContext *ctx) {
  draw_flag(flag_segments[current_flag], flags[current_flag], ctx);
}

void update_flag() {
  layer_mark_dirty(flag);
}

static void time_band_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(window_get_root_layer(main_window));
}

static void main_window_load(Window *window) {

  GRect bounds = layer_get_bounds(window_get_root_layer(window));

  flag = layer_create(bounds);
  layer_set_update_proc(flag, flag_update_proc);

  time_band = layer_create(bounds);
  layer_set_update_proc(time_band, time_band_update_proc);

  layer_add_child(window_get_root_layer(window), flag);

  time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GILBERT_46));

  int time_x = 0;
  int time_y = (bounds.size.h / 2) - 35;
  int time_w = bounds.size.w;
  int time_h = bounds.size.h;

  time_layer = text_layer_create(GRect(time_x, time_y, time_w, time_h));

  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text_color(time_layer, GColorWhite);
  text_layer_set_font(time_layer, time_font);
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);

  layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));

}

static void main_window_unload(Window *window) {

}

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
