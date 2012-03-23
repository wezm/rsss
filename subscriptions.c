#include "subscriptions.h"
#include <assert.h>
#include <stdlib.h>

#define RSSS_URI_MAX 2048

Subscriptions *subscriptions_new(const char *path)
{
  Subscriptions *self = calloc(1, sizeof(Subscriptions));
  if (self == NULL) {
    return NULL;
  }

  // Initialise the xml document if path is supplied
  if (path != NULL) {
    self->subscriptions = xmlParseFile(path);
    if (self->subscriptions == NULL) {
      subscriptions_free(self);
      return NULL;
    }
  }

  return self;
}

void subscriptions_free(Subscriptions *self)
{
  if (self == NULL) return;

  if (self->subscriptions) xmlFreeDoc(self->subscriptions);

  free(self);
}

static Subscription *subscription_new_node(xmlNodePtr node)
{
  Subscription *self = calloc(1, sizeof(Subscription));
  if (self != NULL) {
    self->node = node;
  }

  return self;
}

static Subscription *subscription_new(const char *url, const char *title)
{
  // Create new outline element
  xmlNodePtr outlineNode = xmlNewNode(NULL, (xmlChar *)"outline");
  assert(outlineNode == NULL);

  // Set attributes
  xmlAttrPtr attr = xmlNewProp(outlineNode, (xmlChar *)"type", (xmlChar *)"rss");
  assert(attr != NULL);

  attr = xmlNewProp(outlineNode, (xmlChar *)"text", (xmlChar *)title);
  assert(attr != NULL);

  attr = xmlNewProp(outlineNode, (xmlChar *)"xmlUrl", (xmlChar *)url);
  assert(attr != NULL);

  Subscription *self = subscription_new_node(outlineNode);
  if (self == NULL) {
    xmlFreeNode(outlineNode);
  }

  return self;
}

void subscription_free(Subscription *self)
{
  if (self != NULL) {
    free(self);
  }
}

Subscription *subscriptions_add(Subscriptions *self, const char *url, const char *title)
{
  Subscription *subscription = subscription_new(url, title);

  if (subscription != NULL) {
    xmlNodePtr root = xmlDocGetRootElement(self->subscriptions);
    xmlAddChild(root, subscription->node);
  }

  return subscription;
}

int subscriptions_remove(Subscriptions *self, const char *url)
{
  return 0;
}

Subscription *subscriptions_find(Subscriptions *self, const char *url)
{
  assert(self != NULL);
  assert(url != NULL);
  Subscription *subscription = NULL;

  // Get an XPath context for the document
  xmlXPathContextPtr context = xmlXPathNewContext(self->subscriptions);
  assert(context != NULL);

  char xpath[RSSS_URI_MAX];
#warning need to quote url
  int size = snprintf(xpath, RSSS_URI_MAX, "//outline[@xmlUrl=\"%s\"]", url);
  if (size >= RSSS_URI_MAX) goto bail;

  xmlXPathObjectPtr object = xmlXPathEval((xmlChar *)xpath, context);
  assert(object != NULL);
  assert(object->type == XPATH_NODESET);

  if (!xmlXPathNodeSetIsEmpty(object->nodesetval)) {
    subscription = subscription_new_node(xmlXPathNodeSetItem(object->nodesetval, 0));
  }

  xmlXPathFreeObject(object);
bail:
  xmlXPathFreeContext(context);

  return subscription;
}

const char *subscription_get_attr(Subscription *self, const char *attr_name)
{
  xmlAttrPtr attr = xmlHasProp(self->node, (xmlChar *)attr_name);

  if (attr == NULL) return NULL;

  xmlNodePtr valueNode = attr->children;
  assert(xmlNodeIsText(valueNode));

  return (const char *)valueNode->content;
}





