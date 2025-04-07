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
	// Nhap ten file
	char filename[8];
	printf("Nhap ten file: ");
	scanf("%s", filename);
	int first_cluster = -1;

	// Kiem tra file
	for (int i = 0; i < boot.ROOT_size; i++)
	{
		char str[9];
		if (root[i].size != 0)
		{
			int j;
			for (j = 0; j < 8 && root[i].name[j] != ' '; j++)
				str[j] = root[i].name[j];
			str[j] = 0;
			if (strcmp(filename, str) == 0)
			{
				first_cluster = root[i].first_cluster;
				break;
			}
		}
	}
	getchar();
	if (first_cluster == -1)
	{
		printf("File khong ton tai\n");
	}
	else
	{
		unsigned int cur = first_cluster;
		printf("Thu tu cluster cua file %s la: ", filename);
		while (cur < 0xFFF0)
		{
			printf("%d ", cur);
			cur = fat[cur];
		}

	}
	printf("\n");
	getchar();



}