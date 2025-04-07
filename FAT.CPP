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


void main()
{

	int drive = 3; //A=0, B=1, C=2, D=3 ...
	BOOT boot;
	int res = absread(drive, 1, 0, &boot);
	//Reading FAT16 table from disk D
	unsigned int *fat = (unsigned int *)malloc (boot.FAT_size * boot.bytes_per_sector);
	if (fat == NULL) {
		printf("Not enough memory\n");
		return;
	}
	printf("\n\n");
	printf("Reading FAT16 parameters\n");
	printf("-------------------------\n");
	printf("FAT size: %d \n", boot.FAT_size);
	printf("Reserved: %d \n", boot.reserved);

	// Doc bang FAT
	res = absread(drive, boot.FAT_size, boot.reserved, fat);
	if(res != 0){
		printf("Cannot read FAT\n");
		return;
	}
	// Phan tich bang FAT
	int FAT_cnt =(boot.FAT_size * boot.bytes_per_sector) / 2;
	printf("So luong index trong bang FAT: %d\n", FAT_cnt);
	int free_count = 0;
	printf("Noi dung bang FAT (100): \n");
	for (int i = 2; i < 100; i++)
	{
		printf("%d ",fat[i] );
		if (fat[i] == 0)
		{
			free_count++;
		}
		else
			continue;
	}
	printf("\nSo luong cluster rong trong 100 cluster dau: %d", free_count);

	getchar();
}
