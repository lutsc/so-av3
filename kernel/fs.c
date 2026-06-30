#include "fs.h"
#include "block.h"
#include "uart.h"

// Constantes FAT
#define SIMPLEFAT_MAGIC 0x53464154 // Flag de SimpleFAT
#define FAT_FREE 0x0000 // Flag cluster livre
#define FAT_EOF 0xFFFF // Flag fim de cluster

// Tamanho e número diversos
#define CLUSTER_SIZE 512 // Qtd. bytes de cada cluster, igual a bloco para encaixar
#define NUM_CLUSTERS 2048 // Qtd. de clusters

#define MAX_FILENAME 32 // Qtd. char para nome do arquivo
#define MAX_DIR_ENTRIES 64 // Qtd. máxima de diretórios na root

#define MAX_OPEN_FILES 8 // Qtd. máxima de arquivos em um diretório

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
	uint8_t used; // Ocupado = 1, Livre = 0
} dir_entry_t;

// Estrutura do descritor de arquivo
typedef struct {
	uint32_t pos; // Posição em bytes de leitura/escrita
	uint32_t dir_index; // Índice que fica no diretório root[]
	uint8_t used; // Ocupado = 1, Livre = 0
} file_desc_t;

// Variáveis globais
static superblock_t sb; // Superbloco
static uint16_t fat[NUM_CLUSTERS]; // Clusters
static dir_entry_t root[MAX_DIR_ENTRIES]; // Diretórios
static file_desc_t fdt[MAX_OPEN_FILES]; // Descritores de arquivos

// Funções helper
void mem_set(void *pointer, int value, uint32_t num) {
	uint8_t *ptr = (uint8_t *)pointer;

	for (uint32_t i = 0; i < num; i++)
		ptr[i] = value;
}

void mem_copy(void *destination, void *source, uint32_t num) {
	uint8_t *dst = (uint8_t *)destination;
	uint8_t *src = (uint8_t *)source; 

	for (uint32_t i = 0; i < num; i++)
		dst[i] = src[i];
}

int fs_init(void){
	// Inicializando superbloco
	sb.magic = SIMPLEFAT_MAGIC;
	sb.total_blocks = DISK_SIZE / BLOCK_SIZE;
	sb.total_clusters = NUM_CLUSTERS;
	sb.cluster_size = CLUSTER_SIZE;
	
	// Alocando espaço pro superbloco
	uint8_t buf[BLOCK_SIZE];
	mem_set(buf, 0, BLOCK_SIZE);
	mem_copy(buf, &sb, sizeof(sb));
	block_write(SUPERBLOCK_START_BLOCK, buf);

	// Inicializando FAT
	mem_set(fat, 0, sizeof(fat));

	// Inicializando diretório
	mem_set(root, 0, sizeof(root));
	
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

// Percorre nome do arquivo (array de char) e verifica se são iguais (Igual = 1, Diferente = 0)
int check_available_filename(const char *file1, const char *file2) {
	for (int i = 0; i < MAX_FILENAME; i++) {
		if (file1[i] != file2[i])
			return 0; 
		if (file1[i] == '\0')
			return 1;
	}
	return 1; 
}

int fs_create(const char *name){
	// Procura diretório livre para arquivo
	int free_slot = -1;
	for (int i = 0; i < MAX_DIR_ENTRIES; i++) {
		if (root[i].used) {
			// Checa se há um arquivo do mesmo nome
			if (check_available_filename(root[i].name, name)) {
				uart_print("Arquivo com mesmo nome já existe.\n");
				return -1;
			}
		} else if (free_slot == -1) {
			free_slot = i; // Guarda índice se slot está livre
		}
	}

	if (free_slot == -1) {
		uart_print("Diretório cheio.\n");
		return -1;
	}

	// Procura cluster livre
	int free_cluster = cluster_alloc();
	if(free_cluster == -1){
		return -1;
	}

	// Atualizar diretório
	mem_set(&root[free_slot], 0, sizeof(dir_entry_t));

	// Passa nome do arquivo para o arquivo do slot
	for (int i = 0; (i < (MAX_FILENAME-1)) && (name[i] != '\0'); i++)
		root[free_slot].name[i] = name[i];

	// Inicializa arquivo com 0 bytes de dados, cluster inicial e marca ocupado
	root[free_slot].size = 0;
	root[free_slot].first_cluster = (uint16_t)free_cluster;
	root[free_slot].used = 1;

	uart_print("Arquivo criado: ");
	uart_print(name);
	uart_print("\n");
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
