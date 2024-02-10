#include <stdio.h>

#include <check.h>

#include "lvnr.h"
#include "writer.h"

START_TEST(lvnr_parse){
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

int main(int argc,char *argv[]){
	Suite *s=suite_create("lvnr");
	TCase *tc_core=tcase_create("core");
	tcase_add_test(tc_core,lvnr_parse);
	suite_add_tcase(s,tc_core);
	SRunner *sr=srunner_create(s);
	srunner_run_all(sr,CK_NORMAL);
	int number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}