#include "fs.h"
#include "block.h"
#include "uart.h"
#include <string.h>

// Constantes
#define SIMPLEFAT_MAGIC 0x53464154 // Flag de SimpleFAT
#define FAT_FREE 0x0000 // Flag cluster livre
#define FAT_EOF 0xFFFF // Flag fim de cluster

#define NUM_CLUSTERS 32 // Qtd. de clusters
#define CLUSTER_SIZE 512 // Qtd. bytes de cada cluster

#define MAX_FILENAME 32 // Qtd. char para nome do arquivo
#define MAX_DIR_ENTRIES 32 // Qtd. de diretórios

// Estrutura do superbloco
typedef struct{
  uint32_t magic;
  uint32_t total_blocks;
  uint32_t total_clusters;
  uint32_t cluster_size; // Qtd. bytes da cluster
} superblock_t;

// Estrutura do diretório
typedef struct{
  char name[MAX_FILENAME]; 
  uint32_t size; // Qtd. bytes do arquivo
  uint16_t first_cluster; // "Ponteiro" do 1° cluster
} dir_entry_t;

// Variáveis globais
static superblock_t sb;
static uint16_t fat[NUM_CLUSTERS];
static dir_entry_t root[MAX_DIR_ENTRIES];

int fs_init(void){
	// Iniciando superbloco
	sb.magic = SIMPLEFAT_MAGIC;
	// sb.total_blocks = // ?
	sb.total_clusters = NUM_CLUSTERS;
	sb.cluster_size = CLUSTER_SIZE;
	
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
