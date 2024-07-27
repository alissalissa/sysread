//
// Created by alissa on 7/27/24.
//
#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "dfvar.h"
#include "writer.h"

START_TEST(dfvar_encode){
		ck_assert(write_dfvar("./dfvar.sys"));
}END_TEST

START_TEST(dfvar_read){
	FILE *handle=fopen("./dfvar.sys","rb");
	ck_assert(handle);
	dfvar_t *list=dfvar_fnew(handle);
	fail_if(!list);
	if(list)
		ck_assert(dfvar_destroy(list));
}END_TEST

int main(int argc,char *argv[]){
	Suite *s=suite_create("dfvar");
	TCase *writer_tc=tcase_create("write");
	tcase_add_test(writer_tc,dfvar_encode);
	TCase *loader_tc=tcase_create("load");
	tcase_add_test(loader_tc,dfvar_read);
	suite_add_tcase(s,writer_tc);
	suite_add_tcase(s,loader_tc);
	SRunner *sr=srunner_create(s);
	srunner_run_all(sr,CK_NORMAL);
	int num_failed=srunner_ntests_failed(sr);
	srunner_free(sr);
	return (num_failed==0)?EXIT_SUCCESS:EXIT_FAILURE;
}