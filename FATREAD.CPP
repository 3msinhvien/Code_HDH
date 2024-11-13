#include <dos.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

struct BOOT {  // Cau truc boot cho FAT16
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

int main() {
    int drive = 3;  // A=0, B=1, C=2, D=3 (o D)

    // Doc boot sector tu o dia de lay thong tin
    BOOT boot;
    int res = absread(drive, 1, 0, &boot);
    if(res != 0){
        printf("Khong the doc boot sector\n");
        return 1;
    }

    // Cap phat bo nho cho bang FAT
    unsigned int *fat = (unsigned int *)malloc(boot.FAT_size * boot.bytes_per_sector);
    if (fat == NULL) {
        printf("Khong du bo nho\n");
        return 1;
    }

    // Doc bang FAT tu o dia
    res = absread(drive, boot.FAT_size, boot.reserved, fat);
    if(res != 0){
        printf("Khong the doc bang FAT\n");
        free(fat);
        return 1;
    }

    // Hien thi va phan tich noi dung bang FAT
    printf("Noi dung va phan tich bang FAT:\n");
    printf("-------------------------------\n");

    int num_cells = (boot.FAT_size * boot.bytes_per_sector) / sizeof(unsigned int);
    for (int i = 0; i < num_cells; i++) {
        printf("FAT[%d]: %u - ", i, fat[i]);

        // Phan tich trang thai cluster
        if (fat[i] == 0x0000) {
            printf("Cluster trong\n");
        } else if (fat[i] >= 0x0002 && fat[i] <= 0xFFF6) {
            printf("Cluster da cap phat, chi den cluster tiep theo: %u\n", fat[i]);
        } else if (fat[i] >= 0xFFF8 && fat[i] <= 0xFFFF) {
            printf("Cluster cuoi cung cua tep\n");
        } else {
            printf("Trang thai khong xac dinh\n");
        }

        if (i % 16 == 15) {  // Sau moi 16 cell, xuong dong
            printf("\n");
        }
    }

    // Giai phong bo nho
    free(fat);

    return 0;
}

