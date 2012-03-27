#include <stdlib.h>
#include <stdio.h>
// #include <Eina.h>
// #include <sys/types.h>
// #include <sys/stat.h>
#include <check.h>

// #include "../rsss.h"
#include "check_subscriptions.h"
#include "check_feeds.h"
// #include "check_datastore.h"
// #include "check_xml_parser.h"
// #include "check_feed_parser.h"
// #include "check_collection_parser.h"

int main(int argc, char * const *argv)
{
  int number_failed;

  // if (!backpage_init(EINA_LOG_LEVEL_INFO)) {
  //   fprintf(stderr, "Error calling backpage_init\n");
  //   return EXIT_FAILURE;
  // }

  SRunner *sr = srunner_create(subscriptions_suite());
  srunner_add_suite(sr, feeds_suite());
  // srunner_add_suite(sr, xml_parser_suite());
  // srunner_add_suite(sr, feed_parser_suite());
  // srunner_add_suite(sr, collection_parser_suite());
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  // backpage_shutdown();

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
