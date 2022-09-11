#include "veri.h"
#include <iostream>
#include <stack>
#include <queue>
#include <chrono>
#include <functional>

using namespace std;

void IkiliSiralamaAgaci::agacaEkle(Dugum **donulenDugum, Dugum *eklenecekDugum) {
    if (*donulenDugum == NULL){
        *donulenDugum = eklenecekDugum;
    }
        /* eklenecek dugumun verisinin mevcut dugumden buyuk yada kucuk olmasına gore
           donulen dugumun sag yada sol dugumu icin recursive cagri yapiliyor.
         */
    else if ((*donulenDugum)->veri < eklenecekDugum->veri)
        agacaEkle(&(*donulenDugum)->sag, eklenecekDugum);
    else if ((*donulenDugum)->veri > eklenecekDugum->veri)
        agacaEkle(&(*donulenDugum)->sol, eklenecekDugum);
}

void IkiliSiralamaAgaci::agacKur(int *agacKurulacakDizi) {
    Dugum *yeniDugum;
    kok = NULL;
    // verilen dizinin tum elemanlari icin yeniDugum olusturulup agacaEkle fonksiyonuna gonderiliyor
    for (int i = 0; i < UZUNLUK; i++) {
        yeniDugum = new Dugum;
        yeniDugum->veri = agacKurulacakDizi[i];
        yeniDugum->sag = NULL;
        yeniDugum->sol = NULL;
        agacaEkle(&(kok),yeniDugum);
    }
}

void IkiliSiralamaAgaci::agaciBosalt(Dugum *islemYapilacakDugum) {
    // recursive cagri ile agac icin alinan bellek gozleri iade ediliyor.
    if(islemYapilacakDugum == NULL )
        return;
    else if(islemYapilacakDugum->sol != NULL )
        agaciBosalt(islemYapilacakDugum->sol );
    else if(islemYapilacakDugum->sag != NULL )
        agaciBosalt(islemYapilacakDugum->sag );

    delete islemYapilacakDugum;
}

void IkiliSiralamaAgaci::agacKapat() {
    agaciBosalt(kok);
}

bool DFS(Dugum *aramaYapilacakAgacinKoku, int aranacakDeger){
    Dugum *p;
    stack<Dugum*> yigin;
    yigin.push(aramaYapilacakAgacinKoku);
    while (!yigin.empty()){
        p = yigin.top();
        yigin.pop();
        if (aranacakDeger == p->veri){
            cout << "SONUC => VERI BULUNDU" << endl;
            return true;
        }
        if (p->sag)
            yigin.push(p->sag);
        if (p->sol)
            yigin.push(p->sol);
    }
    cout << "SONUC => VERI BULUNAMADI" << endl;
    return false;
}

bool BFS(Dugum  *aramaYapilacakAgacinKoku, int aranacakDeger){
    Dugum *p;
    queue<Dugum*> kuyruk;
    kuyruk.push(aramaYapilacakAgacinKoku);
    while (!kuyruk.empty()){
        p = kuyruk.front();
        kuyruk.pop();
        if (aranacakDeger == p->veri){
            cout << "SONUC => VERI BULUNDU" << endl;
            return true;
        }
        if (p->sol)
            kuyruk.push(p->sol);
        if (p->sag)
            kuyruk.push(p->sag);
    }
    cout << "SONUC => VERI BULUNAMADI" << endl;
    return false;
}

void secilenYonteminSonuclanmaSuresiniOlc(Dugum * aranacakKok, int aranacakDeger, function<bool(Dugum *, int)> fonk){
    auto begin = std::chrono::high_resolution_clock::now();
    fonk(aranacakKok,aranacakDeger);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin); cout << "GECEN SURE => " << elapsed.count() << " nano-saniye" << endl;
}

void aranacakVerininGirilmesiMenusu(IkiliSiralamaAgaci secilenAgac ,int secilenAgacNo, string secilenAramaYontemi){
    bool menu = true;
    while (menu){
        cout << "Agac " << secilenAgacNo << " uzerinde " << secilenAramaYontemi << " ile arayacaginiz degeri giriniz:" << endl;
        int aranacakDeger;
        cin >> aranacakDeger;
        if (secilenAramaYontemi == "DFS") // yontemSecimMenusu'nden gelen parametre DFS ise
            secilenYonteminSonuclanmaSuresiniOlc(secilenAgac.kok, aranacakDeger, &DFS);
        else
            secilenYonteminSonuclanmaSuresiniOlc(secilenAgac.kok, aranacakDeger, &BFS);

        // ilk arama sonrasi kullanicinin karsisina cikacak secenek secimi
        int secim;
        cout << endl << "Yeni deger aramak icin 1'i, yontem secim ekranina donmek icin 2'yi seciniz." << endl;
        cin >> secim;
        switch (secim) {
            case 1:
                aranacakVerininGirilmesiMenusu(secilenAgac, secilenAgacNo, secilenAramaYontemi);
                break;
            case 2:
                menu = false;
                break;
            default:
                cout << "Lutfen 1 veya 2 seceneklerinden birini seciniz" << endl;
        }
    }
}

void yontemSecimMenusu(IkiliSiralamaAgaci secilenAgac, int secilenAgacNo){
    bool menu = true;
    while (menu){
        cout << "Agac " << secilenAgacNo << " icin arama yontemini seciniz.(DFS:1, BFS:2, AnaMenu:3):" << endl;
        int secim;
        cin >> secim;
        switch (secim) {
            case 1:
                aranacakVerininGirilmesiMenusu(secilenAgac, secilenAgacNo, "DFS");
                break;
            case 2:
                aranacakVerininGirilmesiMenusu(secilenAgac,secilenAgacNo, "BFS");
                break;
            case 3:
                menu = false;
                break;
            default:
                cout << "Lutfen parantez icinde verilen seceneklerden birini giriniz." << endl << endl;
                break;
        }
    }
}

void agacSecimMenu(IkiliSiralamaAgaci ilkAgac, IkiliSiralamaAgaci ikinciAgac, IkiliSiralamaAgaci ucuncuAgac){
    bool menu = true;
    while (menu){
        cout << "Arama yapacaginiz agaci seciniz.(1,2,3 - Cikis:4):" << endl;
        int secim;
        cin >> secim;
        switch (secim) {
            case 1:
                yontemSecimMenusu(ilkAgac, secim);
                break;
            case 2:
                yontemSecimMenusu(ikinciAgac,secim);
                break;
            case 3:
                yontemSecimMenusu(ucuncuAgac,secim);
                break;
            case 4:
                menu = false;
                break;
            default:
                cout << "Lutfen parantez icinde verilen seceneklerden birini giriniz." << endl << endl;
                break;
        }
    }
}

int main() {

    IkiliSiralamaAgaci ilkAgac;
    IkiliSiralamaAgaci ikinciAgac;
    IkiliSiralamaAgaci ucuncuAgac;

    // uc agac verilen farklı diziler ile kuruluyor
    ilkAgac.agacKur(dizi1);
    ikinciAgac.agacKur(dizi2);
    ucuncuAgac.agacKur(dizi3);

    // kullanicinin arama yapacagi agaci sectigi menu cagiriliyor
    agacSecimMenu(ilkAgac, ikinciAgac, ucuncuAgac);

    // alınan bellek gozleri iade ediliyor
    cout << "Agac bellekleri iade ediliyor. Iyi gunler." << endl;
    ilkAgac.agacKapat();
    ikinciAgac.agacKapat();
    ucuncuAgac.agacKapat();

    return 0;
}
