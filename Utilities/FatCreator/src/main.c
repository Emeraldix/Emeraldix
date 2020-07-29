#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define FATFS_INC_WRITE_SUPPORT 1
#include "fat_filelib.h"

#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

PACK(
struct FatBPB 
{
	char   	BS_OEMName[8];
	uint16  BPB_BytePerSec;
	uint8   BPB_SecPerClus;
	uint16  BPB_RsvdSecCnt;
	uint8   BPB_NumFATs;
	uint16  BPB_RootEntCnt;
	uint16  BPB_TotSec16;
	uint8   BPB_Media;
	uint16  BPB_FATsz16;
	uint16  BPB_SecPerTrk;
	uint16  BPB_NumHeads;
	uint32  BPB_HiddSec;
	uint32  BPB_TotSec32;
}
);

PACK(
struct Fat32 
{
	uint32 	EBR_SecPerFat;
	uint16 	EBR_Flags;
	uint16 	EBR_FatVersion;
	uint32 	EBR_ClusRootDir;
	uint16 	EBR_SecNumFSInfo;
	uint16 	EBR_SecNumBackup;
	uint32 	EBR_Reserved1;
	uint32 	EBR_Reserved2;
	uint32 	EBR_Reserved3;
	uint8 	EBR_Drive;
	uint8 	EBR_Reserved4;
	uint8 	EBR_Signature;
	uint32 	EBR_VolumeID;
	char 	EBR_VolumeLabel[11];
	char 	EBR_SysIdent[8];
}
);

PACK(
struct FSInfo
{
	uint32 Signature1;
	char Reserved1[480];
	uint32 Signature2;
	uint32 LastFreeClus;
	uint32 NumLastFreeClus;
	char Reserved2[12];
	uint32 Signature3;
}
);

FILE *imgS;
FILE *fiB;

struct FatBPB bpb;
struct Fat32 epb;
struct FSInfo fio;
uint32 FAT[3];

int div_up(int x, int y);
FILE* createImg(uint32 sizeMB, char* path);

int ReadMedia(unsigned long sector, unsigned char *buffer, unsigned long sectorCount);
int WriteMedia(unsigned long sector, unsigned char *buffer, unsigned long sectorCount);

