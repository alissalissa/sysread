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

//TODO add writing for a header test sysfile
bool write_header(const char *path){
	assert(path);
	bstream_t *rec_type=bstream_new_wl(REC_TYPE_SIZE);
	bstream_t *prod_name=bstream_new_wl(PROD_NAME_SIZE);
	bstream_t *creation_date=bstream_new_wl(CREATION_DATE_SIZE);
	bstream_t *creation_time=bstream_new_wl(CREATION_TIME_SIZE);
	bstream_t *file_label=bstream_new_wl(FILE_LABEL_SIZE);
	bstream_t *padding=bstream_new_wl(PADDING_SIZE);
	strcpy(rec_type->stream,"$FL2\0");
	strcpy(prod_name->stream,"@(#) SPSS DATA FILETESTTESTTESTTEST\0");
	for(int i=35;i<PROD_NAME_SIZE;i++)
		prod_name->stream[i]=' ';
	int32_t layout_code=2;
	int32_t nominal_case_size=50;
	int32_t compression=0;
	int32_t weight_index=0;
	int32_t ncases=-1;
	float bias=100.0;
	strcpy(creation_date->stream,"01 Jan 77\0");
	strcpy(creation_time->stream,"00:00:00\0");
	strcpy(file_label->stream,"Test file\0");
	for(int i=9;i<FILE_LABEL_SIZE;i++)
		file_label->stream[i]=' ';
	for(int i=0;i<PADDING_SIZE;i++)
		padding->stream[i]=0x00;

	FILE *output=fopen(path,"wb");
	if(!output){
		bstream_mass_destroy(6,rec_type,prod_name,creation_date,creation_time,file_label,padding);
		return false;
	}
	fwrite(rec_type->stream,sizeof(char),REC_TYPE_SIZE,output);
	fwrite(prod_name->stream,sizeof(char),PROD_NAME_SIZE,output);
	fwrite(&layout_code,sizeof(int32_t),1,output);
	fwrite(&nominal_case_size,sizeof(int32_t),1,output);
	fwrite(&compression,sizeof(int32_t),1,output);
	fwrite(&weight_index,sizeof(int32_t),1,output);
	fwrite(&ncases,sizeof(int32_t),1,output);
	fwrite(&bias,sizeof(float),1,output);
	fwrite(creation_date->stream,sizeof(char),CREATION_DATE_SIZE,output);
	fwrite(creation_time->stream,sizeof(char),CREATION_TIME_SIZE,output);
	fwrite(file_label->stream,sizeof(char),FILE_LABEL_SIZE,output);
	fwrite(padding->stream,sizeof(char),PADDING_SIZE,output);
	fclose(output);
	return true;
}

bool write_vlsr(const char *path){
	assert(path);
	int32_t record_type=(int32_t)VLSR_RECORD_TYPE;
	int32_t subtype=(int32_t)VLSR_SUBTYPE;
	int32_t byte_size=1;
	const char *half_one="foo=bar";
	const char *half_two="foobar=fooestbar";
	int32_t count=25;
	FILE *handle=fopen(path,"wb");
	if(!handle)
		return false;
	fwrite(&record_type,sizeof(int32_t),1,handle);
	fwrite(&subtype,sizeof(int32_t),1,handle);
	fwrite(&byte_size,sizeof(int32_t),1,handle);
	fwrite(&count,sizeof(int32_t),1,handle);
	fwrite(half_one,sizeof(char),7,handle);
	fputc(0x00,handle);
	fputc(0x09,handle);
	fwrite(half_two,sizeof(char),16,handle);
	fclose(handle);
	return true;
}

