//Implementation for test data writer
#include "writer.h"

bool write_mrset(const char *path){
	//declare the stream data
	char mc_test_string[]="$abcde=C 10 my mcgroup a b c";
	char md_test_string1[]="$b=D 55 0  g e f d";
	char md_test_string2[]="$d=E 1 2 34 13 third mdgroup k l m";
	char md_test_string3[]="$e=E 11 6 choice 0  n o p";

	const int32_t writer_rec_type=7;
	const int32_t writer_MDCV_subtype=19;
	const int32_t writer_subtype=7;
	const int32_t writer_size=1;

	FILE *handle=fopen(path,"w");
	if(!handle) return false;
	fwrite(&writer_rec_type,sizeof(writer_rec_type),1,handle);
	fwrite(&writer_MDCV_subtype,sizeof(writer_MDCV_subtype),1,handle);
	fwrite(&writer_size,sizeof(writer_size),1,handle);
	const int32_t total_count=28+18+34+25+3;
	fwrite(&total_count,sizeof(total_count),1,handle);

	fwrite(mc_test_string,sizeof(char),28,handle);
	fputc(0x0a,handle);
	fwrite(md_test_string1,sizeof(char),18,handle);
	fputc(0x0a,handle);
	fwrite(md_test_string2,sizeof(char),34,handle);
	fputc(0x0a,handle);
	fwrite(md_test_string3,sizeof(char),25,handle);
	if(ferror(handle)){
		fclose(handle);
		return false;
	}
	fclose(handle);
	return true;
}