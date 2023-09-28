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

bool write_epir(const char *path){
	char test_data[]="TESTING INFO";
	const int32_t type=7;
	const int32_t subtype=10;
	const int32_t size=1;
	const int32_t count=12;
	FILE *handle=fopen(path,"wb");
	if(!handle) return false;
	fwrite(&type,sizeof(int32_t),1,handle);
	fwrite(&subtype,sizeof(int32_t),1,handle);
	fwrite(&size,sizeof(int32_t),1,handle);
	fwrite(&count,sizeof(int32_t),1,handle);
	fwrite(test_data,size,count,handle);
	fclose(handle);
	return true;
}

bool write_disppar(const char *path){
	int32_t rec_type=7;
	int32_t subtype=11;
	int32_t size=4;
	int32_t count=12;
	int32_t measures[]={
		MEASURE_NOMINAL,
		MEASURE_ORDINAL,
		MEASURE_SCALE,
		MEASURE_UNKNOWN
	};
	int32_t widths[]={
		5,
		10,
		20,
		30
	};
	int32_t alignments[]={
		ALIGN_LEFT,
		ALIGN_CENTER,
		ALIGN_RIGHT,
		ALIGN_CENTER
	};
	FILE *handle=fopen(path,"w");
	if(!handle) return false;

	fwrite(&rec_type,sizeof(int32_t),1,handle);
	fwrite(&subtype,sizeof(int32_t),1,handle);
	fwrite(&size,sizeof(int32_t),1,handle);
	fwrite(&count,sizeof(int32_t),1,handle);
	for(int i=0;i<count/3;i++){
		fwrite(&(measures[i]),sizeof(int32_t),1,handle);
		fwrite(&(widths[i]),sizeof(int32_t),1,handle);
		fwrite(&(alignments[i]),sizeof(int32_t),1,handle);
	}
	if(ferror(handle)){
		fclose(handle);
		return false;
	}
	fclose(handle);
	return true;
}

bool write_svset(const char *path){
	char substance[]="one= a b c d\ntwo= e f g h\nthree= i j k l m n o p";
	FILE *handle=fopen(path,"w");
	if(!handle) return false;
	int32_t rec_type=SVSET_TYPE;
	int32_t subtype=SVSET_SUBTYPE;
	int32_t byte_size=SYSVSET_BYTE_SIZE;
	int32_t count=48;
	fwrite(&rec_type,sizeof(int32_t),1,handle);
	fwrite(&subtype,sizeof(int32_t),1,handle);
	fwrite(&byte_size,sizeof(int32_t),1,handle);
	fwrite(&count,sizeof(int32_t),1,handle);
	fwrite(substance,sizeof(char),count,handle);
	if(ferror(handle)){
		fclose(handle);
		return false;
	}
	fclose(handle);
	return true;
}

bool write_lvnr(const char *path){
	FILE *handle=fopen(path,"wb");
	if(!handle) return false;
	int32_t rec_type=7;
	int32_t subtype=13;
	char pair_one[]="foo=bar";
	char pair_two[]="foobar=test";
	fwrite(&rec_type,sizeof(int32_t),1,handle);
	if(ferror(handle)){
		fclose(handle);
		return false;
	}
	fwrite(&subtype,sizeof(int32_t),1,handle);
	if(ferror(handle)){
		fclose(handle);
		return false;
	}
	int32_t count=19;
	fwrite(&count,sizeof(int32_t),1,handle);
	if(ferror(handle)){
		fclose(handle);
		return false;
	}
	fwrite(pair_one,sizeof(char),7,handle);
	if(ferror(handle)){
		fclose(handle);
		return false;
	}
	fputc(0x09,handle);
	if(ferror(handle)){
		fclose(handle);
		return false;
	}
	fwrite(pair_two,sizeof(char),11,handle);
	if(ferror(handle)){
		fclose(handle);
		return false;
	}
	fclose(handle);
	return true;
}