bool write_lsvlabel(const char *path){
	//Open file
	FILE *output=fopen(path,"wb");
	assert(output);
	
	//Write the header
	const int32_t rec_type=LSVLABEL_RECORD_TYPE;
	const int32_t subtype=LSVLABEL_SUBTYPE;
	const int32_t size=1;
	fwrite(&rec_type,sizeof(int32_t),1,output);
	fwrite(&subtype,sizeof(int32_t),1,output);
	fwrite(&size,sizeof(int32_t),1,output);

	//Compile the data for the record so that we can eventually compute its size
	char var_one_name[]="foo\0";
	int32_t var_one_name_length=strlen(var_one_name);
	char var_two_name[]="bar\0";
	int32_t var_two_name_length=strlen(var_two_name);
	char *var_one_labels[]={
		"test1\0",
		"test2\0"
	};
	char *var_one_values[]={
		"testvalue1\0",
		"testvalue2\0"
	};
	char *var_two_labels[]={
		"test3\0",
		"test4\0"
	};
	char *var_two_values[]={
		"testvalue3\0",
		"testvalue4\0"
	};
	int32_t n_labels=2;
	int32_t var_width=strlen(var_one_values[0]);
	int32_t label_length=strlen(var_two_labels[0]);

	int32_t count=strlen(var_one_name)+strlen(var_two_name);
	count+=(sizeof(int32_t)*2); //var_name_len
	count+=((sizeof(int32_t)*2)*2); //var_width and n_labels
	count+=((sizeof(int32_t)*4)*2); //value_len and label_len
	count+=(strlen(var_one_labels[0]) + strlen(var_one_labels[1]) + strlen(var_one_values[0]) + strlen(var_one_values[1]));
	count+=(strlen(var_two_labels[0]) + strlen(var_two_labels[1]) + strlen(var_two_values[0]) + strlen(var_two_values[1]));

	fwrite(&count,sizeof(int32_t),1,output);
	fwrite(&var_one_name_length,sizeof(int32_t),1,output);
	fwrite(var_one_name,sizeof(char),strlen(var_one_name),output);
	fwrite(&var_width,sizeof(int32_t),1,output);
	fwrite(&n_labels,sizeof(int32_t),1,output);
	for(int i=0;i<=1;i++){
		fwrite(&var_width,sizeof(int32_t),1,output);
		fwrite(var_one_values[i],sizeof(char),var_width,output);
		fwrite(&label_length,sizeof(int32_t),1,output);
		fwrite(var_one_labels[i],sizeof(char),label_length,output);
	}

	fwrite(&var_two_name_length,sizeof(int32_t),1,output);
	fwrite(var_two_name,sizeof(char),strlen(var_two_name),output);
	fwrite(&var_width,sizeof(int32_t),1,output);
	fwrite(&n_labels,sizeof(int32_t),1,output);
	for(int i=0;i<=1;i++){
		fwrite(&var_width,sizeof(int32_t),1,output);
		fwrite(var_two_values[i],sizeof(char),var_width,output);
		fwrite(&label_length,sizeof(int32_t),1,output);
		fwrite(var_two_labels[i],sizeof(char),label_length,output);
	}

	if(ferror(output)){
		fclose(output);
		return false;
	}
	fclose(output);
	return true;
}

bool write_lsmissing(const char *path){
	const int32_t record_type=LSMVR_RECORD_TYPE;
	const int32_t subtype=LSMVR_SUBTYPE;
	const int32_t size_check=1;

	bstream_t *var_one_name=bstream_char_new("foo",3);
	bstream_t *var_two_name=bstream_char_new("foobar",6);
	const char n_missing_values=2;
	const int32_t value_length=8;
	char *values_one[]={
		"abcdefgh",
		"ijklmnop"
	};
	char *values_two[]={
		"qrstuvwx",
		"yzabcdef"
	};

	int32_t count=var_one_name->length+var_two_name->length;
	count+=sizeof(int32_t)*4;
	count+=sizeof(char)*2;
	count+=32;

	FILE *output=fopen(path,"wb");
	if(!output)
		return false;

	fwrite(&record_type,sizeof(int32_t),1,output);
	fwrite(&subtype,sizeof(int32_t),1,output);
	fwrite(&size_check,sizeof(int32_t),1,output);
	fwrite(&count,sizeof(int32_t),1,output);

	fwrite(&var_one_name->length,sizeof(int32_t),1,output);
	fwrite(var_one_name->stream,sizeof(char),var_one_name->length,output);
	fwrite(&n_missing_values,sizeof(char),1,output);
	fwrite(&value_length,sizeof(int32_t),1,output);
	fwrite(values_one[0],sizeof(char),8,output);
	fwrite(values_one[1],sizeof(char),8,output);

	fwrite(&var_two_name->length,sizeof(int32_t),1,output);
	fwrite(var_two_name->stream,sizeof(char),var_two_name->length,output);
	fwrite(&n_missing_values,sizeof(char),1,output);
	fwrite(&value_length,sizeof(int32_t),1,output);
	fwrite(values_two[0],sizeof(char),8,output);
	fwrite(values_two[1],sizeof(char),8,output);

	if(ferror(output)){
		fclose(output);
		return false;
	}

	fclose(output);
	return true;
}

bool write_dfvar(const char *path){
	int32_t record_type=7;
	int32_t subtype=17;
	int32_t size_check=1;
	char attributes[]="foobar";
	int32_t count=6;
	
	FILE *handle=fopen(path,"wb");
	if(!handle)
		return false;
	
	fwrite(&record_type,sizeof(int32_t),1,handle);
	fwrite(&subtype,sizeof(int32_t),1,handle);
	fwrite(&size_check,sizeof(int32_t),1,handle);
	fwrite(&count,sizeof(int32_t),1,handle);
	fwrite(attributes,sizeof(char),6,handle);
	
	if(ferror(handle)){
		fclose(handle);
		return false;
	}
	
	fclose(handle);
	return true;
}
