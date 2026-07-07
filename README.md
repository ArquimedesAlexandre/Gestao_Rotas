este projecto foi feito como parte do nosso projecto curricular por Arquimedes Margareth e Delcia

# Gestao_Rotas

## Descrição
Sistema de gestão de rotas em C para fins académicos. O projeto permite gerir províncias, criar e bloquear ligações rodoviárias, consultar rotas ativas e calcular o menor caminho entre duas localidades.

O sistema está organizado por perfis de acesso:

- `Admin`: monitorizar a rede, cadastrar províncias, criar ou atualizar ligações, bloquear rotas e guardar os dados.
- `User`: listar províncias, visualizar o mapa, procurar rotas ativas e ver o menor caminho.

## Uso de IA
Foi usada IA como apoio técnico durante o desenvolvimento para:

- reorganizar o código em ficheiros separados;
- ajustar a compreender conceitos;
- ajudar na escrita desta documentação.

A IA foi usada como ferramenta de apoio e validação, não como substituto do trabalho de programação e compreensão do código.

## Recursos usados para estudar
Foram consultados e usados como apoio os seguintes recursos:

- documentação da linguagem C;
- documentação do `stdio.h`, `string.h`, `stdlib.h` e `unistd.h`;
- exemplos de listas ligadas e grafos;
- estudo de algoritmos de pesquisa de caminho e menor caminho;
- testes locais no `GCC` e no `VS Code`.

## Como compilar
No Windows, dentro da pasta do projeto, compilar com:

```bash
gcc .\main.c .\admin_functions\admin_functions.c .\user_functions\user_functions.c -o gestao.exe
```

Se estiver a usar outro compilador, basta incluir também os ficheiros `admin_functions/admin_functions.c` e `user_functions/user_functions.c`, além de `main.c`.

## Estrutura do projeto

- `main.c`: menu principal, login e fluxo da aplicação.
- `admin_functions/admin_functions.c`: funções do administrador.
- `user_functions/user_functions.c`: funções do utilizador e helpers comuns.
- `includes/gestao.h`: estruturas, protótipos e definições.
- `DataBase/`: ficheiros com províncias, rotas, login e relatório.
