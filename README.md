## MicroKernel RISC-V 64 Bits em C

Implementação de um microkernel para a arquitetura RISC-V (RV64), desenvolvido para a disciplina de Sistemas Operacionais.
O projeto executa em Supervisor Mode (S-Mode) utilizando OpenSBI como firmware de inicialização e QEMU como ambiente de simulação, sendo um ambiente bare-metal, sem sistema operacional hospedeiro.

O projeto está dividido em duas partes:

1. Núcleo do microkernel — boot, gerência de memória, tarefas, escalonamento e tratamento de interrupções.
2. Sistema de arquivos (SimpleFAT) — driver de blocos, superbloco, tabela de alocação (FAT) e operações de arquivo em disco virtual na memória.

---

### Núcleo

- Boot em Assembly RISC-V
- Inicialização do ambiente de execução
- Driver UART para saída serial
- Gerenciamento de memória simples com kmalloc()
- Criação de tarefas com pilha
- Estruturas TCB (Task Control Block)
- Chaveamento de contexto completo em Assembly
- Escalonador por Round-Robin e escalonamento cooperativo
- Suporte a preempção por timer
- Tratamento de traps e interrupções com troca de contexto a partir de trap

### Sistema de arquivos (SimpleFAT)

- Driver de disco virtual em memória
- Superbloco com número mágico, total de blocos, total de clusters e tamanho de cluster
- Tabela de alocação de clusters (FAT)
- Layout de disco fixo para 2048 blocos: superbloco, região da FAT, região de diretório e região de dados
- Diretório raiz com entradas de até 64 arquivos (nome, tamanho, primeiro cluster, flags)
- Tabela de descritores de arquivo aberto (até 8 arquivos simultâneos) com leitura/escrita
- Operações completas de arquivo:
  - fs_init - inicializa o sistema de arquivos
  - fs_create - cria arquivo
  - fs_open / fs_close - abre/fecha arquivo
  - fs_read / fs_write - leitura e escrita em clusters encadeados, com alocação dinâmica de clusters e verificação de erro
  - fs_delete - remove arquivo e libera seus clusters
  - fs_sync - transfere superbloco, FAT e diretório da RAM para o disco virtual
  - cluster_alloc - aloca o próximo cluster livre na FAT
- Funções auxiliares para substituir a libc
- Failsafes para leitura/escrita em clusters desalinhados e para erros de leitura/escrita de bloco

---

## Build e execução

Requer toolchain riscv64-unknown-elf-gcc e qemu-system-riscv64

```bash
make        # compila e gera kernel.elf
make run    # executa no QEMU
make clean  # remove objs e o bin
```

---

## Objetivo

Este projeto tem como objetivo implementar um microkernel simples para a arquitetura RISC-V, permitindo o estudo prático dos principais conceitos de Sistemas Operacionais, incluindo inicialização do sistema, gerenciamento de memória, gerenciamento de tarefas, escalonamento, tratamento de interrupções e implementação de sistema de arquivos.
