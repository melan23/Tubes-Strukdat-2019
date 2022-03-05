#include "header.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	int noNota = 0, noMeja, jumlah, noNotaCari, nkasir=0;
	double totalHarga, harga, omset=0;
	string tanggal, namaPesanan;
	char pilih, pilPembayaran;
	int i,maks=0;
	string yaTidak;
	
	ListNota LND;
	ListNota LNK;
	
	ListMenu LM;
	ListMenu LMD;
	
	Menu menu;
	Nota nota;
	
	adrNota N;
	adrNota ND;
	adrNota temp;
	adrMenu M, M2;
	adrMenu MD;
	adrMenu tempMenu;
	
	createEmpty(&LND);
	createEmpty(&LNK);
	
	createEmptyMenu(&LM);
	createEmptyMenu(&LMD);
	
	Jual makanan[5];
	Jual minuman[5];
	
	//m makanan dan minuman yang dijual
	initMakanan(makanan);
	initMinuman(minuman);
	
	// tanggal
	time_t t = time(NULL);
  	struct tm tm = *localtime(&t);
  	

  	do {
	 	loadListKasir(&LNK, &noNota);
  		loadListDapur(&LND);
  		
  		system("CLS");
  		printf("\t---La Grande---\n");
  		printf("[1]. Pesan makanan\n");
  		printf("[2]. Tambah pesanan\n");
  		printf("[3]. Pembayaran\n");
  		printf("[4]. Laporan\n");
  		printf("[5]. Tampil List Kasir\n");
  		printf("[6]. Tampil List Dapur\n");
  		printf("[0]. Keluar program\n");
  		printf("Pilih [1/2/3/..]: ");pilih = getchar();
  		
  		switch(pilih) {
  			case '1':
  				printf("\n\t=====PESAN MAKANAN=====\n");
  				tampilMenu();
  				createEmptyMenu(&LM);
  				createEmptyMenu(&LMD);
  				do {
  					printf("\nNomor meja: ");scanf("%d", &noMeja);
  					if(noMeja < 1)
  						printf("\n[!] NOMOR MEJA TIDAK VALID !");
				}while(noMeja < 1);
				noNota++;
				nkasir++;
				sprintf(tanggal,"%d-%d-%d",tm.tm_mday, tm.tm_mon, tm.tm_year + 1900); 
				nota = makeNota(noNota, tanggal, noMeja);
				N = alokasiNota(nota);
				ND = alokasiNota(nota);
				insertLastNota(&LNK, N);
				insertLastNota(&LND, ND);
				
				do {
					do {
						printf("\nPilih menu: ");fflush(stdin);gets(namaPesanan);
						if(cekValidPesanan(namaPesanan)==0)
							printf("\n[!] MENU TIDAK ADA !");
					}while(cekValidPesanan(namaPesanan)==0);
					do {
						printf("\nJumlah pesanan: ");scanf("%d", &jumlah);
						if(jumlah < 1)
							printf("\n[!] JUMLAH PESANAN TIDAK VALID !");
					}while(jumlah < 1);
					N->nota.jumlahNodeMenu++;
					ND->nota.jumlahNodeMenu++;
					menu = makeMenu(namaPesanan, jumlah, getHarga(namaPesanan));
					M = alokasiMenu(menu);
					MD = alokasiMenu(menu);
					insertLastMenu(&LM, M);
					insertLastMenu(&LMD, MD);
					do {
						printf("\nApakah ingin memesan lagi [Ya/Tidak]?");fflush(stdin);gets(yaTidak);
					}while(strcmpi(yaTidak,"Ya")!=0&&strcmpi(yaTidak,"Tidak")!=0);
				}while(strcmpi(yaTidak,"Ya")==0);
				N->nota.totalHarga = hitungTotalHarga(LM);
				ND->nota.totalHarga = hitungTotalHarga(LMD);
				N->M = LM;
				ND->M = LMD;
  				break;
  			case '2':
  				printf("\n\t=====TAMBAH PESANAN=====\n");
  				if(!isEmptyNota(LNK)) {
  					printf("\nMasukan no nota: ");scanf("%d",&noNotaCari);
  					N = addressOf(LNK, noNotaCari);
  					ND = addressOf(LND, noNotaCari);
  					if(N!=NULL && ND!=NULL) {
  						tampilMenu();
						do {
							printf("\nPilih menu: ");fflush(stdin);gets(namaPesanan);
							if(cekValidPesanan(namaPesanan)==0)
								printf("\n[!] MENU TIDAK ADA !");
						}while(cekValidPesanan(namaPesanan)==0);
						do {
							printf("\nJumlah pesanan: ");scanf("%d", &jumlah);
							if(jumlah < 1)
								printf("\n[!] JUMLAH PESANAN TIDAK VALID !");
						}while(jumlah < 1);
						menu = makeMenu(namaPesanan, jumlah, getHarga(namaPesanan));
						M = alokasiMenu(menu);
						M2 = alokasiMenu(menu);
						tempMenu = getAdrLastMenu(N->M);
						tempMenu->nextMenu = M;
						tempMenu = getAdrLastMenu(ND->M);
						tempMenu->nextMenu = M2;
						N->nota.totalHarga = hitungTotalHarga(N->M);
						N->nota.jumlahNodeMenu++;
						ND->nota.totalHarga = hitungTotalHarga(ND->M);
						ND->nota.jumlahNodeMenu++;
					}
					else
						printf("\n[!] NO NOTA TIDAK ADA !");
				}
				else
					printf("\n[!] BELUM ADA NOTA !");
  				break;
  			case '3':
  				do {
  					system("CLS");
  					printf("\n\t=====PEMBAYARAN=====\n");
  					printf("\t1. Langsung Bayar\n");
  					printf("\t2. Gabung Nota\n");
  					printf("\t3. Pisah Nota \n");
  					printf("\t0. Kembali ke Menu Utama\n");
  					printf("\tPilih menu [1/2/..]: ");pilPembayaran = getchar();
  					
  					switch(pilPembayaran) {
  						case '1':
  							printf("\nMasukan no nota: ");scanf("%d", &noNotaCari);
  							temp = addressOf(LNK, noNotaCari);
  							if(temp != NULL) {
  								pembayaran(&LNK, temp, makanan, minuman, &omset);
							}
							else
								printf("\n[!] NOTA TIDAK DITEMUKAN !");
  							break;
  						case '2':
  							gabungNota(&LNK, &noNota, makanan, minuman, &omset);
  							break;
  						case '3':
  							pisahNota(&LNK, &noNota, makanan, minuman, &omset);
  							break;
  						case '0':
  							printf("\nKembali ke Menu Utama..");
  							break;
					  }
					  getch();
				  }while(pilPembayaran!='1'&&pilPembayaran!='2'&&pilPembayaran!='3'&&pilPembayaran!='0');
  				break;
  			case '4':
  				for(i=0;i<5;i++) {
  					if(makanan[i].jumlah > maks && makanan[i].jumlah != 0) {
  						maks = makanan[i].jumlah;
					  }
				  }
				  printf("\nJumlah makanan terbanyak: ");
				  for(i=0;i<5;i++) {
				  	if(makanan[i].jumlah==maks)
				  		printf("\n%s dengan jumlah pembelian: %d", makanan[i].namaMenu, makanan[i].jumlah);
				  }
				  maks = 0;
				  for(i=0;i<5;i++) {
  					if(minuman[i].jumlah > maks) {
  						maks = minuman[i].jumlah;
					  }
				  }
				  printf("\n\nJumlah minuman terbanyak: ");
				  for(i=0;i<5;i++) {
				  	if(minuman[i].jumlah==maks) {
				  		printf("\n%s dengan jumlah pembelian: %d", minuman[i].namaMenu, minuman[i].jumlah);
					  }
				  }
				  printf("\n\nTotal Omzet: Rp%.2lf", omset);
  				break;
  			case '5':
  				printf("\n\t===ISI LIST KASIR===");
  				if(!isEmptyNota(LNK))
  					printNota(LNK);
  				else
  					printf("\n[!] BELUM ADA NOTA !");
  				break;
  			case '6':
  				printf("\n\t===ISI LIST DAPUR===");
  				if(!isEmptyNota(LND))
  					printNota(LND);
  				else
  					printf("\n[!] BELUM ADA NOTA !");
  				break;
  			case '0':
  				printf("\nKeluar program...");
  				break;
		}
		getch();
		saveFileListKasir(LNK, noNota);
		saveFileListDapur(LND);
	}while(pilih!='0');
	
	return 0;
}
