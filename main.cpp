#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <curl/curl.h>

const char *urls[] = {
    "https://yapbenzet.org.tr/1.json",  // 1. URL
    "https://yapbenzet.org.tr/2.json",  // 2. URL
    "https://yapbenzet.org.tr/3.json",  // 3. URL
    "https://yapbenzet.org.tr/4.json",  // 4. URL
    "https://yapbenzet.org.tr/5.json",  // 5. URL
    "https://bilgisayar.kocaeli.edu.tr/1.json",  // 6. URL
    "https://bilgisayar.kocaeli.edu.tr/2.json",  // 7. URL
    "https://bilgisayar.kocaeli.edu.tr/3.json",  // 8. URL
    "https://bilgisayar.kocaeli.edu.tr/4.json",  // 9. URL
    "https://bilgisayar.kocaeli.edu.tr/5.json"   // 10. URL
};

// Toplam URL sayýsý
#define URL_COUNT (sizeof(urls) / sizeof(urls[0]))

// CURL'in yazma fonksiyonu
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *stream) {
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

// urlden json dosyasýný alýyoruz ve temp.json dosyasýna burada yazýrýyoruz
int download_json(const char *url, const char *filename) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        FILE *file = fopen(filename, "wb");
        if (!file) {
            fprintf(stderr, "Dosya açma hatasý: %s\n", filename);
            return -1;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

        // **SSL Doðrulamayý Devre Dýþý Býrakma (Geçici Çözüm)**
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        // CURL isteklerini gerçekleþtir
        res = curl_easy_perform(curl);
        fclose(file);

        if (res != CURLE_OK) {
            fprintf(stderr, "CURL hatasý: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            return -1;
        }

        curl_easy_cleanup(curl);
    }
    return 0;
}



typedef struct {
    int saldiri;
    int savunma;
    int saglik;
    int kritik_sans; 
    int adet;
    float oran;
} Birim;


typedef struct {
    Birim piyadeler;
    Birim okcular;
    Birim suvariler;
    Birim kusatma_makineleri;
} InsanImparatorlugu;

typedef struct {
    Birim ork_dovusculeri;
    Birim mizrakcilar;
    Birim varg_binicileri;
    Birim troller;
} OrkLegi;

typedef struct {
    char bonusTuru[20];
    char bonusDegeri[20];
    char aciklama[100];
    int adet;
} KahramanlarBirim;

typedef struct {
    char etkiDegeri[20];
    char etkiTuru[20];
    char aciklama[100];
    int adet;
} YaratiklarBirim;

typedef struct {
    KahramanlarBirim Alparslan ;
    KahramanlarBirim Fatih_Sultan_Mehmet;
    KahramanlarBirim Mete_Han ;
    KahramanlarBirim Yavuz_Sultan_Selim ;
    KahramanlarBirim Tugrul_Bey;
}KahramanlarInsan;

typedef struct {
    KahramanlarBirim Goruk_Vahsi;
    KahramanlarBirim Thruk_Kemikkiran;
    KahramanlarBirim Vrog_Kafakiran ;
    KahramanlarBirim Ugar_Zalim ;
}KahramanlarOrk;

typedef struct {
    YaratiklarBirim Ejderha;
    YaratiklarBirim Agri_Dagi_Devleri;
    YaratiklarBirim Tepegoz;
    YaratiklarBirim Karakurt;
    YaratiklarBirim Samur;
}YaratiklarInsan;

typedef struct {
    YaratiklarBirim Kara_Troll;
    YaratiklarBirim Golge_Kurtlari;
    YaratiklarBirim Camur_Devleri;
    YaratiklarBirim Ates_Iblisi;
    YaratiklarBirim Makrog_Savas_Beyi;
    YaratiklarBirim Buz_Devleri;
}YaratiklarOrk;

typedef struct {
    char arastirma_adi[70];
    int seviye_adi;
    char deger[10];
    char aciklama[256];
    int adet; 
    char birlik[10];
    char birim[20];
    char etkiAlani[30]; 
} Seviye;

typedef struct {
    char arastirma_adi[70];
    Seviye arastirma[3];
} Yetenekler;

void parse_seviye(char* line, Seviye* seviye) {
    if (strstr(line, "\"deger\":")) {
        sscanf(line, " \"deger\": \"%[^\"]\",", seviye->deger);
    } else if (strstr(line, "\"aciklama\":")) {
        sscanf(line, " \"aciklama\": \"%[^\"]\"", seviye->aciklama);

        
        if (strstr(seviye->aciklama, "tum_birimler")) {
            strcpy(seviye->birim, "tum_birimler");
        } else if (strstr(seviye->aciklama, "kusatma_makineleri")) {
            strcpy(seviye->birim, "kusatma_makineleri");
        } else if (strstr(seviye->aciklama, "piyade")) {
            strcpy(seviye->birim, "piyadeler");
        } else if (strstr(seviye->aciklama, "okcu")) {
            strcpy(seviye->birim, "okcular");
        } else if (strstr(seviye->aciklama, "suvari")) {
            strcpy(seviye->birim, "suvariler");
        } else if (strstr(seviye->aciklama, "ork_dovusculeri")) {
            strcpy(seviye->birim, "ork_dovusculeri");
        } else if (strstr(seviye->aciklama, "mizrakcilar")) {
            strcpy(seviye->birim, "mizrakcilar");
        } else if (strstr(seviye->aciklama, "varg_binicileri")) {
            strcpy(seviye->birim, "varg_binicileri");
        } else if (strstr(seviye->aciklama, "troller")) {
            strcpy(seviye->birim, "troller");
        } else {
            strcpy(seviye->birim, "bilinmeyen"); 
        }

        
        if (strstr(seviye->aciklama, "savunma")) {
            strcpy(seviye->etkiAlani, "savunma");
        } else if (strstr(seviye->aciklama, "saldiri")) {
            strcpy(seviye->etkiAlani, "saldiri");
        } else if (strstr(seviye->aciklama, "saglik")) {
            strcpy(seviye->etkiAlani, "saglik");
        } else if (strstr(seviye->aciklama, "kritik")) {
            strcpy(seviye->etkiAlani, "kritik_sans");
        } else {
            strcpy(seviye->etkiAlani, "bilinmeyen"); 
        }
    }
}

void parse_json_file3(const char* filename, Yetenekler* yetenekler, int* yetenek_sayisi) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Dosya açýlamadý: %s\n", filename);
        return;
    }

    char line[1024];
    Yetenekler* current_yetenek = NULL;
    int index = 0;

    while (fgets(line, sizeof(line), file)) {
        
        if (strstr(line, "\"") && strstr(line, "{") && !strstr(line, "\"seviye_")) {
            sscanf(line, " \"%[^\"]\": {", yetenekler[index].arastirma_adi);
            current_yetenek = &yetenekler[index];
            index++;
        } else if (current_yetenek != NULL) {
            
            for (int i = 0; i < 3; i++) {
                char seviye_label[12];
                snprintf(seviye_label, sizeof(seviye_label), "\"seviye_%d\":", i + 1);

                if (strstr(line, seviye_label)) {
                    current_yetenek->arastirma[i].seviye_adi = i + 1;

                    
                    while (fgets(line, sizeof(line), file)) {
                        if (strstr(line, "}")) break;
                        parse_seviye(line, &current_yetenek->arastirma[i]);
                    }
                    break;
                }
            }
        }
    }

    *yetenek_sayisi = index;
    fclose(file);
}


void yazdir_yetenekler(Yetenekler* yetenek, int yetenek_sayisi) {
    for (int i = 0; i < yetenek_sayisi; i++) {
        printf("=== %s ===\n", yetenek[i].arastirma_adi);
        for (int j = 0; j < 3; j++) {
            printf("Seviye: %d\n", yetenek[i].arastirma[j].seviye_adi);
            printf("Deðer: %s\n", yetenek[i].arastirma[j].deger);
            printf("Açýklama: %s\n", yetenek[i].arastirma[j].aciklama);
            printf("Adet: %d\n", yetenek[i].arastirma[j].adet);
            printf("Birlik: %s\n", yetenek[i].arastirma[j].birlik);
            printf("Birim: %s\n", yetenek[i].arastirma[j].birim); 
            printf("Etki Alaný: %s\n", yetenek[i].arastirma[j].etkiAlani);
            printf("--------------------------\n");
        }
    }
}



