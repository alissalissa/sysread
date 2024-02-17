#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "lsvlabel.h"
#include "writer.h"

START_TEST(lsvlabel_encode){
	ck_assert(write_lsvlabel("./lsvlabel.sys"));
}END_TEST

START_TEST(lsvlabel_read){
	FILE *handle=fopen("./lsvlabel.sys","rb");
	ck_assert(handle);
	lsvlabel_list_t *list=lsvlabel_list_fnew(handle);
	fail_if(!list);
	if(list)
		ck_assert(lsvlabel_list_destroy(list));
}END_TEST

int main(int argc,char *argv[]){
	Suite *s=suite_create("lsvlabel");
	TCase *writer_tc=tcase_create("write");
	tcase_add_test(writer_tc,lsvlabel_encode);
	TCase *loader_tc=tcase_create("load");
	tcase_add_test(loader_tc,lsvlabel_read);
	suite_add_tcase(s,writer_tc);
	suite_add_tcase(s,loader_tc);
	SRunner *sr=srunner_create(s);
	srunner_run_all(sr,CK_NORMAL);
	int num_failed=srunner_ntests_failed(sr);
	srunner_free(sr);
	return (num_failed==0)?EXIT_SUCCESS:EXIT_FAILURE;
}