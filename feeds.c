#include "feeds.h"
#include <stdio.h>
#include <limits.h>
#include <assert.h>

static int get_subscriptions_path(const char *basepath, char *buffer, size_t len)
{
  assert(basepath != NULL);
  assert(buffer != NULL);
  return snprintf(buffer, len, "%s_subscriptions.opml", basepath);
}

// static int get_entries_path(const char *basepath, char *buffer, size_t len)
// {
//   assert(basepath != NULL);
//   assert(buffer != NULL);
//   return snprintf(buffer, len, "%s_entries.xml", basepath);
// }

Feeds *feeds_new(const char *path)
{
  char subscriptions_path[PATH_MAX];
  //entries_path[PATH_MAX]
  Feeds *self = calloc(1, sizeof(Feeds));
  if (self == NULL) goto bail;

  if (get_subscriptions_path(path, subscriptions_path, PATH_MAX) >= PATH_MAX) goto bail;
  self->subscriptions = subscriptions_new(subscriptions_path);
  if (self->subscriptions == NULL) goto bail;

  // self->subscriptions = subscriptions_new(subscriptions_path(path, subscriptions_path, PATH_MAX));
  // if (entries == NULL) goto bail;

  return self;

bail:
  feeds_free(self);
  return NULL;
}

void feeds_free(Feeds *self)
{
  if (self == NULL) return;

  // if (self->entries != NULL) entries_free(self->entries);
  if (self->subscriptions != NULL) subscriptions_free(self->subscriptions);

  free(self);

}

int feeds_subscribe(Feeds *self, const char *url, const char *title)
{
  Subscription *subscription = subscriptions_add(self->subscriptions, url, title);
  return subscription != NULL;
}

int feeds_unsubscribe(Feeds *self, const char *url)
{
  return subscriptions_remove(self->subscriptions, url);
}
