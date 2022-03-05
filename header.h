#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef char string[100];

typedef struct tNota* adrNota;
typedef struct tNota* ListNota;

typedef struct tMenu* adrMenu;
typedef struct tMenu* ListMenu;

typedef struct {
	string namaPesanan;
	int jumlah;
	double harga;
}Menu;
typedef struct {
	int noNota;
	string tanggal;
	int noMeja;
	double totalHarga;
	int jumlahNodeMenu;
}Nota;

typedef struct tNota {
	Nota nota;
	adrNota nextNota;
	ListMenu M;
}NodeNota;
typedef struct tMenu {
	Menu menu;
	adrMenu nextMenu;
}NodeMenu;

typedef struct tJual {
	string namaMenu;
	int jumlah;
}Jual;


//-- Pembayaran
void pembayaran(ListNota *LN, adrNota bayar, Jual makanan[5], Jual minuman[5], double *omset);


//-- Menu yang dijual
void initMakanan(Jual makanan[5]);
void initMinuman(Jual minuman[5]);
void cekMakanan(Jual M[5], string makanan, int jumlah);
void cekMinuman(Jual M[5], string minuman, int jumlah);


//--- Nota
void createEmpty(ListNota *LN);
bool isEmptyNota(ListNota LN);
bool isFoundNota(ListNota LN, int noNota);
Nota makeNota(int noNota, string tanggal, int noMeja);
adrNota alokasiNota(Nota N);
void insertFirstNota(ListNota *LN, adrNota P);
void insertLastNota(ListNota *LN, adrNota P);
void insertAfterNota(adrNota prec, adrNota P);
double hitungTotalHarga(ListMenu LM);
void printDataNota(Nota N);
void printNota(ListNota LN);
adrNota addressOf(ListNota LN, int noNota);
void delFirstNota(ListNota *LN);
void delLastNota(ListNota *LN);
void delAtNota(ListNota *LN, adrNota hapus);
void gabungNota(ListNota *LN, int *noNotaMin, Jual makanan[5], Jual minuman[5], double *omset);
void decrementNoNota(ListNota *LN, int noNota);
void incrementNoNota(ListNota *LN);
int getNoNotaTerakhir(ListNota LUNAS);
void pisahNota(ListNota *LN, int *noNotaB, Jual makanan[5], Jual minuman[5], double *omset);


//-- Menu
bool isEmptyMenu(ListMenu LM);
void tampilMenu();
int cekValidPesanan(string namaPesanan);
double getHarga(string namaPesanan);
Menu makeMenu(string namaPesanan, int jumlah, double harga);
adrMenu alokasiMenu(Menu M);
void insertFirstMenu(ListMenu *LM, adrMenu P);
void insertLastMenu(ListMenu *LM, adrMenu P);
void insertAfterMenu(adrMenu prec, adrMenu P);
void printDataMenu(Menu M);
void printMenu(ListMenu LM);
bool isOneElementMenu(ListMenu LM);
int cekAdaMenu(ListMenu LM, string namaPesanan);
adrMenu addressOfMenu(ListMenu LM, string namaPesanan);
void delFirstMenu(ListMenu *LM);
void delLastMenu(ListMenu *LM);
void delAtMenu(ListMenu *LM, adrMenu hapus);
adrMenu getAdrLastMenu(ListMenu LM);

//-- File handling
void saveFileListKasir(ListNota LNK, int noNota);
void saveFileListDapur(ListNota LNK);
void loadListKasir(ListNota *LNK, int *noNota);
void loadListDapur(ListNota *LNK);
