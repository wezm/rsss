#include "check_feeds.h"
#include <unistd.h>
#include "../feeds.h"

START_TEST (test_feeds_new)
{
  // Test creation with a file
  Feeds *feeds = feeds_new("test/fixtures/google-reader-feeds.xml");
  fail_if(feeds == NULL, "controller is NULL");
  feeds_free(feeds);

#warning add test for file that doesn't exist
}
END_TEST

START_TEST (test_feeds_subscribe)
{
  char tmp[] = "/tmp/check_feedsXXXXXX";
  fail_if(mktemp(tmp) == NULL, "Unable to generate temp file path");

  Feeds *feeds = feeds_new(tmp);
  fail_if(feeds == NULL, "controller is NULL");

  fail_if(!feeds_subscribe(feeds, "http://example.com/feed.rss", "Example Feed"));

  feeds_free(feeds);
  unlink(tmp);
}
END_TEST

START_TEST (test_feeds_sync)
{
  char tmp[] = "/tmp/check_feedsXXXXXX";
  fail_if(mktemp(tmp) == NULL, "Unable to generate temp file path");

  Feeds *feeds = feeds_new(tmp);
  fail_if(feeds == NULL, "controller is NULL");

  fail_if(!feeds_subscribe(feeds, "http://example.com/feed.rss", "Example Feed"));

  fail_if(feeds_sync(feeds) < 0, "Error syncing feeds");

  feeds_free(feeds);
  unlink(tmp);
}
END_TEST

Suite *feeds_suite()
{
  TCase *tc_core = tcase_create ("Core");
  tcase_add_test (tc_core, test_feeds_new);
  tcase_add_test (tc_core, test_feeds_subscribe);
  tcase_add_test (tc_core, test_feeds_sync);

  Suite *s = suite_create ("Feeds Manager");
  suite_add_tcase (s, tc_core);

  return s;
}
