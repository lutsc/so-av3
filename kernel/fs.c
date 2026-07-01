#include "fs.h"
#include "block.h"
#include "uart.h"

#define NULL ((void*)0) // NULL

// Constantes SimpleFAT
#define SIMPLEFAT_MAGIC 0x53464154 // Flag de SimpleFAT
#define FAT_FREE 0x0000 					 // Flag cluster livre
#define FAT_EOF 0xFFFF 						 // Flag fim de arquivo

// Tamanho e número diversos
#define CLUSTER_SIZE 512 	// Qtd. bytes de cada cluster, igual a bloco para encaixar
#define NUM_CLUSTERS 2048 // Qtd. de total clusters

#define MAX_FILENAME 32 	 // Qtd. char para nome do arquivo
#define MAX_DIR_ENTRIES 64 // Qtd. máxima de diretórios na root

#define MAX_OPEN_FILES 8 // Qtd. máxima de arquivos em um diretório

// Layout dos blocos (Baseado em 2048 blocos)
#define SUPERBLOCK_START_BLOCK 0 // Bloco que inicia o superbloco [0]
#define FAT_START_BLOCK 1 // Bloco que inicia a seção de FAT [1 a 8]
#define DIR_START_BLOCK 9 // Bloco que inicia a seção de diretórios [9 a 10]
#define DATA_START_BLOCK 14 // Bloco que inicia a seção de dados [11 a 2047]

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

// Percorre nome do arquivo (array de char) e verifica se são iguais (Igual = 1, Diferente = 0)
int check_same_filename(const char *file1, const char *file2) {
	for (int i = 0; i < MAX_FILENAME; i++) {
		if (file1[i] != file2[i])
			return 0; 
		if (file1[i] == '\0')
			return 1;
	}
	return 1; 
}

// Passa dados das variáveis globais pro disco virtual 
void fs_sync(void) {
  uint8_t buffer[BLOCK_SIZE];

	uint32_t RESERVED_FAT_BLOCKS = (DIR_START_BLOCK - FAT_START_BLOCK); // 9 - 1 = 8 blocos
  uint32_t ENTRIES_PER_BLOCKS = BLOCK_SIZE / sizeof(uint16_t); // 512 / 2 = 256 entradas
	
  // Salva mudanças da FAT nos clusters reservados 
  for (uint32_t i = 0; i < RESERVED_FAT_BLOCKS; i++) {
		// Limpa buffer por precaução
		mem_set(buffer, 0, BLOCK_SIZE);

		// Ponteiro de onde começa o bloco que deve ser escrito
		uint16_t *fat_ptr = &fat[i * ENTRIES_PER_BLOCKS];

		// Copia pro buffer e escreve no bloco
    mem_copy(buffer, fat_ptr, BLOCK_SIZE);
    block_write(FAT_START_BLOCK + i, buffer);
  }

	uint32_t RESERVED_DIR_BLOCKS = ((DATA_START_BLOCK - DIR_START_BLOCK)); // 14 - 9 = 5 blocos

  // Salva mudanças da root nos clusters reservados
  for (uint32_t i = 0; i < RESERVED_DIR_BLOCKS; i++) {
		// Limpa buffer por precaução
		mem_set(buffer, 0, BLOCK_SIZE);

		// Cálculo para o quanto a cluster vai ser preenchida
    uint32_t block_offset = i * BLOCK_SIZE;

		// Se o que resta do root for menor que precisa, copia apenas o que cabe
    uint32_t chunk;
    if ((sizeof(root) - block_offset) < BLOCK_SIZE) {
      chunk = sizeof(root) - block_offset;
    } else {
      chunk = BLOCK_SIZE;
    }

		// Passa pedaço que vai ser escrito na root e depois escreve como bloco no disco virtual
    mem_copy(buffer, &((uint8_t *)root)[block_offset], chunk);
    block_write(DIR_START_BLOCK + i, buffer);
  }
}

// Inicia pastas
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

// Cria arquivo
int fs_create(const char *name){
	// Procura diretório livre para arquivo
	int directory_index = -1;
	for (int i = 0; i < MAX_DIR_ENTRIES; i++) {
		if (root[i].used) {
			// Checa se há um arquivo do mesmo nome
			if (check_same_filename(root[i].name, name)) {
				uart_print("Arquivo com mesmo nome já existe.\n");
				return -1;
			}
		} else if (directory_index == -1) {
			directory_index = i; // Guarda índice se slot está livre
		}
	}

	if (directory_index == -1) {
		uart_print("Diretório cheio.\n");
		return -1;
	}

	// Procura cluster livre
	int free_cluster = cluster_alloc();
	if(free_cluster == -1){
		return -1;
	}

	// Atualizar diretório
	mem_set(&root[directory_index], 0, sizeof(dir_entry_t));

	// Passa nome do arquivo para o arquivo do slot
	for (int i = 0; i < (MAX_FILENAME-1) && (name[i] != '\0'); i++)
		root[directory_index].name[i] = name[i];

	// Inicializa arquivo com 0 bytes de dados, cluster inicial e marca ocupado
	root[directory_index].size = 0;
	root[directory_index].first_cluster = (uint16_t)free_cluster;
	root[directory_index].used = 1;

	uart_print("Arquivo criado: ");
	uart_print(name);
	uart_print("\n");
	return 0;
}

