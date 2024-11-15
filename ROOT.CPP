#include <dos.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
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

struct ROOT {  // Cau truc thong tin cua tep trong ROOT
    char name[8];
    char ext[3];
    char attr;
    char reserved[10];
    char time[2];
    char date[2];
    int first_cluster;
    long size;
};

int main() {
    int drive = 2;  // A=0, B=1, C=2 (o C)

    // Doc boot sector tu o dia de lay thong tin ve he thong tap tin
    BOOT boot;
    int res = absread(drive, 1, 0, &boot);
    if(res != 0){
        printf("Khong the doc boot sector\n");
        return 1;
    }

    // Tinh toan kich thuoc va vi tri cua vung ROOT
    int num_byte = boot.ROOT_size * 32;  // Moi muc trong ROOT co kich thuoc 32 byte
    int root_begin = boot.reserved + boot.FAT_size * boot.FAT_cnt;  // Vi tri bat dau cua vung ROOT

    // Cap phat bo nho cho vung ROOT
    ROOT *root = (ROOT *)malloc(num_byte);
    if(root == NULL) {
        printf("Khong du bo nho\n");
        return 1;
    }

    // Doc vung ROOT tu o dia
    int num_sector = num_byte / boot.bytes_per_sector;
    res = absread(drive, num_sector, root_begin, (void *)root);
    if(res != 0){
        printf("Khong the doc ROOT\n");
        free(root);
        return 1;
    }

    // Hien thi thong tin cac tep trong vung ROOT
    printf("Noi dung vung ROOT:\n");
    printf("-------------------\n");

    for (int i = 0; i < boot.ROOT_size; i++) {
        if (root[i].name[0] == 0x00) break;  // Khong con tep nao
        if ((unsigned char)root[i].name[0] == 0xE5) continue;  // Tep da bi xoa

        // Hien thi ten tep
        printf("Ten tep: ");
        for (int j = 0; j < 8; j++) {
            if (root[i].name[j] == ' ') break;
            printf("%c", root[i].name[j]);
        }

        // Hien thi phan mo rong
        printf(".");
        for (int k = 0; k < 3; k++) {
            if (root[i].ext[k] == ' ') break;
            printf("%c", root[i].ext[k]);
        }

        // Hien thi cac thong tin khac
        printf("\n");
        printf("Thuoc tinh: %02X\n", root[i].attr);
        printf("Cluster dau tien: %d\n", root[i].first_cluster);
        printf("Kich thuoc: %ld byte\n", root[i].size);
        printf("-------------------\n");
    }

    // Giai phong bo nho
    free(root);

    return 0;
}

