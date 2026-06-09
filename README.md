Implementação de um microkernel educacional para a arquitetura *RISC-V (RV64)*, desenvolvido para a disciplina de Sistemas Operacionais.

O projeto executa em Supervisor Mode (S-Mode) utilizando OpenSBI como firmware de inicialização e QEMU como ambiente de simulação.

Funcionalidades:
- Boot em Assembly RISC-V
- Inicialização do ambiente de execução
- Driver UART para saída serial
- Gerenciamento de memória com kmalloc()
- Criação de tarefas (Tasks)
- Estruturas TCB (Task Control Block)
- Chaveamento de contexto completo
- Escalonador Round-Robin
- Escalonamento cooperativo
- Suporte a preempção por timer
- Tratamento de traps e interrupções
- Ambiente totalmente bare-metal

Este projeto tem como objetivo implementar um microkernel simples para a arquitetura RISC-V, permitindo o estudo prático dos principais conceitos de Sistemas Operacionais, incluindo inicialização do sistema, gerenciamento de memória, gerenciamento de tarefas, escalonamento e tratamento de interrupções.