void parse_json_file4(const char* filename, InsanImparatorlugu* insan, OrkLegi* ork,
                       KahramanlarInsan* kInsan, KahramanlarOrk* kOrk,
                       YaratiklarInsan* yInsan, YaratiklarOrk* yOrk,
                       Yetenekler* yetenek, int yetenek_sayisi) {

    
    insan->piyadeler.adet = 0;
    insan->okcular.adet = 0;
    insan->kusatma_makineleri.adet = 0;
    insan->suvariler.adet=0;

    ork->ork_dovusculeri.adet = 0;
    ork->mizrakcilar.adet = 0;
    ork->troller.adet = 0;

    kInsan->Alparslan.adet = 0;
    kInsan->Fatih_Sultan_Mehmet.adet = 0;
    kInsan->Mete_Han.adet = 0;
    kInsan->Yavuz_Sultan_Selim.adet = 0;
    kInsan->Tugrul_Bey.adet = 0;

    kOrk->Goruk_Vahsi.adet = 0;
    kOrk->Thruk_Kemikkiran.adet = 0;
    kOrk->Vrog_Kafakiran.adet = 0;
    kOrk->Ugar_Zalim.adet = 0;

    yInsan->Ejderha.adet = 0;
    yInsan->Agri_Dagi_Devleri.adet = 0;
    yInsan->Tepegoz.adet = 0;
    yInsan->Karakurt.adet = 0;
    yInsan->Samur.adet = 0;

    yOrk->Kara_Troll.adet = 0;
    yOrk->Golge_Kurtlari.adet = 0;
    yOrk->Camur_Devleri.adet = 0;
    yOrk->Ates_Iblisi.adet = 0;
    yOrk->Makrog_Savas_Beyi.adet = 0;
    yOrk->Buz_Devleri.adet = 0;

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Dosya açýlamadý: %s\n", filename);
        return;
    }

    char line[1024];
    char current_imparatorluk[20] = "";

    while (fgets(line, sizeof(line), file)) {
        // ** Ýnsan Birimleri **
        if (strstr(line, "\"piyadeler\":")) {
            sscanf(line, " \"piyadeler\": %d,", &insan->piyadeler.adet);
        } else if (strstr(line, "\"okcular\":")) {
            sscanf(line, " \"okcular\": %d,", &insan->okcular.adet);
        } else if (strstr(line, "\"kusatma_makineleri\":")) {
            sscanf(line, " \"kusatma_makineleri\": %d,", &insan->kusatma_makineleri.adet);
        } else if (strstr(line, "\"suvariler\":")) {
            sscanf(line, " \"suvariler\": %d,", &insan->suvariler.adet);
        }

        // ** Ork Birimleri **
        if (strstr(line, "\"ork_dovusculeri\":")) {
            sscanf(line, " \"ork_dovusculeri\": %d,", &ork->ork_dovusculeri.adet);
        } else if (strstr(line, "\"mizrakcilar\":")) {
            sscanf(line, " \"mizrakcilar\": %d,", &ork->mizrakcilar.adet);
        } else if (strstr(line, "\"troller\":")) {
            sscanf(line, " \"troller\": %d,", &ork->troller.adet);
        } else if (strstr(line, "\"varg_binicileri\":")) {
            sscanf(line, " \"varg_binicileri\": %d,", &ork->troller.adet);
        }

        
        if (strstr(line, "\"kahramanlar\":")) {
            char* token = strtok(line, "[\", ]");
            while (token != NULL) {
                if (strcmp(token, "Alparslan") == 0) kInsan->Alparslan.adet = 1;
                else if (strcmp(token, "Fatih_Sultan_Mehmet") == 0) kInsan->Fatih_Sultan_Mehmet.adet = 1;
                else if (strcmp(token, "Mete_Han") == 0) kInsan->Mete_Han.adet = 1;
                else if (strcmp(token, "Yavuz_Sultan_Selim") == 0) kInsan->Yavuz_Sultan_Selim.adet = 1;
                else if (strcmp(token, "Tugrul_Bey") == 0) kInsan->Tugrul_Bey.adet = 1;

                else if (strcmp(token, "Goruk_Vahsi") == 0) kOrk->Goruk_Vahsi.adet = 1;
                else if (strcmp(token, "Thruk_Kemikkiran") == 0) kOrk->Thruk_Kemikkiran.adet = 1;
                else if (strcmp(token, "Vrog_Kafakiran") == 0) kOrk->Vrog_Kafakiran.adet = 1;
                else if (strcmp(token, "Ugar_Zalim") == 0) kOrk->Ugar_Zalim.adet = 1;

                token = strtok(NULL, "[\", ]");
            }
        }

        if (strstr(line, "\"canavarlar\":")) {
            char* token = strtok(line, "[\", ]");
            while (token != NULL) {
                if (strcmp(token, "Ejderha") == 0) yInsan->Ejderha.adet = 1;
                else if (strcmp(token, "Agri_Dagi_Devleri") == 0) yInsan->Agri_Dagi_Devleri.adet = 1;
                else if (strcmp(token, "Tepegoz") == 0) yInsan->Tepegoz.adet = 1;
                else if (strcmp(token, "Karakurt") == 0) yInsan->Karakurt.adet = 1;
                else if (strcmp(token, "Samur") == 0) yInsan->Samur.adet = 1;

                else if (strcmp(token, "Kara_Troll") == 0) yOrk->Kara_Troll.adet = 1;
                else if (strcmp(token, "Golge_Kurtlari") == 0) yOrk->Golge_Kurtlari.adet = 1;
                else if (strcmp(token, "Camur_Devleri") == 0) yOrk->Camur_Devleri.adet = 1;
                else if (strcmp(token, "Ates_Iblisi") == 0) yOrk->Ates_Iblisi.adet = 1;
                else if (strcmp(token, "Makrog_Savas_Beyi") == 0) yOrk->Makrog_Savas_Beyi.adet = 1;
                else if (strcmp(token, "Buz_Devleri") == 0) yOrk->Buz_Devleri.adet = 1;

                token = strtok(NULL, "[\", ]");
            }
        }

       
        if (strstr(line, "insan_imparatorlugu")) {
            strcpy(current_imparatorluk, "insan");
        } else if (strstr(line, "ork_legi")) {
            strcpy(current_imparatorluk, "ork");
        }

       
        if (strstr(line, "\"arastirma_seviyesi\":")) {
            char birlik[10]; 
            strcpy(birlik, current_imparatorluk); 

            while (fgets(line, sizeof(line), file) && !strstr(line, "}")) {
                char arastirma_adi[50];
                int seviye;

                sscanf(line, " \"%[^\"]\": %d,", arastirma_adi, &seviye);

                for (int i = 0; i < yetenek_sayisi; i++) {
                    if (strcmp(yetenek[i].arastirma_adi, arastirma_adi) == 0) {
                        for (int j = 0; j < 3; j++) {
                            if (yetenek[i].arastirma[j].seviye_adi == seviye) {
                                yetenek[i].arastirma[j].adet = 1;
                                strcpy(yetenek[i].arastirma[j].birlik, birlik); 
                                break; 
                            }
                        }
                    }
                }
            }
        }
    }

    fclose(file);
}


void parse_json_file1(const char* filename, KahramanlarInsan* kInsan, KahramanlarOrk* kOrk) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Dosya açýlamadý: %s\n", filename);
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        
        if (strstr(line, "\"Alparslan\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break; 
                if (strstr(line, "\"bonus_turu\":")) sscanf(line, " \"bonus_turu\": \"%[^\"]\",", kInsan->Alparslan.bonusTuru);
                else if (strstr(line, "\"bonus_degeri\":")) sscanf(line, " \"bonus_degeri\": \"%[^\"]\",", kInsan->Alparslan.bonusDegeri);
                else if (strstr(line, "\"aciklama\":")) sscanf(line, " \"aciklama\": \"%[^\"]\"", kInsan->Alparslan.aciklama);
            }
        }
        else if (strstr(line, "\"Fatih_Sultan_Mehmet\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break; 
                if (strstr(line, "\"bonus_turu\":")) sscanf(line, " \"bonus_turu\": \"%[^\"]\",", kInsan->Fatih_Sultan_Mehmet.bonusTuru);
                else if (strstr(line, "\"bonus_degeri\":")) sscanf(line, " \"bonus_degeri\": \"%[^\"]\",", kInsan->Fatih_Sultan_Mehmet.bonusDegeri);
                else if (strstr(line, "\"aciklama\":")) sscanf(line, " \"aciklama\": \"%[^\"]\"", kInsan->Fatih_Sultan_Mehmet.aciklama);
            }
        }
        else if (strstr(line, "\"Mete_Han\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break; 
                if (strstr(line, "\"bonus_turu\":")) sscanf(line, " \"bonus_turu\": \"%[^\"]\",", kInsan->Mete_Han.bonusTuru);
                else if (strstr(line, "\"bonus_degeri\":")) sscanf(line, " \"bonus_degeri\": \"%[^\"]\",", kInsan->Mete_Han.bonusDegeri);
                else if (strstr(line, "\"aciklama\":")) sscanf(line, " \"aciklama\": \"%[^\"]\"", kInsan->Mete_Han.aciklama);
            }
        }
        else if (strstr(line, "\"Yavuz_Sultan_Selim\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break; 
                if (strstr(line, "\"bonus_turu\":")) sscanf(line, " \"bonus_turu\": \"%[^\"]\",", kInsan->Yavuz_Sultan_Selim.bonusTuru);
                else if (strstr(line, "\"bonus_degeri\":")) sscanf(line, " \"bonus_degeri\": \"%[^\"]\",", kInsan->Yavuz_Sultan_Selim.bonusDegeri);
                else if (strstr(line, "\"aciklama\":")) sscanf(line, " \"aciklama\": \"%[^\"]\"", kInsan->Yavuz_Sultan_Selim.aciklama);
            }
        }
        else if (strstr(line, "\"Tugrul_Bey\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break; 
                if (strstr(line, "\"bonus_turu\":")) sscanf(line, " \"bonus_turu\": \"%[^\"]\",", kInsan->Tugrul_Bey.bonusTuru);
                else if (strstr(line, "\"bonus_degeri\":")) sscanf(line, " \"bonus_degeri\": \"%[^\"]\",", kInsan->Tugrul_Bey.bonusDegeri);
                else if (strstr(line, "\"aciklama\":")) sscanf(line, " \"aciklama\": \"%[^\"]\"", kInsan->Tugrul_Bey.aciklama);
            }
        }
        else if (strstr(line, "\"Goruk_Vahsi\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break; 
                if (strstr(line, "\"bonus_turu\":")) sscanf(line, " \"bonus_turu\": \"%[^\"]\",", kOrk->Goruk_Vahsi.bonusTuru);
                else if (strstr(line, "\"bonus_degeri\":")) sscanf(line, " \"bonus_degeri\": \"%[^\"]\",", kOrk->Goruk_Vahsi.bonusDegeri);
                else if (strstr(line, "\"aciklama\":")) sscanf(line, " \"aciklama\": \"%[^\"]\"", kOrk->Goruk_Vahsi.aciklama);
            }
        }
        else if (strstr(line, "\"Thruk_Kemikkiran\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break; 
                if (strstr(line, "\"bonus_turu\":")) sscanf(line, " \"bonus_turu\": \"%[^\"]\",", kOrk->Thruk_Kemikkiran.bonusTuru);
                else if (strstr(line, "\"bonus_degeri\":")) sscanf(line, " \"bonus_degeri\": \"%[^\"]\",", kOrk->Thruk_Kemikkiran.bonusDegeri);
                else if (strstr(line, "\"aciklama\":")) sscanf(line, " \"aciklama\": \"%[^\"]\"", kOrk->Thruk_Kemikkiran.aciklama);
            }
        }
        else if (strstr(line, "\"Vrog_Kafakiran\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break; 
                if (strstr(line, "\"bonus_turu\":")) sscanf(line, " \"bonus_turu\": \"%[^\"]\",", kOrk->Vrog_Kafakiran.bonusTuru);
                else if (strstr(line, "\"bonus_degeri\":")) sscanf(line, " \"bonus_degeri\": \"%[^\"]\",", kOrk->Vrog_Kafakiran.bonusDegeri);
                else if (strstr(line, "\"aciklama\":")) sscanf(line, " \"aciklama\": \"%[^\"]\"", kOrk->Vrog_Kafakiran.aciklama);
            }
        }
        else if (strstr(line, "\"Ugar_Zalim\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break; 
                if (strstr(line, "\"bonus_turu\":")) sscanf(line, " \"bonus_turu\": \"%[^\"]\",", kOrk->Ugar_Zalim.bonusTuru);
                else if (strstr(line, "\"bonus_degeri\":")) sscanf(line, " \"bonus_degeri\": \"%[^\"]\",", kOrk->Ugar_Zalim.bonusDegeri);
                else if (strstr(line, "\"aciklama\":")) sscanf(line, " \"aciklama\": \"%[^\"]\"", kOrk->Ugar_Zalim.aciklama);
            }
        }
    }

    fclose(file);
}

