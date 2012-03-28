#include "subscriptions.h"
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#define RSSS_URI_MAX 2048

static xmlDocPtr build_new_document(void);

Subscriptions *subscriptions_new(const char *path)
{
  if (path == NULL) return NULL;

  Subscriptions *self = calloc(1, sizeof(Subscriptions));
  if (self == NULL) goto subscription_new_error;

  self->path = strdup(path);
  if (self->path == NULL) goto subscription_new_error;

  // Check if the file exists
  struct stat path_stat;
  int ok = stat(path, &path_stat);

  if (ok == 0) {
    self->subscriptions = xmlParseFile(path);
  }
  else if (errno == ENOENT) {
    // Build an empty OPML file
    self->subscriptions = build_new_document();
  }

  if (self->subscriptions == NULL) goto subscription_new_error;

  return self;

subscription_new_error:
  subscriptions_free(self);
  return NULL;
}

void subscriptions_free(Subscriptions *self)
{
  if (self == NULL) return;

  if (self->path) free(self->path);
  if (self->subscriptions) xmlFreeDoc(self->subscriptions);

  free(self);
}

// Returns the number of bytes written or -1 in case of error.
int subscriptions_sync(Subscriptions *self)
{
  int format = 1; // 1 = indent
  return xmlSaveFormatFileEnc(self->path, self->subscriptions, "utf-8", format);
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
#warning this is wrong, needs to append to body
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


static xmlDocPtr build_new_document(void)
{
  static const char *template = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<opml version=\"1.0\">\n"
    "  <head>\n"
    "    <title>Subscriptions</title>\n"
    "  </head>\n"
    "  <body>\n"
    "  </body>\n"
    "</opml>\n";

  return xmlParseMemory(template, strlen(template));
}



