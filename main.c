#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>
#include <time.h>

typedef struct {
    char dosyaYolu[PATH_MAX];
    char dosyaAdi[NAME_MAX];
    struct stat dosyaBilgisi;
} DosyaBilgisi;

int dosyaKarsilastir(const void *a, const void *b);

void dosyaListele(const char *dizin, int gizli, int boyut, int tarih, const char *dosyaAdi);

int main(int argc, char *argv[]) {
    const char *dizin = ".";
    int gizli = 0;
    int boyut = 0;
    int tarih = 0;
    const char *dosyaAdi = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-g") == 0) {
            gizli = 1;
        } else if (strcmp(argv[i], "-s") == 0) {
            boyut = 1;
        } else if (strcmp(argv[i], "-t") == 0) {
            tarih = 1;
        } else if (strcmp(argv[i], "-a") == 0) {
            gizli = 1;
            boyut = 1;
            tarih = 1;
        } else if (strcmp(argv[i], "-f") == 0) {
            dosyaAdi = argv[++i];
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printf("Kullanım: %s [-a] [-s] [-t] [-g] [-f dosyaAdi] [-h|--help] [dizin]\n", argv[0]);
            printf("-g: Gizli dosyaları da listele\n");
            printf("-s: Dosya boyutlarını listele\n");
            printf("-t: Dosya tarih bilgisini listele\n");
            printf("-a: Gizli dosyaları, dosya boyutlarını ve tarih bilgisini listele\n");
            printf("-f dosyaAdi: Çıktıyı belirtilen dosyaya kaydet\n");
            printf("-h, --help: Yardım mesajını göster\n");
            return 0;
        } else {
            dizin = argv[i];
        }
    }

    struct stat dizinBilgisi;
    if (stat(dizin, &dizinBilgisi) == -1) {
        printf("Dizin kontrol hatası\n");
        return 1;
    }

    dosyaListele(dizin, gizli, boyut, tarih, dosyaAdi);
    return 0;
}

void dosyaListele(const char *dizin, int gizli, int boyut, int tarih, const char *dosyaAdi) {
    FILE *dosyaPtr;
    DIR *dir;
    struct dirent *dosya;
    struct stat dosyaBilgisi;

    if (dosyaAdi != NULL) {
        dosyaPtr = fopen(dosyaAdi, "w");
        if (dosyaPtr == NULL) {
            perror("Dosya açılamadı");
            exit(EXIT_FAILURE);
        }
    }

    if ((dir = opendir(dizin)) == NULL) {
        perror("Dizin açılamadı");
        exit(EXIT_FAILURE);
    }

    DosyaBilgisi dosyalar[1000];
    int dosyaSayisi = 0;

    while ((dosya = readdir(dir)) != NULL) {
        if (!gizli && dosya->d_name[0] == '.') {
            continue;
        }

        char dosyaYolu[PATH_MAX];
        snprintf(dosyaYolu, PATH_MAX, "%s/%s", dizin, dosya->d_name);

        if (lstat(dosyaYolu, &dosyaBilgisi) == -1) {
            perror("Dosya bilgisi alınamadı");
            exit(EXIT_FAILURE);
        }

        snprintf(dosyalar[dosyaSayisi].dosyaYolu, PATH_MAX, "%s/%s", dizin, dosya->d_name);
        strncpy(dosyalar[dosyaSayisi].dosyaAdi, dosya->d_name, NAME_MAX);
        dosyalar[dosyaSayisi].dosyaBilgisi = dosyaBilgisi;

        dosyaSayisi++;
    }

    qsort(dosyalar, dosyaSayisi, sizeof(DosyaBilgisi), dosyaKarsilastir);

    for (int i = 0; i < dosyaSayisi; i++) {
        if (dosyaAdi != NULL) {
            if (boyut && tarih) {
                char tarihStr[20];
                strftime(tarihStr, sizeof(tarihStr), "%H:%M:%S %d/%m/%Y", localtime(&dosyalar[i].dosyaBilgisi.st_mtime));
                fprintf(dosyaPtr, "%-25s %-10lu bytes %s\n", dosyalar[i].dosyaAdi, (unsigned long)dosyalar[i].dosyaBilgisi.st_size, tarihStr);
            } else if (boyut) {
                fprintf(dosyaPtr, "%-25s %-10lu bytes\n", dosyalar[i].dosyaAdi, (unsigned long)dosyalar[i].dosyaBilgisi.st_size);
            } else if (tarih) {
                char tarihStr[20];
                strftime(tarihStr, sizeof(tarihStr), "%H:%M:%S %d/%m/%Y", localtime(&dosyalar[i].dosyaBilgisi.st_mtime));
                fprintf(dosyaPtr, "%-25s %s\n", dosyalar[i].dosyaAdi, tarihStr);
            } else {
                fprintf(dosyaPtr, "%-25s\n", dosyalar[i].dosyaAdi);
            }
        } else {
            if (boyut && tarih) {
                char tarihStr[20];
                strftime(tarihStr, sizeof(tarihStr), "%H:%M:%S %d/%m/%Y", localtime(&dosyalar[i].dosyaBilgisi.st_mtime));
                printf("%-25s %-10lu bytes %s\n", dosyalar[i].dosyaAdi, (unsigned long)dosyalar[i].dosyaBilgisi.st_size, tarihStr);
            } else if (boyut) {
                printf("%-25s %-10lu bytes\n", dosyalar[i].dosyaAdi, (unsigned long)dosyalar[i].dosyaBilgisi.st_size);
            } else if (tarih) {
                char tarihStr[20];
                strftime(tarihStr, sizeof(tarihStr), "%H:%M:%S %d/%m/%Y", localtime(&dosyalar[i].dosyaBilgisi.st_mtime));
                printf("%-25s %s\n", dosyalar[i].dosyaAdi, tarihStr);
            } else {
                printf("%-25s\n", dosyalar[i].dosyaAdi);
            }
        }
    }

    closedir(dir);

    if (dosyaAdi != NULL) {
        fclose(dosyaPtr);
    }
}

int dosyaKarsilastir(const void *a, const void *b) {
    const DosyaBilgisi *dosyaA = (const DosyaBilgisi *)a;
    const DosyaBilgisi *dosyaB = (const DosyaBilgisi *)b;

    return strcmp(dosyaA->dosyaAdi, dosyaB->dosyaAdi);
}
