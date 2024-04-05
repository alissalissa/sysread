//Unit tests for long string missing values record
#include <check.h>

#include "lsmissing.h"
#include "writer.h"

START_TEST(lsmissing_encode){
	ck_assert(write_lsmissing("./lsmissing.sys"));
}END_TEST

START_TEST(lsmissing_read){
	FILE *handle=fopen("./lsmissing.sys","rb");
	ck_assert(handle);
	lsmvr_t *record=lsmvr_fnew(handle);
	fail_if(!record);
	if(record)
		ck_assert(lsmvr_destroy(record));
}END_TEST

int main(int argc,char *argv[]){
	Suite *s=suite_create("lsmissing");
	TCase *writer_tc=tcase_create("write");
	tcase_add_test(writer_tc,lsmissing_encode);
	TCase *loader_tc=tcase_create("load");
	tcase_add_test(loader_tc,lsmissing_read);
	suite_add_tcase(s,writer_tc);
	suite_add_tcase(s,loader_tc);
	SRunner *sr=srunner_create(s);
	srunner_run_all(sr,CK_NORMAL);
	int num_failed=srunner_ntests_failed(sr);
	srunner_free(sr);
	return (num_failed==0)?EXIT_SUCCESS:EXIT_FAILURE;
}