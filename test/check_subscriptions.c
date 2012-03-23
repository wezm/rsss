#include "check_subscriptions.h"
#include "../subscriptions.h"

START_TEST (test_subscriptions_new)
{
  // Test creation without a file
  Subscriptions *subscriptions = subscriptions_new(NULL);
  fail_if(subscriptions == NULL, "controller is NULL");
  subscriptions_free(subscriptions);

  // Test creation with a file
  subscriptions = subscriptions_new("test/fixtures/google-reader-subscriptions.xml");
  fail_if(subscriptions == NULL, "controller is NULL");
  subscriptions_free(subscriptions);

#warning add test for file that doesn't exist
}
END_TEST

// START_TEST (test_subscriptions_add)
// {
//   subscriptions = subscriptions_new("test/fixtures/google-reader-subscriptions.xml");
//   fail_if(subscriptions == NULL, "controller is NULL");
//   subscriptions_free(subscriptions);
// }
// END_TEST

/*

  <outline text="Daring Fireball" title="Daring Fireball"
      type="rss" xmlUrl="http://daringfireball.net/index.xml" htmlUrl="http://daringfireball.net/"/>

*/
START_TEST (test_subscriptions_find)
{
  Subscriptions *subscriptions = subscriptions_new("test/fixtures/google-reader-subscriptions.xml");
  fail_if(subscriptions == NULL, "controller is NULL");

  Subscription *feed = subscriptions_find(subscriptions, "http://daringfireball.net/index.xml");
  fail_if(subscriptions == NULL, "Unable to find subscription");

  subscription_free(feed);
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

// Test delete subscription

// Test update subscription

Suite *subscriptions_suite()
{
  Suite *s = suite_create ("Subscriptions Manager");

  /* Core test case */
  TCase *tc_core = tcase_create ("Core");
  tcase_add_test (tc_core, test_subscriptions_new);
  tcase_add_test (tc_core, test_subscriptions_find);
  tcase_add_test (tc_core, test_subscription_get_attr);
  // tcase_add_test (tc_core, test_subscriptions_parse_empty);
  // tcase_add_test (tc_core, test_subscriptions_parse_simple);
  suite_add_tcase (s, tc_core);

  return s;
}