void parse_json_file2(const char* filename, YaratiklarInsan* yInsan, YaratiklarOrk* yOrk) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Dosya açýlamadý: %s\n", filename);
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        
        if (strstr(line, "\"Ejderha\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break; 
                if (strstr(line, "\"etki_degeri\":")) sscanf(line, " \"etki_degeri\": \"%[^\"]\",", yInsan->Ejderha.etkiDegeri);
                else if (strstr(line, "\"etki_turu\":")) sscanf(line, " \"etki_turu\": \"%[^\"]\",", yInsan->Ejderha.etkiTuru);
                else if (strstr(line, "\"aciklama\":")) sscanf(line, " \"aciklama\": \"%[^\"]\"", yInsan->Ejderha.aciklama);
            }
        }
        else if (strstr(line, "\"Agri_Dagi_Devleri\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break; 
                if (strstr(line, "\"etki_degeri\":")) sscanf(line, " \"etki_degeri\": \"%[^\"]\",", yInsan->Agri_Dagi_Devleri.etkiDegeri);
                else if (strstr(line, "\"etki_turu\":")) sscanf(line, " \"etki_turu\": \"%[^\"]\",", yInsan->Agri_Dagi_Devleri.etkiTuru);
                else if (strstr(line, "\"aciklama\":")) sscanf(line, " \"aciklama\": \"%[^\"]\"", yInsan->Agri_Dagi_Devleri.aciklama);
            }
        }
        else if (strstr(line, "\"Tepegoz\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break; 
                if (strstr(line, "\"etki_degeri\":")) sscanf(line, " \"etki_degeri\": \"%[^\"]\",", yInsan->Tepegoz.etkiDegeri);
                else if (strstr(line, "\"etki_turu\":")) sscanf(line, " \"etki_turu\": \"%[^\"]\",", yInsan->Tepegoz.etkiTuru);
                else if (strstr(line, "\"aciklama\":")) sscanf(line, " \"aciklama\": \"%[^\"]\"", yInsan->Tepegoz.aciklama);
            }
        }
        else if (strstr(line, "\"Karakurt\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break; 
                if (strstr(line, "\"etki_degeri\":")) sscanf(line, " \"etki_degeri\": \"%[^\"]\",", yInsan->Karakurt.etkiDegeri);
                else if (strstr(line, "\"etki_turu\":")) sscanf(line, " \"etki_turu\": \"%[^\"]\",", yInsan->Karakurt.etkiTuru);
                else if (strstr(line, "\"aciklama\":")) sscanf(line, " \"aciklama\": \"%[^\"]\"", yInsan->Karakurt.aciklama);
            }
        }
        else if (strstr(line, "\"Samur\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break; 
                if (strstr(line, "\"etki_degeri\":")) sscanf(line, " \"etki_degeri\": \"%[^\"]\",", yInsan->Samur.etkiDegeri);
                else if (strstr(line, "\"etki_turu\":")) sscanf(line, " \"etki_turu\": \"%[^\"]\",", yInsan->Samur.etkiTuru);
                else if (strstr(line, "\"aciklama\":")) sscanf(line, " \"aciklama\": \"%[^\"]\"", yInsan->Samur.aciklama);
            }
        }
        else if (strstr(line, "\"Kara_Troll\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break; 
                if (strstr(line, "\"etki_degeri\":")) sscanf(line, " \"etki_degeri\": \"%[^\"]\",", yOrk->Kara_Troll.etkiDegeri);
                else if (strstr(line, "\"etki_turu\":")) sscanf(line, " \"etki_turu\": \"%[^\"]\",", yOrk->Kara_Troll.etkiTuru);
                else if (strstr(line, "\"aciklama\":")) sscanf(line, " \"aciklama\": \"%[^\"]\"", yOrk->Kara_Troll.aciklama);
            }
        }
        else if (strstr(line, "\"Golge_Kurtlari\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break; 
                if (strstr(line, "\"etki_degeri\":")) sscanf(line, " \"etki_degeri\": \"%[^\"]\",", yOrk->Golge_Kurtlari.etkiDegeri);
                else if (strstr(line, "\"etki_turu\":")) sscanf(line, " \"etki_turu\": \"%[^\"]\",", yOrk->Golge_Kurtlari.etkiTuru);
                else if (strstr(line, "\"aciklama\":")) sscanf(line, " \"aciklama\": \"%[^\"]\"", yOrk->Golge_Kurtlari.aciklama);
            }
        }
        else if (strstr(line, "\"Camur_Devleri\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break; 
                if (strstr(line, "\"etki_degeri\":")) sscanf(line, " \"etki_degeri\": \"%[^\"]\",", yOrk->Camur_Devleri.etkiDegeri);
                else if (strstr(line, "\"etki_turu\":")) sscanf(line, " \"etki_turu\": \"%[^\"]\",", yOrk->Camur_Devleri.etkiTuru);
                else if (strstr(line, "\"aciklama\":")) sscanf(line, " \"aciklama\": \"%[^\"]\"", yOrk->Camur_Devleri.aciklama);
            }
        }
        else if (strstr(line, "\"Ates_Iblisi\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break; 
                if (strstr(line, "\"etki_degeri\":")) sscanf(line, " \"etki_degeri\": \"%[^\"]\",", yOrk->Ates_Iblisi.etkiDegeri);
                else if (strstr(line, "\"etki_turu\":")) sscanf(line, " \"etki_turu\": \"%[^\"]\",", yOrk->Ates_Iblisi.etkiTuru);
                else if (strstr(line, "\"aciklama\":")) sscanf(line, " \"aciklama\": \"%[^\"]\"", yOrk->Ates_Iblisi.aciklama);
            }
        }
        else if (strstr(line, "\"Makrog_Savas_Beyi\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break; 
                if (strstr(line, "\"etki_degeri\":")) sscanf(line, " \"etki_degeri\": \"%[^\"]\",", yOrk->Makrog_Savas_Beyi.etkiDegeri);
                else if (strstr(line, "\"etki_turu\":")) sscanf(line, " \"etki_turu\": \"%[^\"]\",", yOrk->Makrog_Savas_Beyi.etkiTuru);
                else if (strstr(line, "\"aciklama\":")) sscanf(line, " \"aciklama\": \"%[^\"]\"", yOrk->Makrog_Savas_Beyi.aciklama);
            }
        }
        else if (strstr(line, "\"Buz_Devleri\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break; 
                if (strstr(line, "\"etki_degeri\":")) sscanf(line, " \"etki_degeri\": \"%[^\"]\",", yOrk->Buz_Devleri.etkiDegeri);
                else if (strstr(line, "\"etki_turu\":")) sscanf(line, " \"etki_turu\": \"%[^\"]\",", yOrk->Buz_Devleri.etkiTuru);
                else if (strstr(line, "\"aciklama\":")) sscanf(line, " \"aciklama\": \"%[^\"]\"", yOrk->Buz_Devleri.aciklama);
            }
        }
    }

    fclose(file);
}

void parse_json_file(const char* filename, InsanImparatorlugu* insan, OrkLegi* ork) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Dosya açýlamadý: %s\n", filename);
        return;
    }

        insan->suvariler.adet=0;

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        
        if (strstr(line, "\"piyadeler\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break; 
                if (strstr(line, "\"saldiri\":")) sscanf(line, " \"saldiri\": %d,", &insan->piyadeler.saldiri);
                else if (strstr(line, "\"savunma\":")) sscanf(line, " \"savunma\": %d,", &insan->piyadeler.savunma);
                else if (strstr(line, "\"saglik\":")) sscanf(line, " \"saglik\": %d,", &insan->piyadeler.saglik);
                else if (strstr(line, "\"kritik_sans\":")) sscanf(line, " \"kritik_sans\": %d", &insan->piyadeler.kritik_sans);
            }
        }
        else if (strstr(line, "\"okcular\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break;
                if (strstr(line, "\"saldiri\":")) sscanf(line, " \"saldiri\": %d,", &insan->okcular.saldiri);
                else if (strstr(line, "\"savunma\":")) sscanf(line, " \"savunma\": %d,", &insan->okcular.savunma);
                else if (strstr(line, "\"saglik\":")) sscanf(line, " \"saglik\": %d,", &insan->okcular.saglik);
                else if (strstr(line, "\"kritik_sans\":")) sscanf(line, " \"kritik_sans\": %d", &insan->okcular.kritik_sans);
            }
        }
        else if (strstr(line, "\"suvariler\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break;
                if (strstr(line, "\"saldiri\":")) sscanf(line, " \"saldiri\": %d,", &insan->suvariler.saldiri);
                else if (strstr(line, "\"savunma\":")) sscanf(line, " \"savunma\": %d,", &insan->suvariler.savunma);
                else if (strstr(line, "\"saglik\":")) sscanf(line, " \"saglik\": %d,", &insan->suvariler.saglik);
                else if (strstr(line, "\"kritik_sans\":")) sscanf(line, " \"kritik_sans\": %d", &insan->suvariler.kritik_sans);
            }
        }
        else if (strstr(line, "\"kusatma_makineleri\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break;
                if (strstr(line, "\"saldiri\":")) sscanf(line, " \"saldiri\": %d,", &insan->kusatma_makineleri.saldiri);
                else if (strstr(line, "\"savunma\":")) sscanf(line, " \"savunma\": %d,", &insan->kusatma_makineleri.savunma);
                else if (strstr(line, "\"saglik\":")) sscanf(line, " \"saglik\": %d,", &insan->kusatma_makineleri.saglik);
                else if (strstr(line, "\"kritik_sans\":")) sscanf(line, " \"kritik_sans\": %d", &insan->kusatma_makineleri.kritik_sans);
            }
        }
        else if (strstr(line, "\"ork_dovusculeri\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break;
                if (strstr(line, "\"saldiri\":")) sscanf(line, " \"saldiri\": %d,", &ork->ork_dovusculeri.saldiri);
                else if (strstr(line, "\"savunma\":")) sscanf(line, " \"savunma\": %d,", &ork->ork_dovusculeri.savunma);
                else if (strstr(line, "\"saglik\":")) sscanf(line, " \"saglik\": %d,", &ork->ork_dovusculeri.saglik);
                else if (strstr(line, "\"kritik_sans\":")) sscanf(line, " \"kritik_sans\": %d", &ork->ork_dovusculeri.kritik_sans);
            }
        }
        else if (strstr(line, "\"mizrakcilar\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break;
                if (strstr(line, "\"saldiri\":")) sscanf(line, " \"saldiri\": %d,", &ork->mizrakcilar.saldiri);
                else if (strstr(line, "\"savunma\":")) sscanf(line, " \"savunma\": %d,", &ork->mizrakcilar.savunma);
                else if (strstr(line, "\"saglik\":")) sscanf(line, " \"saglik\": %d,", &ork->mizrakcilar.saglik);
                else if (strstr(line, "\"kritik_sans\":")) sscanf(line, " \"kritik_sans\": %d", &ork->mizrakcilar.kritik_sans);
            }
        }
        else if (strstr(line, "\"varg_binicileri\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break;
                if (strstr(line, "\"saldiri\":")) sscanf(line, " \"saldiri\": %d,", &ork->varg_binicileri.saldiri);
                else if (strstr(line, "\"savunma\":")) sscanf(line, " \"savunma\": %d,", &ork->varg_binicileri.savunma);
                else if (strstr(line, "\"saglik\":")) sscanf(line, " \"saglik\": %d,", &ork->varg_binicileri.saglik);
                else if (strstr(line, "\"kritik_sans\":")) sscanf(line, " \"kritik_sans\": %d", &ork->varg_binicileri.kritik_sans);
            }
        }
        else if (strstr(line, "\"troller\":")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break;
                if (strstr(line, "\"saldiri\":")) sscanf(line, " \"saldiri\": %d,", &ork->troller.saldiri);
                else if (strstr(line, "\"savunma\":")) sscanf(line, " \"savunma\": %d,", &ork->troller.savunma);
                else if (strstr(line, "\"saglik\":")) sscanf(line, " \"saglik\": %d,", &ork->troller.saglik);
                else if (strstr(line, "\"kritik_sans\":")) sscanf(line, " \"kritik_sans\": %d", &ork->troller.kritik_sans);
            }
        }
    }

    fclose(file);
}

