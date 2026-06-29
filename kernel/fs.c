#include "fs.h"
#include "block.h"
#include "uart.h"
#include <string.h>

#define SIMPLEFAT_MAGIC 0x53464154 // Flag de SimpleFAT
#define FAT_FREE 0x0000 // Flag cluster livre
#define FAT_EOF 0xFFFF // Flag fim de cluster

#define NUM_CLUSTERS 32 // Qtd. teste

static uint16_t fat[NUM_CLUSTERS];

int fs_init(void){
	/*
	* TODO:
	* Inicializar superbloco.
	*/
	/*
	* TODO:
	* Inicializar FAT.
	*/
	/*
	* TODO:
	* Inicializar diretrio.
	*/
	return 0;
}

int cluster_alloc(void){
	/*
	* TODO:
	* Procurar FAT_FREE.
	*/
	return -1;
}

int fs_create(const char *name){
	/*
	* TODO:
	* Procurar entrada livre.
	*/
	/*
	* TODO:
	* Alocar cluster inicial.
	*/
	/*
	* TODO:
	* Atualiza Diretrio
	*/
	return 0;
}

int fs_open(const char *name){
	/*
	* TODO:
	* Localizar entrada.
	*/
	return 0;
}

int fs_write(int fd, const void *buffer, uint32_t size){
	/*
	* TODO:
	* Alocar clusters.
	*/
	/*
	* TODO:
	* Atualizar FAT.
	*/
	/*
	* TODO:
	* Copiar dados.
	*/
	return size;
}

int fs_read(int fd, void *buffer, uint32_t size){
	/*
	* TODO:
	* Percorrer FAT.
	*/
	/*
	* TODO:
	* Copiar dados.
	*/
	return size;
}

int fs_delete(const char *name){
	/*
	* TODO:
	* Localizar arquivo.
	*/
	/*
	* TODO:
	* Liberar clusters.
	*/
	/*
	* TODO:
	* Remover entrada.
	*/
	return 0;
}

int fs_close(int fd){
	/*
	* TODO:
	*	Liberar descritor.
	*/
	
	return 0;
}
