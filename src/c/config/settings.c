#include <pebble.h>
#include "settings.h"

#define STORED_SETTINGS 5555

void save_settings() {

  persist_write_bool(STORED_SETTINGS, true);

  for (int i = 0; i < CfgKeyCount; i++) {

    if (i == CfgKeyFlag) {
      persist_write_int(MESSAGE_KEY_CfgKeyFlag, current_flag);
    }

  }

}

void load_settings() {

  if (persist_exists(STORED_SETTINGS)) {

    // Load stored settings
    for (int i = 0; i < CfgKeyCount; i++) {

      if (i == CfgKeyFlag) {
        current_flag = persist_read_int(MESSAGE_KEY_CfgKeyFlag);
      }

    }

  } else {
    current_flag = 1;
  }

}
