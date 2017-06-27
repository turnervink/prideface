#include <pebble.h>
#include "prideface.h"
#include "messaging/messaging.h"
#include "config/settings.h"

static Window *main_window;
static Layer *flag, *time_band;

extern int *flags[];
extern int flag_segments[];

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
  int band_x = (bounds.size.w / 2) - 38;
  int band_y = 0;
  int band_w = 76;
  int band_h = bounds.size.h;
  GRect band = GRect(band_x, band_y, band_w, band_h);
  GRect band_stroke = GRect(band_x, band_y - 2, band_w, band_h + 4);

  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_context_set_stroke_width(ctx, 3);
  graphics_fill_rect(ctx, band, 0, GCornerNone);
  graphics_draw_rect(ctx, band_stroke);
}

static void main_window_load(Window *window) {

  GRect bounds = layer_get_bounds(window_get_root_layer(window));

  flag = layer_create(bounds);
  layer_set_update_proc(flag, flag_update_proc);

  time_band = layer_create(bounds);
  layer_set_update_proc(time_band, time_band_update_proc);

  layer_add_child(window_get_root_layer(window), flag);
  layer_add_child(window_get_root_layer(window), time_band);

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
  // tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

}

static void deinit() {

  window_destroy(main_window);
  // tick_timer_service_unsubscribe();

}

int main(void) {

  init();
  app_event_loop();
  deinit();

}
