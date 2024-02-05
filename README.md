# Dosya Listeleme Projesi

Bu proje, belirli bir dizin içindeki dosyaları listeleme amacıyla yazılmış basit bir C programını içerir.

## Açıklama

Bu C programı, kullanıcı tarafından belirtilen bir dizindeki dosyaları listeler. Program, dosya adı, boyut ve tarih bilgilerini gösterme özelliklerine sahiptir. Ayrıca, gizli dosyaları görüntüleme seçeneği de bulunmaktadır.

## Başlangıç

Projenin yerel bir kopyasını almak ve çalıştırmak için aşağıdaki adımları takip edebilirsiniz.

### Gereksinimler

Projenin çalışması için aşağıdaki yazılımlara ihtiyaç vardır:

- C derleyici (gcc önerilir)
- Linux tabanlı bir işletim sistemi

### Kurulum

1. Bu depoyu klonlayın:

    ```bash
    git clone https://github.com/keremdemirsec/dl.git
    ```

2. Proje klasörüne gidin:

    ```bash
    cd dl
    ```

3. Kodu derleyin:

    ```bash
    gcc -o main main.c
    ```

4. Uygulamayı çalıştırın:

    ```bash
    ./main
    ```

## Kullanım

Programı çalıştırdığınızda, kullanabileceğiniz komut satırı seçenekleri şunlardır:
- `-a`: Gizli dosyaları da listele
- `-s`: Dosya boyutlarını listele
- `-t`: Dosya tarih bilgisini listele
- `-h` veya `--help`: Yardım mesajını göster

Örnek kullanım:

    ./main -a -s -t /home/username/dizin

Katkıda Bulunma

Eğer projeye katkıda bulunmak istiyorsanız, lütfen [CONTRIBUTING.md](CONTRIBUTING.md) dosyasını inceleyin.

Lisans

Bu proje [MIT Lisansı](LICENSE) altında lisanslanmıştır. Detaylar için lisans dosyasını inceleyin.

İletişim

Eğer sorularınız, önerileriniz veya geri bildirimleriniz varsa, bana [e-posta](mailto:keremdemirsec@email.com) üzerinden ulaşabilirsiniz.
