#include <stdio.h>
#include <dos.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>

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
	//Reading boot sector from disk D
	BOOT boot;

	int res = absread(drive, 1, 0, &boot);
	if(res != 0){
		printf("Cannot read boot sector\n");
		return;
	}
	//Reading ROOT from disk D
	printf("\n\n");
	printf("Reading ROOT information:\n");
	printf("------------------------\n");
	int num_byte = boot.ROOT_size * 32;//sizeof(ROOT)
	ROOT *root = (ROOT *)malloc(num_byte);
	if(root == NULL) return;
	int num_sector = num_byte / boot.bytes_per_sector;
	int root_begin = boot.reserved + boot.FAT_size * boot.FAT_cnt;
	// Doc ROOT
	res = absread(drive, num_sector, root_begin, (void *)root);
	if(res != 0){
		printf("\n Cannot read ROOT\n");
		return;
	}
	// Phan tich root
	printf("So luong khoan muc toi da cua ROOT: %d\n", boot.ROOT_size);
	// Hien thi root
	printf("Noi dung ROOT: \n");
	printf("--------------------------\n");
	for (int i =0; i< boot.ROOT_size; i++)
	{
		if (root[i].size != 0)
		{
			for (int j =0; j< 8; j++)
				printf("%c", root[i].name[j]);
			printf("\t");
			for (j = 0; j < 3; j++)
				printf("%c", root[i].ext[j]);
			printf("\t%d", root[i].size);
			printf("\t%d\n", root[i].first_cluster);
		}
	}
	getchar();
}