void guncelle_insan_birimleri(const char* birim, float deger, const char* etkiAlani, InsanImparatorlugu* insan) {
    Birim* selectedBirim = NULL;

    
    if (strcmp(birim, "piyadeler") == 0) {
        selectedBirim = &insan->piyadeler;
    } else if (strcmp(birim, "okcular") == 0) {
        selectedBirim = &insan->okcular;
    } else if (strcmp(birim, "suvariler") == 0) {
        selectedBirim = &insan->suvariler;
    } else if (strcmp(birim, "kusatma_makineleri") == 0) {
        selectedBirim = &insan->kusatma_makineleri;
    }

    
    if (selectedBirim != NULL) {
        if (strcmp(etkiAlani, "saldiri") == 0) {
            selectedBirim->saldiri *= (1 + deger * 0.01); 
        } else if (strcmp(etkiAlani, "savunma") == 0) {
            selectedBirim->savunma *= (1 + deger * 0.01); 
        } else if (strcmp(etkiAlani, "saglik") == 0) {
            selectedBirim->saglik *= (1 + deger * 0.01); 
        } else if (strcmp(etkiAlani, "kritik_sans") == 0) {
            selectedBirim->kritik_sans *= (1 + deger * 0.01); 
        }

        
        selectedBirim->oran *= (1 + deger * 0.01);

       /* printf("%s icin yeni saldiri: %d, savunma: %d, saglik: %d, kritik sans: %d, oran: %.2f\n",
               birim, selectedBirim->saldiri, selectedBirim->savunma,
               selectedBirim->saglik, selectedBirim->kritik_sans, selectedBirim->oran);*/
    }
}

void guncelle_ork_birimleri(const char* birim, float deger, const char* etkiAlani, OrkLegi* ork) {
    Birim* selectedBirim = NULL;

    if (strcmp(birim, "ork_dovusculeri") == 0) {
        selectedBirim = &ork->ork_dovusculeri;
    } else if (strcmp(birim, "mizrakcilar") == 0) {
        selectedBirim = &ork->mizrakcilar;
    } else if (strcmp(birim, "varg_binicileri") == 0) {
        selectedBirim = &ork->varg_binicileri;
    } else if (strcmp(birim, "troller") == 0) {
        selectedBirim = &ork->troller;
    }

    // Güncellemeyi yap
    if (selectedBirim != NULL) {
        if (strcmp(etkiAlani, "saldiri") == 0) {
            selectedBirim->saldiri *= (1 + deger * 0.01); 
        } else if (strcmp(etkiAlani, "savunma") == 0) {
            selectedBirim->savunma *= (1 + deger * 0.01);
        } else if (strcmp(etkiAlani, "saglik") == 0) {
            selectedBirim->saglik *= (1 + deger * 0.01); 
        } else if (strcmp(etkiAlani, "kritik_sans") == 0) {
            selectedBirim->kritik_sans *= (1 + deger * 0.01); 
        }

        
        selectedBirim->oran *= (1 + deger * 0.01);

        /*printf("%s icin yeni saldiri: %d, savunma: %d, saglik: %d, kritik sans: %d, oran: %.2f\n",
               birim, selectedBirim->saldiri, selectedBirim->savunma,
               selectedBirim->saglik, selectedBirim->kritik_sans, selectedBirim->oran);*/
    }
}

void yetenek_etkilerini_uygula(Yetenekler* yetenekler, int yetenek_sayisi, InsanImparatorlugu* insan, OrkLegi* ork) {
    for (int i = 0; i < yetenek_sayisi; i++) {
        for (int j = 0; j < 3; j++) {
            Seviye* seviye = &yetenekler[i].arastirma[j];
            if (seviye->adet == 1) { // Adet 1 ise
                if (strcmp(seviye->birlik, "insan") == 0) {
                    if (strcmp(seviye->birim, "tum_birimler") == 0) {
                        
                        guncelle_insan_birimleri("piyadeler", atof(seviye->deger), seviye->etkiAlani, insan);
                        guncelle_insan_birimleri("okcular", atof(seviye->deger), seviye->etkiAlani, insan);
                        guncelle_insan_birimleri("suvariler", atof(seviye->deger), seviye->etkiAlani, insan);
                        guncelle_insan_birimleri("kusatma_makineleri", atof(seviye->deger), seviye->etkiAlani, insan);
                    } else {
                        
                        guncelle_insan_birimleri(seviye->birim, atof(seviye->deger), seviye->etkiAlani, insan);
                    }
                } else if (strcmp(seviye->birlik, "ork") == 0) {
                    if (strcmp(seviye->birim, "tum_birimler") == 0) {
                        
                        guncelle_ork_birimleri("ork_dovusculeri", atof(seviye->deger), seviye->etkiAlani, ork);
                        guncelle_ork_birimleri("mizrakcilar", atof(seviye->deger), seviye->etkiAlani, ork);
                        guncelle_ork_birimleri("varg_binicileri", atof(seviye->deger), seviye->etkiAlani, ork);
                        guncelle_ork_birimleri("troller", atof(seviye->deger), seviye->etkiAlani, ork);
                    } else {
                        
                        guncelle_ork_birimleri(seviye->birim, atof(seviye->deger), seviye->etkiAlani, ork);
                    }
                } else {
                    
                   // printf("Gecersiz birlik: %s\n", seviye->birlik);
                }
            } else {
        
               // printf("Adet 1 degil: %d\n", seviye->adet);
            }
        }
    }
}

void yeniden_hasar_dagitim_insan(InsanImparatorlugu* insan, int kalanHasar) {
    
    float toplamSavunma =
        insan->kusatma_makineleri.savunma * insan->kusatma_makineleri.adet +
        insan->okcular.savunma * insan->okcular.adet +
        insan->piyadeler.savunma * insan->piyadeler.adet +
        insan->suvariler.savunma * insan->suvariler.adet;

    if (toplamSavunma == 0) return; 

    
    insan->kusatma_makineleri.oran =
        (insan->kusatma_makineleri.savunma * insan->kusatma_makineleri.adet) / toplamSavunma;

    insan->okcular.oran =
        (insan->okcular.savunma * insan->okcular.adet) / toplamSavunma;

    insan->piyadeler.oran =
        (insan->piyadeler.savunma * insan->piyadeler.adet) / toplamSavunma;

    insan->suvariler.oran =
        (insan->suvariler.savunma * insan->suvariler.adet) / toplamSavunma;

   
    insan->kusatma_makineleri.adet -=
        (kalanHasar * insan->kusatma_makineleri.oran) / insan->kusatma_makineleri.saglik;

    insan->okcular.adet -=
        (kalanHasar * insan->okcular.oran) / insan->okcular.saglik;

    insan->piyadeler.adet -=
        (kalanHasar * insan->piyadeler.oran) / insan->piyadeler.saglik;

    insan->suvariler.adet -=
        (kalanHasar * insan->suvariler.oran) / insan->suvariler.saglik;
}

void kahraman_etkilerini_uygula_insan(InsanImparatorlugu* insan, KahramanlarInsan* kInsan) {
   
    KahramanlarBirim* kahramanlar[] = {
        &kInsan->Alparslan,
        &kInsan->Fatih_Sultan_Mehmet,
        &kInsan->Mete_Han,
        &kInsan->Yavuz_Sultan_Selim,
        &kInsan->Tugrul_Bey
    };

    for (int i = 0; i < 5; i++) {
      
        if (kahramanlar[i]->adet != 1) {
            continue;
        }

      
        int etki_degeri = atoi(kahramanlar[i]->bonusDegeri);

      
        if (strcmp(kahramanlar[i]->bonusTuru, "saldiri") == 0) {
            if (strstr(kahramanlar[i]->aciklama, "piyadeler") != NULL) {
                insan->piyadeler.saldiri += (insan->piyadeler.saldiri * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "okcular") != NULL) {
                insan->okcular.saldiri += (insan->okcular.saldiri * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "kusatma_makineleri") != NULL) {
                insan->kusatma_makineleri.saldiri += (insan->kusatma_makineleri.saldiri * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "suvariler") != NULL) {
                insan->suvariler.saldiri += (insan->suvariler.saldiri * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "tum_birimler") != NULL) {
                insan->piyadeler.saldiri += (insan->piyadeler.saldiri * etki_degeri / 100);
                insan->okcular.saldiri += (insan->okcular.saldiri * etki_degeri / 100);
                insan->kusatma_makineleri.saldiri += (insan->kusatma_makineleri.saldiri * etki_degeri / 100);
                insan->suvariler.saldiri += (insan->suvariler.saldiri * etki_degeri / 100);
            }
        } else if (strcmp(kahramanlar[i]->bonusTuru, "savunma") == 0) {
            if (strstr(kahramanlar[i]->aciklama, "piyadeler") != NULL) {
                insan->piyadeler.savunma += (insan->piyadeler.savunma * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "okcular") != NULL) {
                insan->okcular.savunma += (insan->okcular.savunma * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "suvariler") != NULL) {
                insan->suvariler.savunma += (insan->suvariler.savunma * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "kusatma_makineleri") != NULL) {
                insan->kusatma_makineleri.savunma += (insan->kusatma_makineleri.savunma * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "tum_birimler") != NULL) {
                insan->piyadeler.savunma += (insan->piyadeler.savunma * etki_degeri / 100);
                insan->okcular.savunma += (insan->okcular.savunma * etki_degeri / 100);
                insan->kusatma_makineleri.savunma += (insan->kusatma_makineleri.savunma * etki_degeri / 100);
                insan->suvariler.savunma += (insan->suvariler.savunma * etki_degeri / 100);
            }
        } else if (strcmp(kahramanlar[i]->bonusTuru, "saglik") == 0) {
            if (strstr(kahramanlar[i]->aciklama, "piyadeler") != NULL) {
                insan->piyadeler.saglik += (insan->piyadeler.saglik * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "okcular") != NULL) {
                insan->okcular.saglik += (insan->okcular.saglik * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "suvariler") != NULL) {
                insan->suvariler.saglik += (insan->suvariler.saglik * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "kusatma_makineleri") != NULL) {
                insan->kusatma_makineleri.saglik += (insan->kusatma_makineleri.saglik * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "tum_birimler") != NULL) {
                insan->piyadeler.saglik += (insan->piyadeler.saglik * etki_degeri / 100);
                insan->okcular.saglik += (insan->okcular.saglik * etki_degeri / 100);
                insan->kusatma_makineleri.saglik += (insan->kusatma_makineleri.saglik * etki_degeri / 100);
                insan->suvariler.saglik += (insan->suvariler.saglik * etki_degeri / 100);
            }
        } else if (strcmp(kahramanlar[i]->bonusTuru, "kritik_sans") == 0) {
            if (strstr(kahramanlar[i]->aciklama, "piyadeler") != NULL) {
                insan->piyadeler.kritik_sans += (insan->piyadeler.kritik_sans * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "okcular") != NULL) {
                insan->okcular.kritik_sans += (insan->okcular.kritik_sans * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "suvariler") != NULL) {
                insan->suvariler.kritik_sans += (insan->suvariler.kritik_sans * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "tum_birimler") != NULL) {
                insan->piyadeler.kritik_sans += (insan->piyadeler.kritik_sans * etki_degeri / 100);
                insan->okcular.kritik_sans += (insan->okcular.kritik_sans * etki_degeri / 100);
                insan->kusatma_makineleri.kritik_sans += (insan->kusatma_makineleri.kritik_sans * etki_degeri / 100);
                insan->suvariler.kritik_sans += (insan->suvariler.kritik_sans * etki_degeri / 100);
            }
        }
    }
}

