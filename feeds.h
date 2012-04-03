#ifndef FEEDS_INCLUDED
#define FEEDS_INCLUDED

#include <stdbool.h>
#include "subscriptions.h"
#include "entries.h"

typedef struct {
  Subscriptions *subscriptions;
  Entries *entries;
} Feeds;

Feeds *feeds_new(const char *path);
void feeds_free(Feeds *self);

int feeds_subscribe(Feeds *self, const char *url, const char *title);
bool feeds_unsubscribe(Feeds *self, const char *url);

int feeds_sync(Feeds *self);

#endif
