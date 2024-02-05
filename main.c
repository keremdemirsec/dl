#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>
#include <time.h>

void dosyaListele(const char *dizin, int gizli, int boyut, int tarih);

int main(int argc, char *argv[]) {
    const char *dizin = ".";
    int gizli = 0;
    int boyut = 0;
    int tarih = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) {
            gizli = 1;
        } else if (strcmp(argv[i], "-s") == 0) {
            boyut = 1;
        } else if (strcmp(argv[i], "-t") == 0) {
            tarih = 1;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printf("Kullanım: %s [-a] [-s] [-t] [-h|--help] [dizin]\n", argv[0]);
            printf("-a: Gizli dosyaları da listele\n");
            printf("-s: Dosya boyutlarını listele\n");
            printf("-t: Dosya tarih bilgisini listele\n");
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

    dosyaListele(dizin, gizli, boyut, tarih);
    return 0;
}

void dosyaListele(const char *dizin, int gizli, int boyut, int tarih) {
    DIR *dir;
    struct dirent *dosya;
    struct stat dosyaBilgisi;

    if ((dir = opendir(dizin)) == NULL) {
        perror("Dizin açılamadı");
        exit(EXIT_FAILURE);
    }

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

        if (boyut && tarih) {
            char tarihStr[20];
            strftime(tarihStr, sizeof(tarihStr), "%H:%M:%S %d/%m/%Y", localtime(&dosyaBilgisi.st_mtime));
            printf("%-25s %-10lu bytes %s\n", dosya->d_name, (unsigned long)dosyaBilgisi.st_size, tarihStr);
        } else if (boyut) {
            printf("%-25s %-10lu bytes\n", dosya->d_name, (unsigned long)dosyaBilgisi.st_size);
        } else if (tarih) {
            char tarihStr[20];
            strftime(tarihStr, sizeof(tarihStr), "%H:%M:%S %d/%m/%Y", localtime(&dosyaBilgisi.st_mtime));
            printf("%-25s %s\n", dosya->d_name, tarihStr);
        } else {
            printf("%-25s\n", dosya->d_name);
        }
    }

    closedir(dir);
}
