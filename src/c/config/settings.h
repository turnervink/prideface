#pragma once

typedef enum {
  CfgKeyFlag,

  CfgKeyCount
} CfgKey;

int current_flag;

void save_settings();
void load_settings();
