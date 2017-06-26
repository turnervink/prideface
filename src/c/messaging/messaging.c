#include <pebble.h>
#include "../prideface.h"
#include "messaging.h"
#include "../config/settings.h"

static void inbox_received_handler(DictionaryIterator *iter, void *ctx) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Received messageKeys");

  Tuple *t = dict_read_first(iter);

  while (t) {

    int true_key = t->key - 10000;

    if (true_key == CfgKeyFlag) {
      current_flag = atoi(t->value->cstring);
      APP_LOG(APP_LOG_LEVEL_INFO, "Flag is %d", current_flag);
      update_flag();
    }

    t = dict_read_next(iter);

  }

  save_settings();

}

void inbox_failed_handler(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Incoming message dropped! - %d", reason);
}

void outbox_sent_handler(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

void outbox_failed_handler(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

void init_messaging() {
  app_message_register_inbox_received(inbox_received_handler);
  app_message_register_inbox_dropped(inbox_failed_handler);
  app_message_register_outbox_sent(outbox_sent_handler);
  app_message_register_outbox_failed(outbox_failed_handler);
  app_message_open(256, 256);
}
