/*
 dw_Mach-O_utils.h : Mach-O utils definitions.

 (c) 2007-2008 Fernando Iazeolla

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<stdio.h>
#include<readline/readline.h>
#include<readline/history.h>
#include"stdint.h"
#include"main.h"
#include"macho.h"
#include"elf.h"
#include"pe.h"
#include"dw_elf_utils.h"
#include"dw_macho_utils.h"
#include"dw_pe_utils.h"
#include"dw_readline_completion.h"
#include"utils.h"
#include"lang.h"
#include"vars.h"


void load_macho_hd()
{
	int n_seg,n_sect;
	char path[255];
	struct mach_header *mac;
	struct load_command *lc;
	struct segment_command *seg;
	struct section *sect;
	mac=(struct mach_header*)faddr;
	make_table(NULL,"s",-1);
	add_s_var("s","magic",TYPE_VAL,&mac->magic);
	add_s_var("s","cputype",TYPE_VAL,&mac->cputype);
	add_s_var("s","cpusubtype",TYPE_VAL,&mac->cpusubtype);
	add_s_var("s","filetype",TYPE_VAL,&mac->filetype);
	add_s_var("s","ncmds",TYPE_VAL,&mac->ncmds);
	add_s_var("s","sizeofcmds",TYPE_VAL,&mac->sizeofcmds);
	add_s_var("s","flags",TYPE_VAL,&mac->flags);
	if(mac->ncmds>0)
	{
		make_table(NULL,"lc",mac->ncmds);
		lc=(struct load_command*)(faddr+sizeof(struct mach_header));
		n_seg=0;
		//printf("%d %x %d %x %d\n",lc->cmd,mac,sizeof(struct 	mach_header),lc,lc->cmdsize);
		do
		{
			//printf("%d.%d(%x)%d ",n,lc->cmd,lc,lc->cmdsize);
			switch(lc->cmd)
			{
			case LC_SEGMENT:
				//printf("LC_SEGMENT\n");
				seg=(struct segment_command*)lc;
				sprintf(path,"lc[%d]",n_seg);
				add_s_var(path,"cmd",TYPE_VAL,&seg->cmd);
				add_s_var(path,"cmdsize",TYPE_VAL,&seg->cmdsize);
				add_s_var(path,"segname",TYPE_STRING,&seg->segname);
				add_s_var(path,"vmaddr",TYPE_VAL,&seg->vmaddr);
				add_s_var(path,"vmsize",TYPE_VAL,&seg->vmsize);
				add_s_var(path,"fileoff",TYPE_VAL,&seg->fileoff);
				add_s_var(path,"filesize",TYPE_VAL,&seg->filesize);
				add_s_var(path,"maxprot",TYPE_VAL,&seg->maxprot);
				add_s_var(path,"initprot",TYPE_VAL,&seg->initprot);
				add_s_var(path,"nsects",TYPE_VAL,&seg->nsects);
				add_s_var(path,"flags",TYPE_VAL,&seg->flags);
				if(seg->nsects>0)
				{
					make_table(path,"sect",seg->nsects);
					sect=(struct section*)((char*)seg+sizeof(struct segment_command));
					n_sect=0;
					do{
						sprintf(path,"lc[%d]->sect[%d]",n_seg,n_sect);
						add_s_var(path,"sectname",TYPE_STRING,&sect->sectname);
						add_s_var(path,"segname",TYPE_STRING,&sect->segname);
						add_s_var(path,"addr",TYPE_VAL,&sect->addr);
						add_s_var(path,"size",TYPE_VAL,&sect->size);
						add_s_var(path,"offset",TYPE_VAL,&sect->offset);
						add_s_var(path,"align",TYPE_VAL,&sect->align);
						add_s_var(path,"reloff",TYPE_VAL,&sect->reloff);
						add_s_var(path,"nreloc",TYPE_VAL,&sect->nreloc);
						add_s_var(path,"flags",TYPE_VAL,&sect->flags);
						add_s_var(path,"reserved1",TYPE_VAL,&sect->reserved1);
						add_s_var(path,"reserved2",TYPE_VAL,&sect->reserved2);
						sect=(struct section*)((char*)sect+sizeof(struct section));
						n_sect++;
					}while(n_sect<seg->nsects);
				}
				break;
			defaulf:
				break;
			}
			lc=(struct load_command*)((char*)lc+lc->cmdsize);
			n_seg++;
		}while(n_seg<mac->ncmds);
	}
	//make_table(NULL,"lc",4);
	//make_table("lc[2]","sect",2);
	//add_s_var("lc[2]->sect[1]","pippo",TYPE_VAL,&xx);
	printf("mach-o\n");
}
