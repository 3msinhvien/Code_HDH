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
	int drive = 3; // D:\\
	// Doc thong tin tu BOOT sector
	BOOT boot;
	int res = absread(drive, 1, 0, &boot);

		if(res != 0){
		printf("Cannot read boot sector\n");
		return;
	}
	printf("Reading disk parameters\n");
	printf("-------------------------\n");
	printf("Lenh nhay: %p, %p, %p\n", boot.jmp[0], boot.jmp[1], boot.jmp[2]);
	printf("Ten hang san xuat: ");
	for (int i = 0; i < 8; i++)
		printf("%c", boot.OEM[i]);
	printf("\nKich thuoc Sector: %d (bytes)", boot.bytes_per_sector);
	printf("\nKich thuoc Cluster: %d (sector)", boot.sectors_per_cluster);
	printf("\nSo luong sector du tru trong vung du tru cua dia: ");
	printf("%d (u sector)\n", boot.reserved);
	printf("So luong bang FAT: %d \n",boot.FAT_cnt);
	printf("So luong khoan muc trong ROOT: %d\n", boot.ROOT_size);
	printf("So luong sector trong dia: %u\n", boot.total_sectors);
	printf("Kieu dia: %p\n", boot.media);
	printf("Kich thuoc FAT: %d (index)\n", boot.FAT_size);
	printf("So sector/track: %d\n", boot.sectors_per_track);
	printf("So luong dau tu: %d\n", boot.head_cnt);
	printf("So sector an trong partiotion: %d\n", boot.hidden_sectors);
	printf("Tong so sector: %u\n", boot.total_sectors_long);
	printf("Unknown: ");
	for (i = 0; i < 3; i++)
		printf("%c", boot.unknown[i]);
	printf("\nSerial: %ld", boot.serial);
	printf("\nVolume lable: ");
	for (i = 0; i< 11; i++)
		printf("%c", boot.volume[i]);
	printf("\nFAT type: ");
	for (i = 0; i< 8; i++)
		printf("%c", boot.FAT_type[i]);
	getchar();
}