void kahraman_etkilerini_uygula_ork(OrkLegi* ork, KahramanlarOrk* kOrk) {
    
    KahramanlarBirim* kahramanlar[] = {
        &kOrk->Goruk_Vahsi,
        &kOrk->Thruk_Kemikkiran,
        &kOrk->Vrog_Kafakiran,
        &kOrk->Ugar_Zalim
    };

    for (int i = 0; i < 4; i++) {
    
        if (kahramanlar[i]->adet != 1) {
            continue;
        }

        int etki_degeri = atoi(kahramanlar[i]->bonusDegeri);

        if (strcmp(kahramanlar[i]->bonusTuru, "saldiri") == 0) {
    
            if (strstr(kahramanlar[i]->aciklama, "ork_dovusculerine") != NULL) {
                ork->ork_dovusculeri.saldiri += (ork->ork_dovusculeri.saldiri * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "mizrakcilar") != NULL) {
                ork->mizrakcilar.saldiri += (ork->mizrakcilar.saldiri * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "varg_binicileri") != NULL) {
                ork->varg_binicileri.saldiri += (ork->varg_binicileri.saldiri * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "troller") != NULL) {
                ork->troller.saldiri += (ork->troller.saldiri * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "tum_birimler") != NULL) {
                ork->ork_dovusculeri.saldiri += (ork->ork_dovusculeri.saldiri * etki_degeri / 100);
                ork->mizrakcilar.saldiri += (ork->mizrakcilar.saldiri * etki_degeri / 100);
                ork->varg_binicileri.saldiri += (ork->varg_binicileri.saldiri * etki_degeri / 100);
                ork->troller.saldiri += (ork->troller.saldiri * etki_degeri / 100);
            }
        } else if (strcmp(kahramanlar[i]->bonusTuru, "savunma") == 0) {
            if (strstr(kahramanlar[i]->aciklama, "ork_dovusculerine") != NULL) {
                ork->ork_dovusculeri.savunma += (ork->ork_dovusculeri.savunma * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "mizrakcilar") != NULL) {
                ork->mizrakcilar.savunma += (ork->mizrakcilar.savunma * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "varg_binicileri") != NULL) {
                ork->varg_binicileri.savunma += (ork->varg_binicileri.savunma * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "troller") != NULL) {
                ork->troller.savunma += (ork->troller.savunma * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "tum_birimler") != NULL) {
                ork->ork_dovusculeri.savunma += (ork->ork_dovusculeri.savunma * etki_degeri / 100);
                ork->mizrakcilar.savunma += (ork->mizrakcilar.savunma * etki_degeri / 100);
                ork->varg_binicileri.savunma += (ork->varg_binicileri.savunma * etki_degeri / 100);
                ork->troller.savunma += (ork->troller.savunma * etki_degeri / 100);
            }
        } else if (strcmp(kahramanlar[i]->bonusTuru, "saglik") == 0) {
            if (strstr(kahramanlar[i]->aciklama, "ork_dovusculerine") != NULL) {
                ork->ork_dovusculeri.saglik += (ork->ork_dovusculeri.saglik * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "mizrakcilar") != NULL) {
                ork->mizrakcilar.saglik += (ork->mizrakcilar.saglik * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "varg_binicileri") != NULL) {
                ork->varg_binicileri.saglik += (ork->varg_binicileri.saglik * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "troller") != NULL) {
                ork->troller.saglik += (ork->troller.saglik * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "tum_birimler") != NULL) {
                ork->ork_dovusculeri.saglik += (ork->ork_dovusculeri.saglik * etki_degeri / 100);
                ork->mizrakcilar.saglik += (ork->mizrakcilar.saglik * etki_degeri / 100);
                ork->varg_binicileri.saglik += (ork->varg_binicileri.saglik * etki_degeri / 100);
                ork->troller.saglik += (ork->troller.saglik * etki_degeri / 100);
            }
        } else if (strcmp(kahramanlar[i]->bonusTuru, "kritik_sans") == 0) {
            if (strstr(kahramanlar[i]->aciklama, "ork_dovusculerine") != NULL) {
                ork->ork_dovusculeri.kritik_sans += (ork->ork_dovusculeri.kritik_sans * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "mizrakcilar") != NULL) {
                ork->mizrakcilar.kritik_sans += (ork->mizrakcilar.kritik_sans * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "varg_binicileri") != NULL) {
                ork->varg_binicileri.kritik_sans += (ork->varg_binicileri.kritik_sans * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "troller") != NULL) {
                ork->troller.kritik_sans += (ork->troller.kritik_sans * etki_degeri / 100);
            }
            if (strstr(kahramanlar[i]->aciklama, "tum_birimler") != NULL) {
                ork->ork_dovusculeri.kritik_sans += (ork->ork_dovusculeri.kritik_sans * etki_degeri / 100);
                ork->mizrakcilar.kritik_sans += (ork->mizrakcilar.kritik_sans * etki_degeri / 100);
                ork->varg_binicileri.kritik_sans += (ork->varg_binicileri.kritik_sans * etki_degeri / 100);
                ork->troller.kritik_sans += (ork->troller.kritik_sans * etki_degeri / 100);
            }
        }
    }
}

void yaratik_etkilerini_uygula_ork(OrkLegi* ork, YaratiklarOrk* yOrk) {

    YaratiklarBirim* yaratiklar[] = {
        &yOrk->Kara_Troll,
        &yOrk->Golge_Kurtlari,
        &yOrk->Camur_Devleri,
        &yOrk->Ates_Iblisi,
        &yOrk->Makrog_Savas_Beyi,
        &yOrk->Buz_Devleri
    };

    for (int i = 0; i < 6; i++) {

            if (yaratiklar[i]->adet != 1) {
            continue; 
        }


        int etki_degeri = atoi(yaratiklar[i]->etkiDegeri);

        if (strcmp(yaratiklar[i]->etkiTuru, "saldiri") == 0) {
            if (strstr(yaratiklar[i]->aciklama, "ork_dovusculeri") != NULL) {
                ork->ork_dovusculeri.saldiri += (ork->ork_dovusculeri.saldiri * etki_degeri / 100);
            }
            if (strstr(yaratiklar[i]->aciklama, "varg_binicileri") != NULL) {
                ork->varg_binicileri.saldiri += (ork->varg_binicileri.saldiri * etki_degeri / 100);
            }
            if (strstr(yaratiklar[i]->aciklama, "troller") != NULL) {
                ork->troller.saldiri += (ork->troller.saldiri * etki_degeri / 100);
            }
            if (strstr(yaratiklar[i]->aciklama, "mizrakcilar") != NULL) {
                ork->mizrakcilar.saldiri += (ork->mizrakcilar.saldiri * etki_degeri / 100);
            }
            if (strstr(yaratiklar[i]->aciklama, "tum_birimler") != NULL) {
                ork->ork_dovusculeri.saldiri += (ork->ork_dovusculeri.saldiri * etki_degeri / 100);
                ork->varg_binicileri.saldiri += (ork->varg_binicileri.saldiri * etki_degeri / 100);
                ork->troller.saldiri += (ork->troller.saldiri * etki_degeri / 100);
                ork->mizrakcilar.saldiri += (ork->mizrakcilar.saldiri * etki_degeri / 100);
            }
        } else if (strcmp(yaratiklar[i]->etkiTuru, "savunma") == 0) {
            if (strstr(yaratiklar[i]->aciklama, "ork_dovusculeri") != NULL) {
                ork->ork_dovusculeri.savunma += (ork->ork_dovusculeri.savunma * etki_degeri / 100);
            }
            if (strstr(yaratiklar[i]->aciklama, "varg_binicileri") != NULL) {
                ork->varg_binicileri.savunma += (ork->varg_binicileri.savunma * etki_degeri / 100);
            }
            if (strstr(yaratiklar[i]->aciklama, "mizrakcilar") != NULL) {
                ork->mizrakcilar.savunma += (ork->mizrakcilar.savunma * etki_degeri / 100);
            }
            if (strstr(yaratiklar[i]->aciklama, "troller") != NULL) {
                ork->troller.savunma += (ork->troller.savunma * etki_degeri / 100);
            }
            if (strstr(yaratiklar[i]->aciklama, "tum_birimler") != NULL) {
                ork->ork_dovusculeri.savunma += (ork->ork_dovusculeri.savunma * etki_degeri / 100);
                ork->varg_binicileri.savunma += (ork->varg_binicileri.savunma * etki_degeri / 100);
                ork->troller.savunma += (ork->troller.savunma * etki_degeri / 100);
                ork->mizrakcilar.savunma += (ork->mizrakcilar.savunma * etki_degeri / 100);
            }
        } else if (strcmp(yaratiklar[i]->etkiTuru, "saglik") == 0) {
            if (strstr(yaratiklar[i]->aciklama, "ork_dovusculeri") != NULL) {
                ork->ork_dovusculeri.saglik += (ork->ork_dovusculeri.saglik * etki_degeri / 100);
            }
            if (strstr(yaratiklar[i]->aciklama, "varg_binicileri") != NULL) {
                ork->varg_binicileri.saglik += (ork->varg_binicileri.saglik * etki_degeri / 100);
            }
            if (strstr(yaratiklar[i]->aciklama, "mizrakcilar") != NULL) {
                ork->mizrakcilar.saglik += (ork->mizrakcilar.saglik * etki_degeri / 100);
            }
            if (strstr(yaratiklar[i]->aciklama, "troller") != NULL) {
                ork->troller.saglik += (ork->troller.saglik * etki_degeri / 100);
            }
            if (strstr(yaratiklar[i]->aciklama, "tum_birimler") != NULL) {
                ork->ork_dovusculeri.saglik += (ork->ork_dovusculeri.saglik * etki_degeri / 100);
                ork->varg_binicileri.saglik += (ork->varg_binicileri.saglik * etki_degeri / 100);
                ork->troller.saglik += (ork->troller.saglik * etki_degeri / 100);
                ork->mizrakcilar.saglik += (ork->mizrakcilar.saglik * etki_degeri / 100);
            }
        } else if (strcmp(yaratiklar[i]->etkiTuru, "kritik_sans") == 0) {
            if (strstr(yaratiklar[i]->aciklama, "ork_dovusculeri") != NULL) {
            }
            if (strstr(yaratiklar[i]->aciklama, "varg_binicileri") != NULL) {
                ork->varg_binicileri.kritik_sans += etki_degeri; 
            }
            if (strstr(yaratiklar[i]->aciklama, "mizrakcilar") != NULL) {
                ork->mizrakcilar.kritik_sans += etki_degeri; 
            }
            if (strstr(yaratiklar[i]->aciklama, "troller") != NULL) {
                ork->troller.kritik_sans += etki_degeri; 
            }
            if (strstr(yaratiklar[i]->aciklama, "tum_birimler") != NULL) {
                ork->varg_binicileri.kritik_sans += etki_degeri; 
                ork->troller.kritik_sans += etki_degeri; 
                ork->ork_dovusculeri.kritik_sans += etki_degeri; 
                ork->mizrakcilar.kritik_sans += etki_degeri; 
            }
        }
    }
}

