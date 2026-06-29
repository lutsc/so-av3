#include "fs.h"
#include "block.h"
#include "uart.h"
#include <string.h>

// Constantes FAT
#define SIMPLEFAT_MAGIC 0x53464154 // Flag de SimpleFAT
#define FAT_FREE 0x0000 // Flag cluster livre
#define FAT_EOF 0xFFFF // Flag fim de cluster

// Tamanho e número diversos
#define DISK_SIZE (1024*1024) // Tamanho de 1MB
#define BLOCK_SIZE 512 // Qtd. bytes de cada bloco
#define CLUSTER_SIZE 512 // Qtd. bytes de cada cluster, igual a bloco para encaixar
#define NUM_CLUSTERS 2048 // Qtd. de clusters

#define MAX_FILENAME 32 // Qtd. char para nome do arquivo
#define MAX_DIR_ENTRIES 64 // Qtd. de diretórios

// Layout dos blocos (Baseado em 2048 blocos)
#define SUPERBLOCK_START_BLOCK 0 // Bloco que inicia o superbloco [0]
#define FAT_START_BLOCK 1 // Bloco que inicia a seção de FAT [1 a 8]
#define DIR_START_BLOCK 9 // Bloco que inicia a seção de diretórios [9 a 10]
#define DATA_START_BLOCK 11 // Bloco que inicia a seção de dados [11 a 2047]

// Estrutura do superbloco
typedef struct{
  uint32_t magic; // Número mágico
  uint32_t total_blocks; // Total de blocos
  uint32_t total_clusters; // Total de clusters
  uint32_t cluster_size; // Qtd. bytes da cluster
} superblock_t;

// Estrutura do diretório
typedef struct{
  char name[MAX_FILENAME]; // Nome do arquivo
  uint32_t size; // Qtd. bytes do arquivo
  uint16_t first_cluster; // "Ponteiro" do 1° cluster
} dir_entry_t;

// Variáveis globais
static superblock_t sb;
static uint16_t fat[NUM_CLUSTERS];
static dir_entry_t root[MAX_DIR_ENTRIES];

int fs_init(void){
	// Inicializando superbloco
	sb.magic = SIMPLEFAT_MAGIC;
	sb.total_blocks = DISK_SIZE / BLOCK_SIZE;
	sb.total_clusters = NUM_CLUSTERS;
	sb.cluster_size = CLUSTER_SIZE;
	
	// Alocando espaço pro superbloco
	uint8_t buf[BLOCK_SIZE];
	memset(buf, 0, BLOCK_SIZE);
	memcpy(buf, &sb, sizeof(sb));
	block_write(SUPERBLOCK_START_BLOCK, buf);

	// Inicializando FAT
	memset(fat, 0, sizeof(fat));

	// Inicializando diretório
	memset(root, 0, sizeof(root));
	
	uart_print("SimpleFAT inicializado.\n");
	
	return 0;
}

int cluster_alloc(void){
	for (int i = 1; i < NUM_CLUSTERS; i++) {
		if (fat[i] == FAT_FREE) {
			fat[i] = FAT_EOF;
			return i;
		}
	} 
	uart_print("Sem clusters livres.\n");
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
