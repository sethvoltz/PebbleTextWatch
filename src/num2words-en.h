#pragma once
#include "pebble_os.h"
#include "pebble_app.h"
#include "string.h"

void fuzzy_minutes_to_words(PblTm *t, char* words, size_t length);
void fuzzy_hours_to_words(PblTm *t, char* words, size_t length);
void fuzzy_sminutes_to_words(PblTm *t, char* words, size_t length);
void fuzzy_dates_to_words(PblTm *t, char* words, size_t length);