int fs_open(const char *name){
	// Percorre diretório pelo arquivo dado e devolve índice
	int directory_index = -1;
	for (int i = 0; i < MAX_DIR_ENTRIES; i++) {
		if (root[i].used && check_same_filename(root[i].name, name)) {
			directory_index = i;
			break;
		}
	}

	if (directory_index == -1) {
		uart_print("Arquivo não encontrado.\n");
		return -1;
	}

	// Percorre diretório por um descritor livre e devolve índice
	for (int i = 0; i < MAX_OPEN_FILES; i++) {
		if (fdt[i].used == 0) {
			fdt[i].pos = 0;
			fdt[i].dir_index = directory_index;
			fdt[i].used = 1;
			return i;
		}
	}
	uart_print("Sem descritores livres.\n");
	return -1;
}

int fs_write(int fd, const void *buffer, uint32_t size){
	// Verifica se tem diretórios disponíveis (inicializados, abaixo do máximo e ainda não aberto)
	if (fd < 0 || fd >= MAX_OPEN_FILES || fdt[fd].used == 0) 
		return -1;
    
	// Se não escrever ou alocar nada, retorna
	if (buffer == NULL || size == 0) 
		return -1;

	// Aloca um descritor novo com o índice dado
	dir_entry_t *entry = &root[fdt[fd].dir_index];

	// Libera cadeia anterior para reescrever do início
	uint16_t current = entry->first_cluster;
	while (current != FAT_EOF && current != FAT_FREE) {
		uint16_t next = fat[current];
		fat[current] = FAT_FREE;
		current = next;
	}

	// Cálculo de num. de clusters necessária pro arquivo
	uint32_t num_clusters = ((size + CLUSTER_SIZE - 1) / CLUSTER_SIZE);
	uint16_t first = FAT_EOF, prev = FAT_EOF;
	const uint8_t *buf = (const uint8_t *)buffer;

	// Loop de alocação e escrita
	for (uint32_t i = 0; i < num_clusters; i++) {
		int free_cluster = cluster_alloc();
		if (free_cluster == -1) 
			return -1;

		// 
		if (i == 0) {
			first = (uint16_t)free_cluster;
		} else {
			fat[prev] = (uint16_t)free_cluster;
		}
		prev = (uint16_t)free_cluster;

		// Copiar dados para o bloco do cluster
		uint8_t block[BLOCK_SIZE];
		mem_set(block, 0, BLOCK_SIZE);

		// Cálculo de quanto o dado vai ser preenchida na cluster
		uint32_t offset = i * CLUSTER_SIZE;
		uint32_t chunk;
		if ((size - offset) < CLUSTER_SIZE){
			chunk = (size - offset);
		} else {
			chunk = CLUSTER_SIZE;
		}

		for (uint32_t j = 0; j < chunk; j++)
			block[j] = buf[offset + j];

		block_write(DATA_START_BLOCK + (free_cluster - 1), block);
	}

	// Atualiza diretório
	fdt[fd].pos = size;
	entry->size = size;
	entry->first_cluster = first;
	return (int)size;
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
	// Percorre diretório pelo arquivo dado e devolve índice
	int directory_index = -1;
	for (int i = 0; i < MAX_DIR_ENTRIES; i++) {
		if (root[i].used && check_same_filename(root[i].name, name)) {
			directory_index = i;
			break;
		}
	}

	if (directory_index == -1) {
		uart_print("Arquivo não encontrado.\n");
		return -1;
	}

	// Percorre clusters, liberando cada uma
	uint16_t current = root[directory_index].first_cluster;
	while (current != FAT_EOF && current != FAT_FREE) {
		uint16_t next = fat[current];
		fat[current] = FAT_FREE;
		current = next;
	}
	
	// Remove arquivo do diretório
	mem_set(&root[directory_index], 0, sizeof(dir_entry_t));

	uart_print("Arquivo removido: ");
	uart_print(name);
	uart_print("\n");
	return 0;
}

int fs_close(int fd){
	if (fd < 0 || fd >= MAX_OPEN_FILES || fdt[fd].used == 0) {
    uart_print("Erro: Descritor inválido ou fechado.\n");
    return -1;
  }

  // Zera descritor
  fdt[fd].pos = 0;
  fdt[fd].dir_index = 0;
  fdt[fd].used = 0;

	return 0;
}
