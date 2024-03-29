#include "check_subscriptions.h"
#include <unistd.h>
#include "../subscriptions.h"

START_TEST (test_subscriptions_new)
{
  // Test creation without a file
  Subscriptions *subscriptions = subscriptions_new(NULL);
  fail_unless(subscriptions == NULL, "controller is not NULL");
  subscriptions_free(subscriptions);

  // Test creation with an existing file
  subscriptions = subscriptions_new("test/fixtures/google-reader-subscriptions.xml");
  fail_if(subscriptions == NULL, "controller is NULL");
  subscriptions_free(subscriptions);

  // Test creation with a not yet existing file
  char tmp[] = "/tmp/check_subscriptionsXXXXXX";
  fail_if(mktemp(tmp) == NULL, "Unable to create temp file");

  subscriptions = subscriptions_new(tmp);
  fail_if(subscriptions == NULL, "controller is NULL");
  subscriptions_free(subscriptions);
}
END_TEST

START_TEST (test_subscriptions_sync)
{
  char tmp[] = "/tmp/check_subscriptionsXXXXXX";
  fail_if(mktemp(tmp) == NULL, "Unable to generate temp file path");

  Subscriptions *subscriptions = subscriptions_new(tmp);
  fail_if(subscriptions == NULL, "controller is NULL");
  fail_if(subscriptions_sync(subscriptions) < 0, "Error syncing subscriptions");

#warning should verify the content of the file

  unlink(tmp);
  subscriptions_free(subscriptions);
}
END_TEST

START_TEST (test_subscriptions_add)
{
  char tmp[] = "/tmp/check_subscriptionsXXXXXX";
  fail_if(mktemp(tmp) == NULL, "Unable to generate temp file path");

  // Create a new document
  Subscriptions *subscriptions = subscriptions_new(tmp);
  fail_if(subscriptions == NULL, "controller is NULL");

  // add a subscription
  Subscription *subscription = subscriptions_add(
    subscriptions,
    "http://test.example.com/feed",
    "RSSS Test Feed"
  );
  fail_if(subscription == NULL, "Subscription is NULL");

  // then save
  fail_if(subscriptions_sync(subscriptions) < 0, "Error syncing subscriptions");

  // Now re-open and find the feed that was added
  subscriptions_free(subscriptions);
  subscriptions = subscriptions_new(tmp);
  fail_if(subscriptions == NULL, "controller is NULL");

  subscription = subscriptions_find(subscriptions, "http://test.example.com/feed");
  fail_if(subscription == NULL, "Unable to find subscription");

  subscriptions_free(subscriptions);
  unlink(tmp);
}
END_TEST

START_TEST (test_subscriptions_find)
{
  Subscriptions *subscriptions = subscriptions_new("test/fixtures/google-reader-subscriptions.xml");
  fail_if(subscriptions == NULL, "controller is NULL");

  // Attempt to find subscription that doesn't exist
  Subscription *feed = subscriptions_find(subscriptions, "http://example.com/not-here");
  fail_unless(feed == NULL, "Expected no subscription, but got subscription");

  // Find one that does exist
  feed = subscriptions_find(subscriptions, "http://daringfireball.net/index.xml");
  fail_if(feed == NULL, "Unable to find subscription");

  subscription_free(feed);
  subscriptions_free(subscriptions);
}
END_TEST

START_TEST (test_subscriptions_remove)
{
  Subscriptions *subscriptions = subscriptions_new("test/fixtures/test_subscriptions.opml");
  fail_if(subscriptions == NULL, "controller is NULL");

  Subscription *feed = subscriptions_find(subscriptions, "http://hivelogic.com/combined.rss");
  fail_if(feed == NULL, "Unable to find subscription");

  subscriptions_remove(subscriptions, feed);
  subscription_free(feed);

  // Now the feed should not exist
  feed = subscriptions_find(subscriptions, "http://hivelogic.com/combined.rss");
  fail_unless(feed == NULL, "Found removed subscription");

  subscriptions_free(subscriptions);
}
END_TEST

START_TEST (test_subscription_get_attr)
{
  Subscriptions *subscriptions = subscriptions_new("test/fixtures/google-reader-subscriptions.xml");
  fail_if(subscriptions == NULL, "controller is NULL");

  Subscription *feed = subscriptions_find(subscriptions, "http://daringfireball.net/index.xml");
  fail_if(subscriptions == NULL, "Unable to find subscription");

  // Retrieve attributes
  ck_assert_str_eq("rss",             subscription_get_attr(feed, "type"));
  ck_assert_str_eq("Daring Fireball", subscription_get_attr(feed, "text"));
  ck_assert_str_eq("Daring Fireball", subscription_get_attr(feed, "title"));
  ck_assert_str_eq("http://daringfireball.net/index.xml", subscription_get_attr(feed, "xmlUrl"));
  ck_assert_str_eq("http://daringfireball.net/",          subscription_get_attr(feed, "htmlUrl"));

  fail_if(subscription_get_attr(feed, "missing") != NULL, "Value is not NULL for missing attribute");

  subscription_free(feed);
  subscriptions_free(subscriptions);
}
END_TEST

// Test list subscriptions

// Test update subscription

Suite *subscriptions_suite()
{
  Suite *s = suite_create ("Subscriptions Manager");

  /* Core test case */
  TCase *tc_core = tcase_create ("Core");
  tcase_add_test (tc_core, test_subscriptions_new);
  tcase_add_test (tc_core, test_subscriptions_find);
  tcase_add_test (tc_core, test_subscriptions_sync);
  tcase_add_test (tc_core, test_subscriptions_add);
  tcase_add_test (tc_core, test_subscriptions_remove);
  tcase_add_test (tc_core, test_subscription_get_attr);
  suite_add_tcase (s, tc_core);

  return s;
}
