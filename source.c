#include "header.h"

void createEmpty(ListNota *LN) {
	(*LN) = NULL;
}
void createEmptyMenu(ListMenu *LM) {
	(*LM) = NULL;
}
bool isOneElement(ListNota LN) {
	return LN->nextNota==NULL;
}


//--- Nota
bool isEmptyNota(ListNota LN) {
	return (LN==NULL);
}
bool isFoundNota(ListNota LN, int noNota) {
	if(!isEmptyNota(LN)) {
		return (LN->nota.noNota == noNota || isFoundNota(LN->nextNota, noNota));
	}
	return false;
}
Nota makeNota(int noNota, string tanggal, int noMeja) {
	Nota N;
	N.noNota = noNota;
	strcpy(N.tanggal, tanggal);
	N.noMeja = noMeja;
	N.totalHarga = 0;
	N.jumlahNodeMenu = 0;
	return N;
}
adrNota alokasiNota(Nota N) {
	adrNota temp;
	temp = (adrNota) malloc(sizeof(NodeNota));
	if(temp == NULL)
		return NULL;
	else {
		temp->nota = N;
		temp->nextNota = NULL;
		temp->M = NULL;
		return temp;
	}
}
void insertFirstNota(ListNota *LN, adrNota P) {
	P->nextNota = (*LN);
	(*LN) = P;
}
void insertLastNota(ListNota *LN, adrNota P) {
	if(isEmptyNota(*LN))
		insertFirstNota(&(*LN), P);
	else
		insertLastNota(&(*LN)->nextNota, P);
}
void insertAfterNota(adrNota prec, adrNota P) {
	P->nextNota = prec->nextNota;
	prec->nextNota = P;
}
double hitungTotalHarga(ListMenu LM) {
	if(!isEmptyMenu(LM))
		return ((LM->menu.harga)*(double)(LM->menu.jumlah) + hitungTotalHarga(LM->nextMenu));
	else
		return 0;
}
void printDataNota(Nota N) {
	printf("\n\n\n----------------------------------");
	printf("\nNomor Nota: %d", N.noNota);
	printf("\nTanggal Nota: %s", N.tanggal);
	printf("\nNomor Meja: %d", N.noMeja);
	printf("\nTotal Harga: Rp%.2lf", N.totalHarga);
}
void printNota(ListNota LN) {
	if(!isEmptyNota(LN)) {
		printDataNota(LN->nota);
		printMenu(LN->M);
		printNota(LN->nextNota);
	}
}
adrNota addressOf(ListNota LN, int noNota) {
	if(isEmptyNota(LN))
		return NULL;
	else {
		if(LN->nota.noNota == noNota)
			return LN;
		else
			return addressOf(LN->nextNota, noNota);
	}
}
void pembayaran(ListNota *LN, adrNota bayar, Jual makanan[5], Jual minuman[5], double *omset) {
	double uang;
	adrNota baruLunas;
	adrMenu menuLunas;
	printDataNota(bayar->nota);
	printMenu(bayar->M);
	do {
		cekMakanan(makanan, bayar->M->menu.namaPesanan, bayar->M->menu.jumlah);
		cekMinuman(minuman, bayar->M->menu.namaPesanan, bayar->M->menu.jumlah);
		bayar->M = bayar->M->nextMenu;
	}while(bayar->M!=NULL);
	printf("\n");
	printf("\nTotal Harga: Rp%.2lf", bayar->nota.totalHarga);
	(*omset) += bayar->nota.totalHarga;
	do {
		printf("\nMasukan jumlah uang: ");scanf("%lf", &uang);
		if(uang < bayar->nota.totalHarga)
			printf("\n[!] UANG KURANG !");
	}while(uang < bayar->nota.totalHarga);
	printf("\nKembalian: Rp%.2lf", uang-(bayar->nota.totalHarga));
	printf("\n\tTerima Kasih ^_^");
	
	baruLunas = alokasiNota(bayar->nota);
	while(bayar->M != NULL) {
		menuLunas = alokasiMenu(bayar->M->menu);
		insertLastMenu(&baruLunas->M,menuLunas);
		bayar->M = bayar->M->nextMenu;
	}
	delAtNota(&(*LN), bayar);
}
// delete
void delFirstNota(ListNota *LN) {
	adrNota hapus;
	hapus = (*LN);
	(*LN) = (*LN)->nextNota;
	free(hapus->M);
	free(hapus);
}
void delLastNota(ListNota *LN) {
	if(isOneElement((*LN)))
		delFirstNota(&(*LN));
	else
		delLastNota(&(*LN)->nextNota);
}
void delAtNota(ListNota *LN, adrNota hapus) {
	if((*LN)==hapus)
		delFirstNota(&(*LN));
	else
		delAtNota(&(*LN)->nextNota,hapus);
}
void gabungNota(ListNota *LN, int *noNotaMin, Jual makanan[5], Jual minuman[5], double *omset) {
	int noNota, notaDec=0;
	string yaTidak;
	adrNota cari;
	adrNota notaBakalGabung;
	adrMenu menuBakalGabung;
	
	printf("\nMasukan nomor nota yang akan dibayar: ");scanf("%d", &noNota);
	cari = addressOf((*LN), noNota);
	if(cari != NULL) {
		do {
			printf("\nMasukan no nota yang ingin digabung: ");scanf("%d", &noNota);
			notaBakalGabung = addressOf((*LN), noNota);
			if(notaBakalGabung != NULL) {
				while(notaBakalGabung->M != NULL) {
					menuBakalGabung = alokasiMenu(notaBakalGabung->M->menu);
					insertLastMenu(&cari->M, menuBakalGabung);
					notaBakalGabung->M = notaBakalGabung->M->nextMenu;
				}
				decrementNoNota(&notaBakalGabung->nextNota, noNota);
				(*noNotaMin)--;
				delAtNota(&(*LN),notaBakalGabung);
				cari->nota.totalHarga = hitungTotalHarga(cari->M); //hitung ulang total harga
			}
			else {
				printf("\n[!] NOTA TIDAK DITEMUKAN !");
			}
			do {
				printf("\nMau gabung nota lagi [Ya/Tidak]? ");fflush(stdin);gets(yaTidak);
			}while(strcmpi(yaTidak,"Tidak")!=0 && strcmpi(yaTidak,"Ya")!=0);
			
		}while(strcmpi(yaTidak,"Ya")==0);
		pembayaran(&(*LN),cari, makanan, minuman, &(*omset));
	}
	else
		printf("\n[!] NOTA TIDAK DITEMUKAN !");
}
void decrementNoNota(ListNota *LN, int noNota) { 
	if(!isEmptyNota(*LN)) {
		(*LN)->nota.noNota--;
		if(isFoundNota((*LN), noNota)==true)
			(*LN)->nota.noNota--;
		decrementNoNota(&(*LN)->nextNota, noNota);
	}
}
void pisahNota(ListNota *LN, int *noNotaB, Jual makanan[5], Jual minuman[5], double *omset) {
	adrNota cari, baru;
	Nota notaBaru;
	adrMenu temp;
	int noNota, n, nomenubaru=0;
	string namaPesanan, yaTidak;
	ListMenu LM;
	adrMenu menuPisah;
	
	createEmptyMenu(&LM);
	
	printf("\nMasukan nomor nota yang akan dipisah: ");scanf("%d", &noNota);
	
	if(isFoundNota(*LN, noNota) == true) {
		cari = addressOf(*LN, noNota);
		printMenu(cari->M);
		if(isOneElementMenu(cari->M)==true && cari->M->menu.jumlah < 2) {
			printf("\n[!] TIDAK DAPAT PISAH NOTA KARENA HANYA ADA 1 PESANAN !");
			return;
		}
		else {
			do {
				createEmptyMenu(&LM);
				do {
					if(!isOneElementMenu(cari->M)==true || cari->M->menu.jumlah > 1) {
						do {
							printf("\nMasukan menu yang akan dipisah dari nota nomor %d: ", cari->nota.noNota);fflush(stdin);gets(namaPesanan);
							if(cekAdaMenu(cari->M, namaPesanan)==0)
								printf("\n[!] MENU TIDAK ADA !");
						}while(cekAdaMenu(cari->M, namaPesanan)==0);
						temp = addressOfMenu(cari->M, namaPesanan);
						if(temp->menu.jumlah > 1) {
							menuPisah = alokasiMenu(temp->menu);
							do {
								printf("\nJumlah yang di pisah: ");scanf("%d",&n);
								if(n >= temp->menu.jumlah)
									printf("\n[!] MELEBIHI JUMLAH PESANAN !");
							}while(n < 1 || n >= temp->menu.jumlah);
							menuPisah->menu.jumlah = n;
							temp->menu.jumlah -= n;
							insertFirstMenu(&LM, menuPisah);
						}
						else {
							menuPisah = alokasiMenu(temp->menu);
							insertFirstMenu(&LM, menuPisah);
							delAtMenu(&cari->M, temp);
							cari->nota.jumlahNodeMenu--;
						}
						if(temp->menu.jumlah==0) {
							delAtMenu(&cari->M, temp);
						}
					}
					else {
						printf("\n[!] TIDAK DAPAT PISAH NOTA KARENA HANYA ADA 1 PESANAN !");
						return;
					}
					do {
						printf("\nIngin menambahkan menu lagi [Ya/Tidak]? ");fflush(stdin);gets(yaTidak);
					}while(strcmpi(yaTidak,"ya")!=0 && strcmpi(yaTidak,"tidak")!=0);
					cari->nota.totalHarga = hitungTotalHarga(cari->M);
				}while(strcmpi(yaTidak,"ya")==0);
				notaBaru = makeNota(noNota+1, cari->nota.tanggal, cari->nota.noMeja);
				notaBaru.jumlahNodeMenu = nomenubaru;
				(*noNotaB)++;
				baru = alokasiNota(notaBaru);
				baru->M = LM;
				baru->nota.totalHarga = hitungTotalHarga(baru->M);
				insertAfterNota(cari, baru);
				incrementNoNota(&cari->nextNota);
				pembayaran(&(*LN), baru, makanan, minuman, &(*omset));
				do {
					printf("\nApakah mau memindahkan menu lagi ke nota yang baru [Ya/Tidak]? ");fflush(stdin);gets(yaTidak);
				}while(strcmpi(yaTidak,"ya")!=0 && strcmpi(yaTidak,"Tidak")!=0);
				nomenubaru++;
			}while(strcmpi(yaTidak, "Ya")==0);
		}
		pembayaran(&(*LN), cari, makanan, minuman, &(*omset));
	}
	else
		printf("\n[!] NO NOTA TIDAK ADA");
}
void incrementNoNota(ListNota *LN) {
	if(!isEmptyNota(*LN)) {
		(*LN)->nota.noNota++;
		incrementNoNota(&(*LN)->nextNota);
	}
}
int getNoNotaTerakhir(ListNota LUNAS) {
	if(!isEmptyNota(LUNAS)) {
		return getNoNotaTerakhir(LUNAS->nextNota);
	}
	return LUNAS->nota.noNota;
}


