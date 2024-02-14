#include <stdio.h>

#include <check.h>

#include "lvnr.h"
#include "writer.h"

START_TEST(lvnr_create){
	char path[]="./lvnr.sys";
	ck_assert(write_lvnr(path));
	FILE *handle=fopen(path,"rb");
	ck_assert(handle);
	lvnr_t *foo=lvnr_fnew(handle);
	fclose(handle);
	ck_assert(foo);
	if(foo)
		ck_assert(lvnr_destroy(foo));
}
END_TEST

START_TEST(lvnr_values){
	char path[]="./lvnr.sys";
	ck_assert(write_lvnr(path));
	FILE *handle=fopen(path,"rb");
	ck_assert(handle);
	lvnr_t *foo=lvnr_fnew(handle);
	fclose(handle);
	ck_assert_int_eq(foo->rec_type,7);
	ck_assert_int_eq(foo->subtype,13);
	ck_assert_int_gt(foo->count,0);
	ck_assert(foo->constructed);
	ck_assert(foo->keys);
	ck_assert(foo->values);
	ck_assert(lvnr_destroy(foo));
}
END_TEST

int main(int argc,char *argv[]){
	Suite *s=suite_create("lvnr");
	TCase *tc_create=tcase_create("create");
	tcase_add_test(tc_create,lvnr_create);
	TCase *tc_values=tcase_create("values");
	tcase_add_test(tc_values,lvnr_values);
	suite_add_tcase(s,tc_create);
	suite_add_tcase(s,tc_values);
	SRunner *sr=srunner_create(s);
	srunner_run_all(sr,CK_NORMAL);
	int number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}