int main(int argc, char *argv[]) {
	char sector[512];

	if(argc < 4) 
	{
		printf(argv[0]);
		printf("\nUsage: FatCreator <type> [...]\n");
		
		printf("Usage: FatCreator disk <size in MB> <sorce img>\n");
		printf("Usage: FatCreator boot <sorce img> <boot loader bin>\n");
		printf("Usage: FatCreator fat32 <sorce img> <boot loader bin>\n");
		printf("Usage: FatCreator fat32add <sorce img> <file> <path in FAT>\n");
		
		return 1;
	}

	if(strcmp(argv[1], "disk") == 0)
	{
		imgS = createImg(atoi(argv[2]), argv[3]);
		fclose(imgS);
	}
    else if(strcmp(argv[1], "boot") == 0) 
	{
		imgS = fopen(argv[2], "rb+");
		if(imgS == NULL) 
		{
            printf("Open file (image) error\n");
            return 1;
        }

		fiB = fopen(argv[3], "r");
        if(fiB == NULL) 
		{
            printf("Open file (boot loader) error\n");
            return 1;
        }

        fseek(fiB, 0L, SEEK_END);
        int sz = ftell(fiB);
        fseek(fiB, 0L, SEEK_SET);

        for(int i = 0; i < div_up(sz, 512); ++i) 
		{
			memset(sector, 0, 512);
            fread(sector, sizeof(char), 512, fiB);
            fwrite(sector, sizeof(char), 512, imgS);
        }
		
		fclose(fiB);
		fclose(imgS);
    } 
	else if(strcmp(argv[1], "fat32") == 0) 
	{
		imgS = fopen(argv[2], "rb+");
		if(imgS == NULL) 
		{
            printf("Open file (image) error\n");
            return 1;
        }
		
		fiB = fopen(argv[3], "r");
        if(fiB == NULL) 
		{
            printf("Open file (boot loader) error\n");
            return 1;
        }
		
		fseek(fiB, 0L, SEEK_END);
        int sz = div_up(ftell(fiB), 512);
        fseek(fiB, 0L, SEEK_SET);
		
		memset(&bpb, 0, sizeof(bpb));
		memset(&epb, 0, sizeof(epb));
		memset(&fio, 0, sizeof(fio));
		
		strcpy(bpb.BS_OEMName, "Emeraldi");
		bpb.BPB_BytePerSec = 512;
		bpb.BPB_RsvdSecCnt = sz + 3; //bootloader + FSInfo + backup (BPB + FSInfo)
		bpb.BPB_NumFATs = 1;
		bpb.BPB_RootEntCnt = 0;
		bpb.BPB_TotSec16 = 0;
		bpb.BPB_Media = 248; // hdd
		bpb.BPB_FATsz16 = 0;
		bpb.BPB_HiddSec = 0;
		
		fseek(imgS, 0L, SEEK_END);
        int secNum = div_up(ftell(imgS), 512);
        fseek(imgS, 0L, SEEK_SET);
		
		bpb.BPB_TotSec32 = secNum;
		if(secNum > 67108864) //32Gb
			bpb.BPB_SecPerClus = 64;
		else if(secNum > 33554432) //16Gb
			bpb.BPB_SecPerClus = 32;
		else if(secNum > 16777216) //8Gb
			bpb.BPB_SecPerClus = 16;
		else
			bpb.BPB_SecPerClus = 8;
		
		epb.EBR_SecPerFat = div_up(div_up((bpb.BPB_TotSec32 - bpb.BPB_RsvdSecCnt), bpb.BPB_SecPerClus), (bpb.BPB_BytePerSec / 4));
		FAT[0] = 0x0FFFFFF8;
		FAT[1] = 0x0FFFFFFF;
		FAT[2] = 0xFFFFFFFF;
		epb.EBR_Flags = 0;
		epb.EBR_FatVersion = 256; //1.0
		epb.EBR_ClusRootDir = 2;
		epb.EBR_SecNumFSInfo = sz;
		epb.EBR_SecNumBackup = sz + 1;
		epb.EBR_Drive = 128;
		epb.EBR_Signature = 41;
		epb.EBR_VolumeID = 0;
		strcpy(epb.EBR_VolumeLabel, "TEST!      ");
		strcpy(epb.EBR_SysIdent, "FAT32   ");
		
		fio.Signature1 = 0x41615252;
		fio.Signature2 = 0x61417272;
		fio.Signature3 = 0xAA550000;
		fio.LastFreeClus = 0xFFFFFFFF;
		fio.NumLastFreeClus = 0xFFFFFFFF;
		
		// Write bootloader
		for(int i = 0; i < sz; ++i) 
		{
			memset(sector, 0, 512);
            fread(sector, sizeof(char), 512, fiB);
            fwrite(sector, sizeof(char), 512, imgS);
        }
		
		fseek(imgS, 3L, SEEK_SET);
		fwrite(&bpb, sizeof(bpb), 1, imgS);
		fwrite(&epb, sizeof(epb), 1, imgS);
		
		fseek(imgS, 0L, SEEK_SET);
		memset(sector, 0, 512);
		fread(sector, sizeof(char), 512, imgS);
		fseek(imgS, 512 * epb.EBR_SecNumFSInfo, SEEK_SET);
		fwrite(&fio, sizeof(fio), 1, imgS);

		// Write backup
		fseek(imgS, 512 * epb.EBR_SecNumBackup, SEEK_SET);
        fwrite(sector, sizeof(char), 512, imgS);
		fwrite(&fio, sizeof(fio), 1, imgS);
		
		fwrite(FAT, sizeof(FAT), 1, imgS);
		
		fclose(fiB);
		fclose(imgS);
	}
	else if(strcmp(argv[1], "fat32add") == 0 && argc > 4)
	{
		imgS = fopen(argv[2], "rb+");
		if(imgS == NULL) 
		{
            printf("Open file (image) error\n");
            return 1;
        }
		
		fseek(imgS, 3L, SEEK_SET);
		fread(&bpb, sizeof(bpb), 1, imgS);
		
		fiB = fopen(argv[3], "r");
        if(fiB == NULL) 
		{
            printf("Open file (file) error\n");
            return 1;
        }
		
		fl_init();
		if(fl_attach_media(ReadMedia, WriteMedia) != FAT_INIT_OK)
		{
			printf("Error initializing fat_io_lib");
			return 0;
		}
		
		FL_FILE* f1 = fl_fopen(argv[4], "w");

        fseek(fiB, 0L, SEEK_END);
        int sz = ftell(fiB);
        fseek(fiB, 0L, SEEK_SET);

        for(int i = 0; i < div_up(sz, 512); ++i) 
		{
			memset(sector, 0, 512);
            size_t readc = fread(sector, sizeof(char), 512, fiB);
            fl_fwrite(sector, sizeof(char), readc, f1);
        }
		
		fl_fclose(f1);
		fl_shutdown();
		fclose(fiB);
		fclose(imgS);
	}
	else
		printf("Invalid parameter");

	return 0;
}

int div_up(int x, int y) 
{
    return (x - 1) / y + 1;
}

FILE* createImg(uint32 sizeMB, char* path)
{
	FILE* img = fopen(path, "w+");
	
	char past[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	for(uint32 i = 0; i < sizeMB * 1024 * 64; ++i)
	{
		int ret = fwrite(past, sizeof(char), 16, img);
		if(ret < 16)
		{
			printf("Write img error");
			return img;
		}
	}
	
	fseek(img, 0L, SEEK_SET);
	
	return img;
}

int ReadMedia(unsigned long sector, unsigned char *buffer, unsigned long sectorCount)
{
	fseek(imgS, bpb.BPB_BytePerSec * sector, SEEK_SET);
	fread(buffer, sizeof(unsigned char), bpb.BPB_BytePerSec * sectorCount, imgS);
	
	return 1;
}

int WriteMedia(unsigned long sector, unsigned char *buffer, unsigned long sectorCount)
{
	fseek(imgS, bpb.BPB_BytePerSec * sector, SEEK_SET);
	fwrite(buffer, sizeof(unsigned char), bpb.BPB_BytePerSec * sectorCount, imgS);
	fflush(imgS);
	
	return 1;
}