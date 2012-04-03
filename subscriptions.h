#ifndef SUBSCRIPTIONS_INCLUDED
#define SUBSCRIPTIONS_INCLUDED

#include <stdbool.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

typedef struct {
  char *path;
  xmlDocPtr subscriptions;
} Subscriptions;

typedef struct {
  xmlNodePtr node;
// TODO: Handle folders
} Subscription;

Subscriptions *subscriptions_new(const char *path);
void subscriptions_free(Subscriptions *self);
int subscriptions_sync(Subscriptions *self);

void subscription_free(Subscription *self);

Subscription *subscriptions_add(Subscriptions *self, const char *url, const char *title);
void subscriptions_remove(Subscriptions *self, Subscription *subscription);
Subscription *subscriptions_find(Subscriptions *self, const char *url);

const char *subscription_get_attr(Subscription *self, const char *attr);

#endif