//-- Menu
bool isEmptyMenu(ListMenu LM) {
	return (LM==NULL);
}
void tampilMenu() {
	printf("\n===MENU MAKANAN===\n");
	printf("1. Nasi Goreng	Rp15.000\n");
	printf("2. Bakwan	Rp3000\n");
	printf("3. Nasi Ayam 	Rp15.000\n");
	printf("4. Nasi Telor	Rp10.000\n");
	printf("5. Nasi Kucing	Rp5000\n");
	
	printf("\n===MENU MINUMAN===\n");
	printf("1. Air Putih 	Rp1000\n");
	printf("2. Es Jeruk	Rp3000\n");
	printf("3. Es Milo	Rp5000\n");
	printf("4. Thai Tea	Rp10.000\n");
	printf("5. Milk Shake	Rp20.000\n");
}
int cekValidPesanan(string namaPesanan) {
	if(strcmpi(namaPesanan,"nasi goreng")!=0&&strcmpi(namaPesanan,"Bakwan")!=0
	&&strcmpi(namaPesanan,"nasi ayam")!=0&&strcmpi(namaPesanan,"nasi telor")!=0
	&&strcmpi(namaPesanan,"nasi kucing")!=0&&strcmpi(namaPesanan,"air putih")!=0
	&&strcmpi(namaPesanan,"es jeruk")!=0&&strcmpi(namaPesanan,"es milo")!=0
	&&strcmpi(namaPesanan,"thai tea")!=0&&strcmpi(namaPesanan,"milk shake")!=0)
		return 0;
	else
		return 1;
}
double getHarga(string namaPesanan) {
	if(strcmpi(namaPesanan,"nasi goreng")==0)
		return 15000;
	else if(strcmpi(namaPesanan,"Bakwan")==0)
		return 3000;
	else if(strcmpi(namaPesanan,"nasi ayam")==0)
		return 15000;
	else if(strcmpi(namaPesanan,"nasi telor")==0)
		return 10000;
	else if(strcmpi(namaPesanan,"nasi kucing")==0)
		return 5000;
	else if(strcmpi(namaPesanan,"air putih")==0)
		return 1000;
	else if(strcmpi(namaPesanan,"es jeruk")==0)
		return 3000;
	else if(strcmpi(namaPesanan,"es milo")==0)
		return 5000;
	else if(strcmpi(namaPesanan,"thai tea")==0)
		return 10000;
	else
		return 20000;
}
Menu makeMenu(string namaPesanan, int jumlah, double harga) {
	Menu M;
	strcpy(M.namaPesanan, namaPesanan);
	M.jumlah = jumlah;
	M.harga = harga;
	return M;
}
adrMenu alokasiMenu(Menu M) {
	adrMenu temp;
	temp = (adrMenu) malloc(sizeof(NodeMenu));
	if(temp==NULL)
		return NULL;
	else {
		temp->menu = M;
		temp->nextMenu = NULL;
		return temp;
	}
}
void insertFirstMenu(ListMenu *LM, adrMenu P) {
	P->nextMenu = (*LM);
	(*LM) = P;
}
void insertLastMenu(ListMenu *LM, adrMenu P) {
	if(isEmptyMenu(*LM))
		insertFirstMenu(&(*LM), P);
	else
		insertLastMenu(&(*LM)->nextMenu, P);
}
void insertAfterMenu(adrMenu prec, adrMenu P) {
	P->nextMenu = prec->nextMenu;
	prec->nextMenu = P;
}
void printDataMenu(Menu M) {
	printf("\n\n\n\tNama Pesanan: %s", M.namaPesanan);
	printf("\n\tJumlah Pesanan: %d", M.jumlah);
	printf("\n\tHarga Pesanan (satuan): %.2lf", M.harga);
}
void printMenu(ListMenu LM) {
	if(!isEmptyMenu(LM)) {
		printDataMenu(LM->menu);
		printMenu(LM->nextMenu);
	}
}
bool isOneElementMenu(ListMenu LM) {
	return (LM->nextMenu==NULL);
}
int cekAdaMenu(ListMenu LM, string namaPesanan) {
	if(!isEmptyMenu(LM))
		return (strcmpi(LM->menu.namaPesanan, namaPesanan)==0 || cekAdaMenu(LM->nextMenu, namaPesanan));
	return 0;
}
adrMenu addressOfMenu(ListMenu LM, string namaPesanan) {
	if(!isEmptyMenu(LM)) {
		if(strcmpi(LM->menu.namaPesanan, namaPesanan)==0)
			return LM;
		else
			return addressOfMenu(LM->nextMenu, namaPesanan);
	}
	else
		return NULL;
}
void delFirstMenu(ListMenu *LM) {
	adrMenu hapus;
	hapus = (*LM);
	(*LM) = (*LM)->nextMenu;
	free(hapus);
}
void delLastMenu(ListMenu *LM) {
	if(isOneElementMenu((*LM)))
		delFirstMenu(&(*LM));
	else
		delLastMenu(&(*LM)->nextMenu);
}
void delAtMenu(ListMenu *LM, adrMenu hapus) {
	if((*LM)==hapus)
		delFirstMenu(&(*LM));
	else
		delAtMenu(&(*LM)->nextMenu,hapus);
}
adrMenu getAdrLastMenu(ListMenu LM) {
	if(!isOneElementMenu(LM))
		return getAdrLastMenu(LM->nextMenu);
	return LM;
}


