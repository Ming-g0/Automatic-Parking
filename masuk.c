#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define jumlahMotor 600
#define jumlahMobil 400
#define FILEPROCESSING "bparking.txt"

void readFile(int *motor, int *mobil) {
    FILE *file = fopen(FILEPROCESSING, "r");
    if (file == NULL) {
        printf("File tidak ditemukan.\n");
        return;
    }

    char data[100];
    int banyakMotor = 0, banyakMobil = 0;

    while (fgets(data, sizeof(data), file) != NULL) {
        if (strstr(data, "Motor") != NULL) {
            banyakMotor++;
        } else if (strstr(data, "Mobil") != NULL) {
            banyakMobil++;
        }
    }

    *motor = jumlahMotor - banyakMotor;
    *mobil = jumlahMobil - banyakMobil;

    fclose(file);
}

int cekKendaraan(const char *nomorKendaraan) {
    FILE *file = fopen(FILEPROCESSING, "r");
    if (file == NULL) {
        printf("File tidak ditemukan.\n");
        return 0;
    }

    char data[100];

    while (fgets(data, sizeof(data), file) != NULL) {
        if (strstr(data, nomorKendaraan) != NULL) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void convertHuruf(char *str) {
    while (*str != '\0') {
        if (*str >= 'a' && *str <= 'z') {
            *str = *str - 32;
        }
        str++;
    }
}

void addData(int *motor, int *mobil) {
    FILE *file = fopen(FILEPROCESSING, "a");
    if (file == NULL) {
        printf("Tidak bisa membuka file.\n");
        exit(1);
    }

    char tipeKendaraan[10], nomorKendaraan[20], waktuMasuk[30];
    time_t realTime;
    struct tm *detailWaktu;

    printf("\n1. Motor\n");
    printf("2. Mobil\n");
    printf("Pilih tipe kendaraan>> ");

    int tipe;
    scanf("%d", &tipe);

    if (tipe == 1) {
        if (*motor > 0) {
            strcpy(tipeKendaraan, "Motor");
            (*motor)--;
        } else {
            printf("Parkir motor penuh.\n");
            fclose(file);
            return;
        }
    } else if (tipe == 2) {
        if (*mobil > 0) {
            strcpy(tipeKendaraan, "Mobil");
            (*mobil)--;
        } else {
            printf("Parkir mobil penuh.\n");
            fclose(file);
            return;
        }
    } else {
        printf("Pilihan tidak valid.\n");
        fclose(file);
        return;
    }

    system("cls");
    printf("Masukkan nomor kendaraan>> ");
    scanf("%s", nomorKendaraan);
    convertHuruf(nomorKendaraan);

    if (cekKendaraan(nomorKendaraan)) {
    	system("cls");
    	printf("----------------------------------------\n");
        printf("Nomor kendaraan tersebut sudah digunakan\n");
        printf("----------------------------------------\n");
        fclose(file);
        return;
    }

    time(&realTime);
    detailWaktu = localtime(&realTime);
    strftime(waktuMasuk, sizeof(waktuMasuk), "%Y-%m-%d %H:%M:%S", detailWaktu);

    fprintf(file, "%s %s %s\n", tipeKendaraan, nomorKendaraan, waktuMasuk);
    fclose(file);
    system("cls");
    printf("--------------------------------------\n");
    printf("--------   Ambil tiket anda   --------\n");
	printf("--------    Silakan masuk     --------\n");
	printf("--------------------------------------\n");
}

int main() {
    int motor, mobil;

    readFile(&motor, &mobil);
	
    printf("=====================================\n");
    printf("=======   Selamat Datang di   =======\n");
    printf("=======      \"B-PARKING\"      =======\n");
    printf("=====================================\n\n\n");
    printf("Jumlah parkir motor yang tersedia: %d\n", motor);
    printf("Jumlah parkir mobil yang tersedia: %d\n", mobil);
    printf("--------------------------------------\n");

    addData(&motor, &mobil);

    return 0;
}