void yaratik_etkilerini_uygula_insan(InsanImparatorlugu* insan, YaratiklarInsan* yInsan) {
    YaratiklarBirim* yaratik[] = {
        &yInsan->Ejderha,
        &yInsan->Agri_Dagi_Devleri,
        &yInsan->Tepegoz,
        &yInsan->Karakurt,
        &yInsan->Samur
    };
    for (int i = 0; i < 5 ; i++) {
               if (yaratik[i]->adet != 1) {
            continue; 
        }

        int etki_degeri = atoi(yaratik[i]->etkiDegeri);

        if (strcmp(yaratik[i]->etkiTuru, "saldiri") == 0) {
            if (strstr(yaratik[i]->aciklama, "piyadeler") != NULL) {
                insan->piyadeler.saldiri += (insan->piyadeler.saldiri * etki_degeri / 100);
            }
            if (strstr(yaratik[i]->aciklama, "okcular") != NULL) {
                insan->okcular.saldiri += (insan->okcular.saldiri * etki_degeri / 100);
            }
            if (strstr(yaratik[i]->aciklama, "kusatma_makineleri") != NULL) {
                insan->kusatma_makineleri.saldiri += (insan->kusatma_makineleri.saldiri * etki_degeri / 100);
            }
            if (strstr(yaratik[i]->aciklama, "suvariler") != NULL) {
                insan->suvariler.saldiri += (insan->suvariler.saldiri * etki_degeri / 100);
            }
            if (strstr(yaratik[i]->aciklama, "tum_birimler") != NULL) {
                insan->piyadeler.saldiri += (insan->piyadeler.saldiri * etki_degeri / 100);
                insan->okcular.saldiri += (insan->okcular.saldiri * etki_degeri / 100);
                insan->kusatma_makineleri.saldiri += (insan->kusatma_makineleri.saldiri * etki_degeri / 100);
                insan->suvariler.saldiri += (insan->suvariler.saldiri * etki_degeri / 100);
            }
        }
        else if (strcmp(yaratik[i]->etkiTuru, "savunma") == 0) {
            if (strstr(yaratik[i]->aciklama, "piyadeler") != NULL) {
                insan->piyadeler.savunma += (insan->piyadeler.savunma * etki_degeri / 100);
            }
            if (strstr(yaratik[i]->aciklama, "okcular") != NULL) {
                insan->okcular.savunma += (insan->okcular.savunma * etki_degeri / 100);
            }
            if (strstr(yaratik[i]->aciklama, "suvariler") != NULL) {
                insan->suvariler.savunma += (insan->suvariler.savunma * etki_degeri / 100);
            }
            if (strstr(yaratik[i]->aciklama, "kusatma_makineleri") != NULL) {
                insan->kusatma_makineleri.savunma += (insan->kusatma_makineleri.savunma * etki_degeri / 100);
            }
            if (strstr(yaratik[i]->aciklama, "tum_birimler") != NULL) {
                insan->piyadeler.savunma += (insan->piyadeler.savunma * etki_degeri / 100);
                insan->okcular.savunma += (insan->okcular.savunma * etki_degeri / 100);
                insan->kusatma_makineleri.savunma += (insan->kusatma_makineleri.savunma * etki_degeri / 100);
                insan->suvariler.savunma += (insan->suvariler.savunma * etki_degeri / 100);
            }
        }
        else if (strcmp(yaratik[i]->etkiTuru, "saglik") == 0) {
            if (strstr(yaratik[i]->aciklama, "piyadeler") != NULL) {
                insan->piyadeler.saglik += (insan->piyadeler.saglik * etki_degeri / 100);
            }
            if (strstr(yaratik[i]->aciklama, "okcular") != NULL) {
                insan->okcular.saglik += (insan->okcular.saglik * etki_degeri / 100);
            }
            if (strstr(yaratik[i]->aciklama, "suvariler") != NULL) {
                insan->suvariler.saglik += (insan->suvariler.saglik * etki_degeri / 100);
            }
            if (strstr(yaratik[i]->aciklama, "kusatma_makineleri") != NULL) {
                insan->kusatma_makineleri.saglik += (insan->kusatma_makineleri.saglik * etki_degeri / 100);
            }
            if (strstr(yaratik[i]->aciklama, "tum_birimler") != NULL) {
                insan->piyadeler.saglik += (insan->piyadeler.saglik * etki_degeri / 100);
                insan->okcular.saglik += (insan->okcular.saglik * etki_degeri / 100);
                insan->kusatma_makineleri.saglik += (insan->kusatma_makineleri.saglik * etki_degeri / 100);
                insan->suvariler.saglik += (insan->suvariler.saglik * etki_degeri / 100);
            }
        }
        else if (strcmp(yaratik[i]->etkiTuru, "kritik_sans") == 0) {
            if (strstr(yaratik[i]->aciklama, "piyadeler") != NULL) {
                insan->piyadeler.kritik_sans += (insan->piyadeler.kritik_sans * etki_degeri / 100);
            }
            if (strstr(yaratik[i]->aciklama, "okcular") != NULL) {
                insan->okcular.kritik_sans += (insan->okcular.kritik_sans * etki_degeri / 100);
            }
            if (strstr(yaratik[i]->aciklama, "suvariler") != NULL) {
                insan->suvariler.kritik_sans += (insan->suvariler.kritik_sans * etki_degeri / 100);
            }
            if (strstr(yaratik[i]->aciklama, "kusatma_makineleri") != NULL) {
                insan->kusatma_makineleri.kritik_sans += (insan->kusatma_makineleri.kritik_sans * etki_degeri / 100);
            }
            if (strstr(yaratik[i]->aciklama, "tum_birimler") != NULL) {
                insan->piyadeler.kritik_sans += (insan->piyadeler.kritik_sans * etki_degeri / 100);
                insan->okcular.kritik_sans += (insan->okcular.kritik_sans * etki_degeri / 100);
                insan->kusatma_makineleri.kritik_sans += (insan->kusatma_makineleri.kritik_sans * etki_degeri / 100);
                insan->suvariler.kritik_sans += (insan->suvariler.kritik_sans * etki_degeri / 100);
            }
        }

    }
}

void yeniden_hasar_dagitim_ork(OrkLegi* ork, int kalanHasar) {
    float toplamSavunma =
        ork->mizrakcilar.savunma * ork->mizrakcilar.adet +
        ork->ork_dovusculeri.savunma * ork->ork_dovusculeri.adet +
        ork->troller.savunma * ork->troller.adet +
        ork->varg_binicileri.savunma * ork->varg_binicileri.adet;

    if (toplamSavunma == 0) return;  

    ork->mizrakcilar.oran =
        (ork->mizrakcilar.savunma * ork->mizrakcilar.adet) / toplamSavunma;

    ork->ork_dovusculeri.oran =
        (ork->ork_dovusculeri.savunma * ork->ork_dovusculeri.adet) / toplamSavunma;

    ork->troller.oran =
        (ork->troller.savunma * ork->troller.adet) / toplamSavunma;

    ork->varg_binicileri.oran =
        (ork->varg_binicileri.savunma * ork->varg_binicileri.adet) / toplamSavunma;

    ork->mizrakcilar.adet -=
        (kalanHasar * ork->mizrakcilar.oran) / ork->mizrakcilar.saglik;

    ork->ork_dovusculeri.adet -=
        (kalanHasar * ork->ork_dovusculeri.oran) / ork->ork_dovusculeri.saglik;

    ork->troller.adet -=
        (kalanHasar * ork->troller.oran) / ork->troller.saglik;

    ork->varg_binicileri.adet -=
        (kalanHasar * ork->varg_binicileri.oran) / ork->varg_binicileri.saglik;
}


