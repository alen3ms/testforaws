#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<getopt.h>
#include<string.h>
#include<errno.h>
#include"main.h"
#include"look_debug.h"

void usage()
{
	printf("dwarf v0.1 -- drugo@code.autistici.org\n");
	printf("=====\n");
	printf("\nUSAGE:\n");
	printf("dwarf [options] [file]\n");
	printf("\nvalid options:\n\n");
	printf("-i            interactive (shell mode)\n");
	printf("-h            help\n");
	printf("-c '...'      command  mode (execute commands)\n");
	printf("-x <file>     execute file script\n");
	printf("-t            file type\n");
	printf("-s <all,sh,main>  section headers\n");
	printf("-f type       force to treat as a <pe|elf|mach-o|mz> file\n");
	printf("-r            reloc (relocate section offsets)\n");
	printf("-S --size= �val,val,�section\n");
	printf("--section= �pos,name\n");
	printf("--rel= sectors, sectname,�offs      (relocate section(s) offset)\n");
	printf("--ph=�pos,name\n");
	printf("-m --move=init,[end,len,]init2       move block\n");
	exit(1);
}
void get_defaults_arg()
{
	ilook_debug=0;
	forced=FT_NULL;
	file_type=FT_NULL;
}
void parse_args(int argc,char **argv)
{
	char xx1[255];
	char *s1;
	int c;
	while (1)
	{
		static struct option long_options[] =
		{
			{"move",required_argument,0,'m'},
			{"size",required_argument,0,'S'},
			{"section",required_argument,0,'s'}
			
		};
		int option_index = 0;
		c = getopt_long (argc, argv, "hidtrS:m:s:c:x:f:",long_options, &option_index);
		if (c == -1) break;
		switch(c)
		{
			case 'i':
				printf("interactive selected....to be developped");
				break;
			case 't':
				s1=(char*)malloc(MAX_STR*sizeof(char));
				printf("file type selected\n");
				s1=file_probe();
				printf("%s\n",s1);
				free(s1);
				break;
			case 'x':
				printf("input script file:%s\n",optarg);
				break;
			case 's':
				if((strcmp(optarg,"all"))==0) ;//section_type=SEC_ALL;
				if((strcmp(optarg,"main"))==0) ;
				if((strcmp(optarg,"sh"))==0) ;
				break;
				case'f':
				if((strcmp(optarg,"null"))==0) forced=FT_NULL;  //default
				if((strcmp(optarg,"pe"))==0) forced=FT_PE;
				if((strcmp(optarg,"elf"))==0) forced=FT_ELF;
				if((strcmp(optarg,"mach-o"))==0) forced=FT_MACHO;
				if((strcmp(optarg,"mz"))==0) forced=FT_MZ;
				break;
			case 'r':
				
				break;
			case 'S':
				printf("size!...to develop.\n");
				break;
			case 'm':
				printf("move!....to develop.\n");
				break;
			case 'd':
				ilook_debug=1;
				break;
			case 'c':
				strcpy(xx1,optarg);
				printf("to execute:%s ....to develop..\n",xx1);
				break;
			case 'h':
			case '?':
				usage();
				break;
			default:
				usage();
				break;
		}
		
	}
}
void look_data_ok()
{
	
}
int main(int argc,char **argv)
{
	get_defaults_arg();
	parse_args(argc,argv);
	look_data_ok();
	if(argc<2) usage();
	if(optind<argc) {strncpy(filename,argv[optind],MAX_FILENAME);}
	else{strncpy(filename,"<NULL>",MAX_FILENAME);}
	if(ilook_debug) look_debug();
	return 0;
}