void cekMakanan(Jual M[5], string makanan, int jumlah) {
	if(strcmpi(makanan, "nasi goreng")==0) 
		M[0].jumlah+=jumlah;
	else if(strcmpi(makanan, "bakwan")==0)
		M[1].jumlah+=jumlah;
	else if(strcmpi(makanan, "nasi ayam")==0)
		M[2].jumlah+=jumlah;
	else if(strcmpi(makanan, "nasi telor")==0)
		M[3].jumlah+=jumlah;
	else if(strcmpi(makanan, "nasi kucing")==0)
		M[4].jumlah+=jumlah;
}
void cekMinuman(Jual M[5], string minuman, int jumlah) {
	if(strcmpi(minuman, "air putih")==0) 
		M[0].jumlah+=jumlah;
	else if(strcmpi(minuman, "es jeruk")==0)
		M[1].jumlah+=jumlah;
	else if(strcmpi(minuman, "thai tea")==0)
		M[2].jumlah+=jumlah;
	else if(strcmpi(minuman, "es milo")==0)
		M[3].jumlah+=jumlah;
	else if(strcmpi(minuman, "milk shake")==0)
		M[4].jumlah+=jumlah;
}
void initMakanan(Jual makanan[5]) {
	strcpy(makanan[0].namaMenu, "nasi goreng");
	makanan[0].jumlah = 0;
	strcpy(makanan[1].namaMenu, "bakwan");
	makanan[1].jumlah = 0;
	strcpy(makanan[2].namaMenu, "nasi ayam");
	makanan[2].jumlah = 0;
	strcpy(makanan[3].namaMenu, "nasi telor");
	makanan[3].jumlah = 0;
	strcpy(makanan[4].namaMenu, "nasi kucing");
	makanan[4].jumlah = 0;
}
void initMinuman(Jual minuman[5]) {
	strcpy(minuman[0].namaMenu, "air putih");
	minuman[0].jumlah = 0;
	strcpy(minuman[1].namaMenu, "es jeruk");
	minuman[1].jumlah = 0;
	strcpy(minuman[2].namaMenu, "thai tea");
	minuman[2].jumlah = 0;
	strcpy(minuman[3].namaMenu, "es milo");
	minuman[3].jumlah = 0;
	strcpy(minuman[4].namaMenu, "milk shake");
	minuman[4].jumlah = 0;
}


