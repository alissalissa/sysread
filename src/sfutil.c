#include "sfutil.h"

void mass_free(int arg_num,...){
	va_list to_free;
	va_start(to_free,arg_num);
	for(int i=0;i<arg_num;i++){
		char *a=va_arg(to_free,char*);
		free(a);
	}
	va_end(to_free);
}

bool verify_date_format(char *sample){
	//char day[]={sample[0],sample[1]};
	char month[]={sample[3],sample[4],sample[5],'\0'};
	//char year[]={sample[7],sample[8]};

	if(strcmp(month,"Jan") && strcmp(month,"Feb") && strcmp(month,"Mar") && strcmp(month,"Apr") && strcmp(month,"May") && strcmp(month,"Jun") && strcmp(month,"Jul") && strcmp(month,"Aug") && strcmp(month,"Sep") && strcmp(month,"Oct") && strcmp(month,"Nov") && strcmp(month,"Dec"))
		return false;

	return true;

}

bool verify_time_format(char *sample){
	if(sample[2]!=':' || sample[5]!=':')
		return false;
	return true;
}