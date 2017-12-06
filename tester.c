#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include "list.h"
#include "common.h"

typedef struct list list_t;
typedef struct link link_t;

struct link
{
  elem_t elem;
  link_t *next;
};

struct list
{
  element_free_fun free;
  element_comp_fun compare;
  element_copy_fun copy;
  link_t *first;
  link_t *last;
  int length;
};

void test_list_new(void)
{
  list_t *list = list_new(NULL,NULL,NULL);
  CU_ASSERT_NOT_EQUAL(list,NULL);
}

void test_list_insert(void)
{
  elem_t *first_place = (elem_t*)"first";
  list_t *list = list_new(NULL,NULL,NULL);
  list_insert(list,0,*first_place);
  CU_ASSERT_TRUE(list_length(list) == 1);
  CU_ASSERT_TRUE((elem_t*)list->first == (elem_t*)"first");
}

int main()
{
  //Initialize
  CU_initialize_registry();

  //Set up suites and tests
  CU_pSuite list = CU_add_suite("Test list functions",NULL,NULL);
  CU_add_test(list, "list new", test_list_new);
  CU_add_test(list,"list insert",test_list_insert);

  CU_basic_set_mode(CU_BRM_VERBOSE);

  //Run test
  CU_basic_run_tests();

  CU_cleanup_registry();
  return 0;
}