//-- file handling
void saveFileListKasir(ListNota LNK, int noNota) {
	int sign=0;
	ListNota B = LNK;
	while(B!=NULL) {
		sign++;
		B= B->nextNota;
	}
	if(!isEmptyNota(LNK)) {
		FILE *f;
		f =  fopen("listkasir.txt","w");
		fprintf(f, "No nota terakhir: %d\n", noNota);
		fprintf(f, "N Kasir: %d\n", sign);
		while(LNK!=NULL) {
			fprintf(f, "No Nota: %d\n", LNK->nota.noNota);
			fprintf(f, "Tanggal: %s\n", LNK->nota.tanggal);
			fprintf(f, "No Meja: %d\n", LNK->nota.noMeja);
			fprintf(f, "Total Harga: %lf\n", LNK->nota.totalHarga);
			fprintf(f, "Jumlah Node Menu: %d\n", LNK->nota.jumlahNodeMenu);
			while(LNK->M!=NULL) {
				fprintf(f, "\tNama Pesanan: %s\n", LNK->M->menu.namaPesanan);
				fprintf(f, "\tJumlah: %d\n", LNK->M->menu.jumlah);
				fprintf(f, "\tHarga: %lf\n", LNK->M->menu.harga);
				LNK->M = LNK->M->nextMenu;
			}
			fprintf(f, "\n");
			LNK = LNK->nextNota;
		}
		fclose(f);
	}
}
void saveFileListDapur(ListNota LNK) {
	int sign=0;
	ListNota B = LNK;
	while(B!=NULL) {
		sign++;
		B= B->nextNota;
	}
	if(!isEmptyNota(LNK)) {
		FILE *f;
		f =  fopen("listdapur.txt","w");
		fprintf(f, "N Dapur: %d\n", sign);
		while(LNK!=NULL) {
			fprintf(f, "No Nota: %d\n", LNK->nota.noNota);
			fprintf(f, "Tanggal: %s\n", LNK->nota.tanggal);
			fprintf(f, "No Meja: %d\n", LNK->nota.noMeja);
			fprintf(f, "Total Harga: %lf\n", LNK->nota.totalHarga);
			fprintf(f, "Jumlah Node Menu: %d\n", LNK->nota.jumlahNodeMenu);
			while(LNK->M!=NULL) {
				fprintf(f, "\tNama Pesanan: %s\n", LNK->M->menu.namaPesanan);
				fprintf(f, "\tJumlah: %d\n", LNK->M->menu.jumlah);
				fprintf(f, "\tHarga: %lf\n", LNK->M->menu.harga);
				LNK->M = LNK->M->nextMenu;
			}
			fprintf(f, "\n");
			LNK = LNK->nextNota;
		}
		fclose(f);
	}
}
void loadListKasir(ListNota *LNK, int *noNota) {
	Nota nota;
	Menu menu;
	ListMenu LM;
	adrNota nt;
	createEmpty(&(*LNK));
	int sign, i, k;
	
	FILE *f;
	f = fopen("listkasir.txt","r");
	fscanf(f, "No nota terakhir: %d\n", &(*noNota));
	fscanf(f, "N Kasir: %d\n", &sign);
	for(i=0;i<sign;i++){
		fscanf(f, "No Nota: %d\n", &nota.noNota);
		fscanf(f, "Tanggal: %s\n", &nota.tanggal);
		fscanf(f, "No Meja: %d\n", &nota.noMeja);
		fscanf(f, "Total Harga: %lf\n", &nota.totalHarga);
		fscanf(f, "Jumlah Node Menu: %d\n", &nota.jumlahNodeMenu);
		nt = alokasiNota(nota);
		for(k=0;k<nota.jumlahNodeMenu;k++){
			fscanf(f, "\tNama Pesanan: %[^\n]", &menu.namaPesanan);
			fscanf(f, "\tJumlah: %d\n", &menu.jumlah);
			fscanf(f, "\tHarga: %lf\n", &menu.harga);
			insertLastMenu(&nt->M, alokasiMenu(menu));
		}
		fscanf(f, "\n");
		insertLastNota(&(*LNK), nt);
	}
	fclose(f);
}
void loadListDapur(ListNota *LNK) {
	Nota nota;
	Menu menu;
	ListMenu LM;
	adrNota nt;
	createEmpty(&(*LNK));
	int sign, i, k;
	
	FILE *f;
	f = fopen("listdapur.txt","r");
	fscanf(f, "N Dapur: %d\n", &sign);
	for(i=0;i<sign;i++){
		fscanf(f, "No Nota: %d\n", &nota.noNota);
		fscanf(f, "Tanggal: %s\n", &nota.tanggal);
		fscanf(f, "No Meja: %d\n", &nota.noMeja);
		fscanf(f, "Total Harga: %lf\n", &nota.totalHarga);
		fscanf(f, "Jumlah Node Menu: %d\n", &nota.jumlahNodeMenu);
		nt = alokasiNota(nota);
		for(k=0;k<nota.jumlahNodeMenu;k++){
			fscanf(f, "\tNama Pesanan: %[^\n]", &menu.namaPesanan);
			fscanf(f, "\tJumlah: %d\n", &menu.jumlah);
			fscanf(f, "\tHarga: %lf\n", &menu.harga);
			insertLastMenu(&nt->M, alokasiMenu(menu));
		}
		fscanf(f, "\n");
		insertLastNota(&(*LNK), nt);
	}
	fclose(f);
}