void savasbaslasin(InsanImparatorlugu* insan, OrkLegi* ork) {
    
    FILE *dosya = fopen("savasadimlari.txt", "w");
    if (dosya == NULL) {
        printf("Dosya açýlamadý!\n");
        return;
    }

    float orijinalSaldiriInsanSuvariler = insan->suvariler.saldiri;
    float orijinalSaldiriInsanPiyadeler = insan->piyadeler.saldiri;
    float orijinalSaldiriInsanOkcular = insan->okcular.saldiri;
    float orijinalSaldiriInsanKusatmaMak = insan->kusatma_makineleri.saldiri;

    float orijinalSaldiriOrkMizrakcilar = ork->mizrakcilar.saldiri;
    float orijinalSaldiriOrkDovusculeri = ork->ork_dovusculeri.saldiri;
    float orijinalSaldiriOrkTroller = ork->troller.saldiri;
    float orijinalSaldiriOrkVargBinicileri = ork->varg_binicileri.saldiri;

    for (int i = 0; i >= 0; i++) {
        
        if (i % 2 == 0) {
            
            if (i > 2) {
                if ((i / 2) % insan->suvariler.kritik_sans == 0) insan->suvariler.saldiri *= 1.5;
                if ((i / 2) % insan->piyadeler.kritik_sans == 0) insan->piyadeler.saldiri *= 1.5;
                if ((i / 2) % insan->okcular.kritik_sans == 0) insan->okcular.saldiri *= 1.5;
            }

            int insantoplamhasar =
                insan->kusatma_makineleri.saldiri * insan->kusatma_makineleri.adet +
                insan->piyadeler.saldiri * insan->piyadeler.adet +
                insan->suvariler.saldiri * insan->suvariler.adet +
                insan->okcular.saldiri * insan->okcular.adet;

            int orktoplamsavunma =
                ork->mizrakcilar.savunma * ork->mizrakcilar.adet +
                ork->ork_dovusculeri.savunma * ork->ork_dovusculeri.adet +
                ork->troller.savunma * ork->troller.adet +
                ork->varg_binicileri.savunma * ork->varg_binicileri.adet;

            int insanNetHasar = insantoplamhasar * (1 - ((float)orktoplamsavunma / insantoplamhasar));
            if (insanNetHasar < 0) insanNetHasar = 0;

            
            float toplamSavunma =
                ork->mizrakcilar.savunma * ork->mizrakcilar.adet +
                ork->ork_dovusculeri.savunma * ork->ork_dovusculeri.adet +
                ork->troller.savunma * ork->troller.adet +
                ork->varg_binicileri.savunma * ork->varg_binicileri.adet;

            ork->mizrakcilar.adet -= insanNetHasar *
                                      (ork->mizrakcilar.savunma * ork->mizrakcilar.adet / toplamSavunma) /
                                      ork->mizrakcilar.saglik;

            ork->ork_dovusculeri.adet -= insanNetHasar *
                                         (ork->ork_dovusculeri.savunma * ork->ork_dovusculeri.adet / toplamSavunma) /
                                         ork->ork_dovusculeri.saglik;

            ork->troller.adet -= insanNetHasar *
                                 (ork->troller.savunma * ork->troller.adet / toplamSavunma) /
                                 ork->troller.saglik;

            ork->varg_binicileri.adet -= insanNetHasar *
                                         (ork->varg_binicileri.savunma * ork->varg_binicileri.adet / toplamSavunma) /
                                         ork->varg_binicileri.saglik;

            int fazlaHasar;

            do {
                fazlaHasar = 0;

                if (ork->mizrakcilar.adet < 0) {
                    fazlaHasar += -ork->mizrakcilar.adet * ork->mizrakcilar.saglik;
                    ork->mizrakcilar.adet = 0;
                }
                if (ork->ork_dovusculeri.adet < 0) {
                    fazlaHasar += -ork->ork_dovusculeri.adet * ork->ork_dovusculeri.saglik;
                    ork->ork_dovusculeri.adet = 0;
                }
                if (ork->troller.adet < 0) {
                    fazlaHasar += -ork->troller.adet * ork->troller.saglik;
                    ork->troller.adet = 0;
                }
                if (ork->varg_binicileri.adet < 0) {
                    fazlaHasar += -ork->varg_binicileri.adet * ork->varg_binicileri.saglik;
                    ork->varg_binicileri.adet = 0;
                }

                if (fazlaHasar > 0) {
                    yeniden_hasar_dagitim_ork(ork, fazlaHasar);
                }

            } while (fazlaHasar > 0);  

            fprintf(dosya, "%d. adim\n", i);
            fprintf(dosya, " Ork Birlikleri:\n");
            fprintf(dosya, " - Mizrakcilar Adet: %d\n", ork->mizrakcilar.adet);
            fprintf(dosya, " - Ork Dövüþçüleri Adet: %d\n", ork->ork_dovusculeri.adet);
            fprintf(dosya, " - Varg Binicileri Adet: %d\n", ork->varg_binicileri.adet);
            fprintf(dosya, " - Troller Adet: %d\n\n", ork->troller.adet);

            if (ork->mizrakcilar.adet <= 0 && ork->ork_dovusculeri.adet <= 0 &&
                ork->varg_binicileri.adet <= 0 && ork->troller.adet <= 0) {
                fprintf(dosya, "Ork Ligi yenildi!\n");
                break;
            }

        } else { 
            if (i > 2) {
                if (i / 2 % ork->mizrakcilar.kritik_sans == 0) {
                    ork->mizrakcilar.saldiri *= 1.5;
                }
                if (i / 2 % ork->ork_dovusculeri.kritik_sans == 0) {
                    ork->ork_dovusculeri.saldiri *= 1.5;
                }
                if (i / 2 % ork->troller.kritik_sans == 0) {
                    ork->troller.saldiri *= 1.5;
                }
                if (i / 2 % ork->varg_binicileri.kritik_sans == 0) {
                    ork->varg_binicileri.saldiri *= 1.5;
                }
            }

            int orktoplamhasar = ork->mizrakcilar.saldiri * ork->mizrakcilar.adet +
                                 ork->ork_dovusculeri.saldiri * ork->ork_dovusculeri.adet +
                                 ork->troller.saldiri * ork->troller.adet +
                                 ork->varg_binicileri.saldiri * ork->varg_binicileri.adet;


            int insantoplamsavunma = insan->kusatma_makineleri.savunma * insan->kusatma_makineleri.adet +
                                      insan->piyadeler.savunma * insan->piyadeler.adet +
                                      insan->suvariler.savunma * insan->suvariler.adet +
                                      insan->okcular.savunma * insan->okcular.adet;

            int orkNetHasar = orktoplamhasar * (1 - ((float)insantoplamsavunma / orktoplamhasar));
            if (orkNetHasar < 0) orkNetHasar = 0;

            float toplamSavunma =
                insan->kusatma_makineleri.savunma * insan->kusatma_makineleri.adet +
                insan->piyadeler.savunma * insan->piyadeler.adet +
                insan->suvariler.savunma * insan->suvariler.adet +
                insan->okcular.savunma * insan->okcular.adet;

            insan->kusatma_makineleri.adet -= orkNetHasar *
                                               (insan->kusatma_makineleri.savunma * insan->kusatma_makineleri.adet / toplamSavunma) /
                                               insan->kusatma_makineleri.saglik;

            insan->piyadeler.adet -= orkNetHasar *
                                      (insan->piyadeler.savunma * insan->piyadeler.adet / toplamSavunma) /
                                      insan->piyadeler.saglik;

            insan->suvariler.adet -= orkNetHasar *
                                     (insan->suvariler.savunma * insan->suvariler.adet / toplamSavunma) /
                                     insan->suvariler.saglik;

            insan->okcular.adet -= orkNetHasar *
                                   (insan->okcular.savunma * insan->okcular.adet / toplamSavunma) /
                                   insan->okcular.saglik;

            int fazlaHasar;

            do {
                fazlaHasar = 0;

                if (insan->kusatma_makineleri.adet < 0) {
                    fazlaHasar += -insan->kusatma_makineleri.adet * insan->kusatma_makineleri.saglik;
                    insan->kusatma_makineleri.adet = 0;
                }
                if (insan->piyadeler.adet < 0) {
                    fazlaHasar += -insan->piyadeler.adet * insan->piyadeler.saglik;
                    insan->piyadeler.adet = 0;
                }
                if (insan->suvariler.adet < 0) {
                    fazlaHasar += -insan->suvariler.adet * insan->suvariler.saglik;
                    insan->suvariler.adet = 0;
                }
                if (insan->okcular.adet < 0) {
                    fazlaHasar += -insan->okcular.adet * insan->okcular.saglik;
                    insan->okcular.adet = 0;
                }

                if (fazlaHasar > 0) {
                    yeniden_hasar_dagitim_insan(insan, fazlaHasar);
                }

            } while (fazlaHasar > 0);  

        
            fprintf(dosya, "%d. adim\n", i);
            fprintf(dosya, " Insan Birlikleri:\n");
            fprintf(dosya, " - Piyadeler Adet: %d\n", insan->piyadeler.adet);
            fprintf(dosya, " - Okcular Adet: %d\n", insan->okcular.adet);
            fprintf(dosya, " - Suvariler Adet: %d\n", insan->suvariler.adet);
            fprintf(dosya, " - Kusatma Makineleri Adet: %d\n\n", insan->kusatma_makineleri.adet);

        
            if (insan->piyadeler.adet <= 0 && insan->okcular.adet <= 0 &&
                insan->suvariler.adet <= 0 && insan->kusatma_makineleri.adet <= 0) {
                fprintf(dosya, "Insan Ýmparatorluðu yenildi!\n");
                break;
            }
        }
    }

    
    fclose(dosya);
}


void print_veriler(
    const InsanImparatorlugu* insan, const OrkLegi* ork,
    const KahramanlarInsan* kInsan, const KahramanlarOrk* kOrk,
    const YaratiklarInsan* yInsan, const YaratiklarOrk* yOrk) {

    printf("** Insan Imparatorlugu Birimleri **\n");
    printf("Piyadeler: %d\n", insan->piyadeler.adet);
    printf("Okcular: %d\n", insan->okcular.adet);
    printf("Suvariler: %d\n", insan->suvariler.adet);
    printf("Kusatma Makineleri: %d\n", insan->kusatma_makineleri.adet);

    printf("\n** Ork Lejyonu Birimleri **\n");
    printf("Ork Dovusculeri: %d\n", ork->ork_dovusculeri.adet);
    printf("Mizrakcilar: %d\n", ork->mizrakcilar.adet);
    printf("Varg Binicileri: %d\n", ork->varg_binicileri.adet);
    printf("Troller: %d\n", ork->troller.adet);

    printf("\n** Kahramanlar (Insan) **\n");
    printf("Alparslan: %d\n", kInsan->Alparslan.adet);
    printf("Fatih Sultan Mehmet: %d\n", kInsan->Fatih_Sultan_Mehmet.adet);
    printf("Mete Han: %d\n", kInsan->Mete_Han.adet);
    printf("Yavuz Sultan Selim: %d\n", kInsan->Yavuz_Sultan_Selim.adet);
    printf("Tugrul Bey: %d\n", kInsan->Tugrul_Bey.adet);

    printf("\n** Kahramanlar (Ork) **\n");
    printf("Goruk Vahsi: %d\n", kOrk->Goruk_Vahsi.adet);
    printf("Thruk Kemikkiran: %d\n", kOrk->Thruk_Kemikkiran.adet);
    printf("Vrog Kafakiran: %d\n", kOrk->Vrog_Kafakiran.adet);
    printf("Ugar Zalim: %d\n", kOrk->Ugar_Zalim.adet);

    printf("\n** Yaratiklar (Insan) **\n");
    printf("Ejderha: %d\n", yInsan->Ejderha.adet);
    printf("Agri Dagi Devleri: %d\n", yInsan->Agri_Dagi_Devleri.adet);
    printf("Tepegoz: %d\n", yInsan->Tepegoz.adet);
    printf("Karakurt: %d\n", yInsan->Karakurt.adet);
    printf("Samur: %d\n", yInsan->Samur.adet);

    printf("\n** Yaratiklar (Ork) **\n");
    printf("Kara Troll: %d\n", yOrk->Kara_Troll.adet);
    printf("Golge Kurtlari: %d\n", yOrk->Golge_Kurtlari.adet);
    printf("Camur Devleri: %d\n", yOrk->Camur_Devleri.adet);
    printf("Ates Iblisi: %d\n", yOrk->Ates_Iblisi.adet);
    printf("Makrog Savas Beyi: %d\n", yOrk->Makrog_Savas_Beyi.adet);
    printf("Buz Devleri: %d\n", yOrk->Buz_Devleri.adet);
}

