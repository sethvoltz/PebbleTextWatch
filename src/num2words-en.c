#include "num2words-en.h"
#include "string.h"

static const char* const ONES[] = {
  "",
  "one",
  "two",
  "three",
  "four",
  "five",
  "six",
  "seven",
  "eight",
  "nine"
};

static const char* const TEENS[] ={
  "",
  "eleven",
  "twelve",
  "thirteen",
  "four",
  "fifteen",
  "six",
  "seven",
  "eight",
  "nine"
};

static const char* const TENS[] = {
  "",
  "ten",
  "twenty",
  "thirty",
  "forty",
  "fifty",
  "sixty",
  "seventy",
  "eighty",
  "ninety"
};

static const char* STR_OH_CLOCK = "o'clock";
static const char* STR_NOON = "noon";
static const char* STR_MID = "mid";
static const char* STR_NIGHT = "night";
static const char* STR_TEEN = "teen";
#if !DAY
static const char* STR_OH = "oh";
#endif

static size_t append_number(char* words, int num) {
  int tens_val = num / 10;
  int ones_val = num % 10;

  size_t len = 0;

  if (tens_val == 1 && num != 10) {
    strcat(words, TEENS[ones_val]);
    return strlen(TEENS[ones_val]);
  }
  strcat(words, TENS[tens_val]);
  len += strlen(TENS[tens_val]);
  if (tens_val < 1) {
    strcat(words, ONES[ones_val]);
    return strlen(ONES[ones_val]);
  }
  return len;
}

static size_t append_string(char* buffer, const size_t length, const char* str) {
  strncat(buffer, str, length);

  size_t written = strlen(str);
  return (length > written) ? written : length;
}


void time_to_words(int hours, int minutes, char* words, size_t length) {

  size_t remaining = length;
  memset(words, 0, length);

  if (hours == 0 || hours == 12) {
    remaining -= append_string(words, remaining, TEENS[2]);
  } else {
    remaining -= append_number(words, hours % 12);
  }

  remaining -= append_string(words, remaining, " ");
  remaining -= append_number(words, minutes);
  remaining -= append_string(words, remaining, " ");
}

void time_to_3words(int hours, int minutes, char *line1, char *line2, char *line3, size_t length)
{
	char value[length];
	time_to_words(hours, minutes, value, length);

	memset(line1, 0, length);
	memset(line2, 0, length);
	memset(line3, 0, length);

	char *start = value;
	char *pch = strstr (start, " ");
	while (pch != NULL) {
		if (line1[0] == 0) {
			memcpy(line1, start, pch-start);
		}  else if (line2[0] == 0) {
			memcpy(line2, start, pch-start);
		} else if (line3[0] == 0) {
			memcpy(line3, start, pch-start);
		}
		start += pch-start+1;
		pch = strstr(start, " ");
	}

	// Truncate long teen values
	if (strlen(line2) > 7) {
		char *pch = strstr(line2, "teen");
		if (pch) {
			memcpy(line3, pch, 4);
			pch[0] = 0;
		}
	}
}

static size_t append_minutes_number(char* words, int num) {
  int ones_val = num % 10;

  size_t len = 0;
  strcat(words, ONES[ones_val]);
  len += strlen(ONES[ones_val]);
  return len;
}

void fuzzy_minutes_to_words(PblTm *t, char* words) {
  int fuzzy_hours = t->tm_hour;
  int fuzzy_minutes = t->tm_min;

  size_t remaining = BUFFER_SIZE;
  memset(words, 0, BUFFER_SIZE);

  //Is it midnight? or noon
  if (fuzzy_minutes != 0 || (fuzzy_hours != 12 && fuzzy_hours != 0)) {
    //is it the top of the hour?
    if(fuzzy_minutes == 0){
      remaining -= append_string(words, remaining, STR_OH_CLOCK);
#if !DAY
    } else if(fuzzy_minutes < 10){     //is it before ten minutes into the hour
      remaining -= append_string(words, remaining, STR_OH);
#endif
    } else {
      remaining -= append_number(words, fuzzy_minutes);
    }
  } else if (fuzzy_hours == 0) {
    remaining -= append_string(words, remaining, STR_NIGHT);
  }
}

void fuzzy_sminutes_to_words(PblTm *t, char* words) {
  int fuzzy_hours = t->tm_hour;
  int fuzzy_minutes = t->tm_min;

  size_t remaining = BUFFER_SIZE;
  memset(words, 0, BUFFER_SIZE);

#if DAY
  if (fuzzy_minutes < 20) {
#else
  if (10 < fuzzy_minutes && fuzzy_minutes < 20) {
#endif
    if (fuzzy_minutes > 13 && 15 != fuzzy_minutes) {
        strcat(words, STR_TEEN);
      }
  } else if (fuzzy_minutes != 0 || (fuzzy_hours != 12 && fuzzy_hours != 0)) {
      remaining -= append_minutes_number(words, fuzzy_minutes);
  }
}

void fuzzy_hours_to_words(PblTm *t, char* words) {
  int fuzzy_hours = t->tm_hour;
  int fuzzy_minutes = t->tm_min;

  size_t remaining = BUFFER_SIZE;
  memset(words, 0, BUFFER_SIZE);

  //Is it midnight?
  if (fuzzy_hours == 0 && fuzzy_minutes == 0) {
    remaining -= append_string(words, remaining, STR_MID);
  //is it noon?
  } else if (fuzzy_hours == 12 && fuzzy_minutes == 0) {
    remaining -= append_string(words, remaining, STR_NOON);
  } else if (fuzzy_hours == 0  || fuzzy_hours == 12){
    remaining -= append_number(words, 12);
  } else {
    //get hour
    remaining -= append_number(words, fuzzy_hours % 12);
  }
}

void fuzzy_dates_to_words(PblTm *t, char* words) {
  string_format_time(words, BUFFER_SIZE, DATE_FORMAT, t);
}
