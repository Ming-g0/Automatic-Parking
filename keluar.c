#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define hargaMotorPerJam 2000
#define hargaMobilPerJam 5000
#define hargaMotorMaks 4000
#define hargaMobilMaks 8000
#define FILEPROCESSING "bparking.txt"


struct Parkir {
    char tipeKendaraan[10];
    char nomorKendaraan[20];
    char waktuMasuk[30];
};


void hapusData(const char *nomorKendaraan, const char *waktuMasuk) {
    FILE *file = fopen(FILEPROCESSING, "r");
    if (file == NULL) {
        printf("File tidak ada.\n");
        exit(1);
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("Tidak bisa membuka file temp.\n");
        exit(1);
    }

    char data[100];
    int flag = 0;
    while (fgets(data, sizeof(data), file)) {
        if (strstr(data, nomorKendaraan) != NULL && strstr(data, waktuMasuk) != NULL) {
            flag = 1;
            continue;
        }
        fprintf(temp, "%s", data);
    }

    fclose(file);
    fclose(temp);

    if (!flag) {
        printf("Data kendaraan tidak ada.\n");
        remove("temp.txt");
        return;
    }

    remove(FILEPROCESSING);
    rename("temp.txt", FILEPROCESSING);
}


void convertHuruf(char *str) {
    while (*str != '\0') {
        if (*str >= 'a' && *str <= 'z') {
            *str = *str - 32;
        }
        str++;
    }
}



void bayarParkir(const char *tipeKendaraan, const char *nomorKendaraan, const char *waktuMasuk) {
    time_t waktuSekarang;
    time(&waktuSekarang);
    struct tm *waktuKeluar = localtime(&waktuSekarang);

    char waktuKeluarConvert[30];
    strftime(waktuKeluarConvert, sizeof(waktuKeluarConvert), "%Y-%m-%d %H:%M:%S", waktuKeluar);

    printf("--------------------------------------\n");
    printf("---------   Data Kendaraan   ---------\n");
    printf("--------------------------------------\n");
    printf("Jenis Kendaraan\t: %s\n", tipeKendaraan);
    printf("Nomor Kendaraan\t: %s\n", nomorKendaraan);
    printf("Waktu Masuk\t: %s\n", waktuMasuk);
    printf("Waktu Keluar\t: %s\n", waktuKeluarConvert);

    int jamMasuk, jamKeluar;
    sscanf(waktuMasuk, "%*d-%*d-%*d %d:%*d:%*d", &jamMasuk);
    sscanf(waktuKeluarConvert, "%*d-%*d-%*d %d:%*d:%*d", &jamKeluar);

    int lamaParkir = (jamKeluar - jamMasuk < 1) ? 1 : jamKeluar - jamMasuk;

    printf("Lama Parkir\t: %d jam\n", lamaParkir);

	float tagihan = 0.0;
    if (strcmp(tipeKendaraan, "Motor") == 0) {
        tagihan = (lamaParkir > 2) ? hargaMotorMaks : lamaParkir * hargaMotorPerJam;
    }
	else if (strcmp(tipeKendaraan, "Mobil") == 0) {
        tagihan = (lamaParkir > 2) ? hargaMobilMaks : lamaParkir * hargaMobilPerJam;
    }

    printf("Jumlah Tagihan\t: %.2f\n", tagihan);
	printf("--------------------------------------\n");
	
    float bayar;
    printf("Uang Pembayaran\t: ");
    scanf("%f", &bayar);

    float kembalian = bayar - tagihan;
    printf("Uang Kembalian\t: %.2f\n\n", kembalian);
    printf("--------------------------------------\n");
    printf("---------   Silakan keluar   ---------\n");
	printf("---------    Terima Kasih    ---------\n");
    printf("--------------------------------------\n");
}


int main() {
    char nomorKendaraan[20];
    
	printf("=====================================\n");
    printf("=======      \"B-PARKING\"      =======\n");
    printf("=====================================\n\n\n");
    
    printf("Masukkan Nomor Kendaraan>> ");
    scanf("%s", nomorKendaraan);
	convertHuruf(nomorKendaraan);
	
    FILE *file = fopen(FILEPROCESSING, "r");
    if (file == NULL) {
        printf("File tidak ada.\n");
        return 1;
    }

    struct Parkir park;
    int flag1 = 0;
    while (fscanf(file, "%s %s %[^\n]\n", park.tipeKendaraan, park.nomorKendaraan, park.waktuMasuk) != EOF) {
    	convertHuruf(park.nomorKendaraan);
    	if (strcasecmp(park.nomorKendaraan, nomorKendaraan) == 0) {
        	flag1 = 1;
        	bayarParkir(park.tipeKendaraan, park.nomorKendaraan, park.waktuMasuk);
        	break;
    	}
	}

    fclose(file);

    if (!flag1) {
        printf("Nomor Kendaraan tidak ada.\n");
        return 1;
    }

    hapusData(nomorKendaraan, park.waktuMasuk);

    return 0;
}

