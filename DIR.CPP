
#include <stdio.h>
#include <dos.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct BOOT {  //for FAT16
	char jmp[3];
	char OEM[8];
	int bytes_per_sector;
	char sectors_per_cluster;
	int reserved;
	char FAT_cnt;
	int ROOT_size;
	int total_sectors;
	char media;
	int FAT_size;
	int sectors_per_track;
	int head_cnt;
	long hidden_sectors;
	long total_sectors_long;
	char unknown[3];
	long serial;
	char volume[11];
	char FAT_type[8];
	char loader[448];
	char mark[2];
};

struct ROOT {
	char name[8];
	char ext[3];
	char attr;
	char reserved[10];
	char time[2];
	char date[2];
	int first_cluster;
	long size;
};

void main()
{
	int drive = 3; //A=0, B=1, C=2, D=3 ...
	BOOT boot;
	int res = absread(drive, 1, 0, &boot);
	if(res != 0){
		printf("Cannot read boot sector\n");
		return;
	}
	unsigned int *fat = (unsigned int *)malloc (boot.FAT_size * boot.bytes_per_sector);
	if (fat == NULL) {
		printf("Not enough memory\n");
		return;
	}
	res = absread(drive, boot.FAT_size, boot.reserved, fat);
	if(res != 0){
		printf("Cannot read FAT\n");
		return;
	}
	int num_byte = boot.ROOT_size * 32;//sizeof(ROOT)
	ROOT *root = (ROOT *)malloc(num_byte);
	if(root == NULL) return;
	int num_sector = num_byte / boot.bytes_per_sector;
	int root_begin = boot.reserved + boot.FAT_size * boot.FAT_cnt;
	res = absread(drive, num_sector, root_begin, (void *)root);
	if(res != 0){
		printf("\n Cannot read ROOT\n");
		return;
	}
	// Duyet thu muc ROOT
	printf("Command: dir\n");
	printf("-------------\n");
	int count_file = 0;
	long count_file_large = 0;
	for (int i = 0; i < boot.ROOT_size; i++)
	{
		if (root[i].size != 0)
		{
			for (int j = 0; j < 8; j++)
				printf("%c", root[i].name[j]);
			printf("\t");
			for (j = 0; j < 3; j++)
				printf("%c", root[i].ext[j]);
			printf("\t%ld", root[i].size);
			printf("\t%02d-%02d",root[i].date[0], root[i].date[1]);
			printf("\t");
			printf("%02d:%02d",root[i].time[0], root[i].time[1]);
			printf("\t");
			printf("\n");
			count_file ++;
			count_file_large += root[i].size;

		}
	}
	printf("\t%d file(s)\t%ld bytes", count_file, count_file_large);

	getchar();

}