void yazdir(InsanImparatorlugu* insan, OrkLegi* ork) {
    printf("Insan Imparatorlugu Birimleri:\n");
    printf("Okcular: Saldiri: %d, Savunma: %d, Saglik: %d, Kritik Sans: %d, Adet: %d\n",
           insan->okcular.saldiri, insan->okcular.savunma, insan->okcular.saglik, insan->okcular.kritik_sans, insan->okcular.adet);
    printf("Kusatma Makineleri: Saldiri: %d, Savunma: %d, Saglik: %d, Kritik Sans: %d, Adet: %d\n",
           insan->kusatma_makineleri.saldiri, insan->kusatma_makineleri.savunma, insan->kusatma_makineleri.saglik, insan->kusatma_makineleri.kritik_sans, insan->kusatma_makineleri.adet);
    printf("Piyadeler: Saldiri: %d, Savunma: %d, Saglik: %d, Kritik Sans: %d, Adet: %d\n",
           insan->piyadeler.saldiri, insan->piyadeler.savunma, insan->piyadeler.saglik, insan->piyadeler.kritik_sans, insan->piyadeler.adet);
    printf("Suvariler: Saldiri: %d, Savunma: %d, Saglik: %d, Kritik Sans: %d, Adet: %d\n\n",
           insan->suvariler.saldiri, insan->suvariler.savunma, insan->suvariler.saglik, insan->suvariler.kritik_sans, insan->suvariler.adet);

    printf("Ork Lejyonu Birimleri:\n");
    printf("Ork Dovusculeri: Saldiri: %d, Savunma: %d, Saglik: %d, Kritik Sans: %d, Adet: %d\n",
           ork->ork_dovusculeri.saldiri, ork->ork_dovusculeri.savunma, ork->ork_dovusculeri.saglik, ork->ork_dovusculeri.kritik_sans, ork->ork_dovusculeri.adet);
    printf("Mizrakcilar: Saldiri: %d, Savunma: %d, Saglik: %d, Kritik Sans: %d, Adet: %d\n",
           ork->mizrakcilar.saldiri, ork->mizrakcilar.savunma, ork->mizrakcilar.saglik, ork->mizrakcilar.kritik_sans, ork->mizrakcilar.adet);
    printf("Varg Binicileri: Saldiri: %d, Savunma: %d, Saglik: %d, Kritik Sans: %d, Adet: %d\n",
           ork->varg_binicileri.saldiri, ork->varg_binicileri.savunma, ork->varg_binicileri.saglik, ork->varg_binicileri.kritik_sans, ork->varg_binicileri.adet);
    printf("Troller: Saldiri: %d, Savunma: %d, Saglik: %d, Kritik Sans: %d, Adet: %d\n\n\n",
           ork->troller.saldiri, ork->troller.savunma, ork->troller.saglik, ork->troller.kritik_sans, ork->troller.adet);
}

void yazdir1(KahramanlarInsan* kInsan, KahramanlarOrk* kOrk) {
    printf("Insan Imparatorlugu Birimleri:\n");
    printf("Alparslan: bonus_turu: %s, bonus_degeri: %s, aciklama: %s\n",
           kInsan->Alparslan.bonusTuru, kInsan->Alparslan.bonusDegeri, kInsan->Alparslan.aciklama);
    printf("Fatih Sultan Mehmet: bonus_turu: %s, bonus_degeri: %s, aciklama: %s\n",
           kInsan->Fatih_Sultan_Mehmet.bonusTuru, kInsan->Fatih_Sultan_Mehmet.bonusDegeri, kInsan->Fatih_Sultan_Mehmet.aciklama);
    printf("Mete Han: bonus_turu: %s, bonus_degeri: %s, aciklama: %s\n",
           kInsan->Mete_Han.bonusTuru, kInsan->Mete_Han.bonusDegeri, kInsan->Mete_Han.aciklama);
    printf("Yavuz Sultan Selim: bonus_turu: %s, bonus_degeri: %s, aciklama: %s\n",
           kInsan->Yavuz_Sultan_Selim.bonusTuru, kInsan->Yavuz_Sultan_Selim.bonusDegeri, kInsan->Yavuz_Sultan_Selim.aciklama);
    printf("Tugrul Bey: bonus_turu: %s, bonus_degeri: %s, aciklama: %s\n\n",
           kInsan->Tugrul_Bey.bonusTuru, kInsan->Tugrul_Bey.bonusDegeri, kInsan->Tugrul_Bey.aciklama);

    printf("\nOrk Lejyonu Birimleri:\n");
    printf("Goruk Vahsi: bonus_turu: %s, bonus_degeri: %s, aciklama: %s\n",
           kOrk->Goruk_Vahsi.bonusTuru, kOrk->Goruk_Vahsi.bonusDegeri, kOrk->Goruk_Vahsi.aciklama);
    printf("Thruk Kemikkiran: bonus_turu: %s, bonus_degeri: %s, aciklama: %s\n",
           kOrk->Thruk_Kemikkiran.bonusTuru, kOrk->Thruk_Kemikkiran.bonusDegeri, kOrk->Thruk_Kemikkiran.aciklama);
    printf("Vrog Kafakiran: bonus_turu: %s, bonus_degeri: %s, aciklama: %s\n",
           kOrk->Vrog_Kafakiran.bonusTuru, kOrk->Vrog_Kafakiran.bonusDegeri, kOrk->Vrog_Kafakiran.aciklama);
    printf("Ugar Zalim: bonus_turu: %s, bonus_degeri: %s, aciklama: %s\n\n\n",
           kOrk->Ugar_Zalim.bonusTuru, kOrk->Ugar_Zalim.bonusDegeri, kOrk->Ugar_Zalim.aciklama);
}

void yazdir2(YaratiklarInsan* yInsan, YaratiklarOrk* yOrk) {
    printf("Insan Imparatorlugu Birimleri:\n");
    printf("Ejderha: etki_degeri: %s, etki_turu: %s, aciklama: %s\n",
           yInsan->Ejderha.etkiDegeri, yInsan->Ejderha.etkiTuru, yInsan->Ejderha.aciklama);
    printf("Agri Dagi Devleri: etki_degeri: %s, etki_turu: %s, aciklama: %s\n",
           yInsan->Agri_Dagi_Devleri.etkiDegeri, yInsan->Agri_Dagi_Devleri.etkiTuru, yInsan->Agri_Dagi_Devleri.aciklama);
    printf("Tepegoz: etki_degeri: %s, etki_turu: %s, aciklama: %s\n",
           yInsan->Tepegoz.etkiDegeri, yInsan->Tepegoz.etkiTuru, yInsan->Tepegoz.aciklama);
    printf("Karakurt: etki_degeri: %s, etki_turu: %s, aciklama: %s\n",
           yInsan->Karakurt.etkiDegeri, yInsan->Karakurt.etkiTuru, yInsan->Karakurt.aciklama);
    printf("Samur: etki_degeri: %s, etki_turu: %s, aciklama: %s\n\n",
           yInsan->Samur.etkiDegeri, yInsan->Samur.etkiTuru, yInsan->Samur.aciklama);

    printf("\nOrk Lejyonu Birimleri:\n");
    printf("Kara Troll: etki_degeri: %s, etki_turu: %s, aciklama: %s\n",
           yOrk->Kara_Troll.etkiDegeri, yOrk->Kara_Troll.etkiTuru, yOrk->Kara_Troll.aciklama);
    printf("Golge Kurtlari: etki_degeri: %s, etki_turu: %s, aciklama: %s\n",
           yOrk->Golge_Kurtlari.etkiDegeri, yOrk->Golge_Kurtlari.etkiTuru, yOrk->Golge_Kurtlari.aciklama);
    printf("Camur Devleri: etki_degeri: %s, etki_turu: %s, aciklama: %s\n",
           yOrk->Camur_Devleri.etkiDegeri, yOrk->Camur_Devleri.etkiTuru, yOrk->Camur_Devleri.aciklama);
    printf("Ates Iblisi: etki_degeri: %s, etki_turu: %s, aciklama: %s\n",
           yOrk->Ates_Iblisi.etkiDegeri, yOrk->Ates_Iblisi.etkiTuru, yOrk->Ates_Iblisi.aciklama);
    printf("Makrog Savas Beyi: etki_degeri: %s, etki_turu: %s, aciklama: %s\n",
           yOrk->Makrog_Savas_Beyi.etkiDegeri, yOrk->Makrog_Savas_Beyi.etkiTuru, yOrk->Makrog_Savas_Beyi.aciklama);
    printf("Buz Devleri: etki_degeri: %s, etki_turu: %s, aciklama: %s\n\n\n\n",
           yOrk->Buz_Devleri.etkiDegeri, yOrk->Buz_Devleri.etkiTuru, yOrk->Buz_Devleri.aciklama);
}




int main() {
    int secim;

   
    printf("Lutfen 1 ile 10 arasinda bir sayi girin: ");
    scanf("%d", &secim);

    
    if (secim < 1 || secim > 10) {
        printf("Gecersiz secim! Lutfen 1 ile 10 arasinda bir sayi girin.\n");
        return 1;
    }

    
    const char *selected_url = urls[secim - 1];
    printf("Secilen URL: %s\n", selected_url);

    // JSON dosyalarýný indir
    const char *temp_filename = "temp.json";
    if (download_json(selected_url, temp_filename) != 0) {
        return 1; 
    }

    InsanImparatorlugu insan;
    OrkLegi ork;
    KahramanlarInsan kInsan;
    KahramanlarOrk kOrk;
    YaratiklarInsan yInsan;
    YaratiklarOrk yOrk;
    Yetenekler yetenekler[10];
    int yetenek_sayisi = 0;

    parse_json_file("unit_types.json", &insan, &ork);
    
    parse_json_file3("research.json", yetenekler, &yetenek_sayisi);
    parse_json_file2("creatures.json", &yInsan, &yOrk);
    parse_json_file1("heroes.json", &kInsan, &kOrk);

   
    parse_json_file4("temp.json", &insan, &ork, &kInsan, &kOrk, &yInsan, &yOrk, yetenekler, yetenek_sayisi);
   

    
    kahraman_etkilerini_uygula_insan(&insan, &kInsan);
    kahraman_etkilerini_uygula_ork(&ork, &kOrk);

    yaratik_etkilerini_uygula_insan(&insan, &yInsan);
    yaratik_etkilerini_uygula_ork(&ork, &yOrk);

    yetenek_etkilerini_uygula(yetenekler, yetenek_sayisi, &insan, &ork);
     yazdir(&insan,&ork);

    
    savasbaslasin(&insan, &ork);

    return 0;
}




















