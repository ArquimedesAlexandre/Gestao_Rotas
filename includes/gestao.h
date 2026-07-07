#ifndef GRAPH_H
# define GRAPH_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define MAX_NAME 50
# define MAX_PROVINCES 30

/*
** Tipos de utilizador
*/
typedef enum e_role
{
	USER,
	ADMIN
}	t_role;

/*
** Utilizador
*/
typedef struct s_user
{
	char	username[MAX_NAME];
	char	password[MAX_NAME];
	t_role	role;
}	t_user;

/*
** Estrada (Aresta)
*/
typedef struct s_edge
{
	int				dest;
	int				distance;
	int				state;
	struct s_edge	*next;
}	t_edge;

/*
** Província (Vértice)
*/
typedef struct s_vertex
{
	char	name[MAX_NAME];
	t_edge	*roads;
	int	state;
}	t_vertex;

/*
** Grafo
*/
typedef struct s_graph
{
	int			count;
	t_vertex	provinces[MAX_PROVINCES];
}	t_graph;

void read_line(char *buffer, size_t size);
void discard_line(void);
const char *state_to_text(int state);
void print_path(int prev[MAX_PROVINCES], int source, int destination, t_graph *graph);
void get_users(t_user *admin, t_user *user);
int find_province(t_graph *graph, char *name);
t_edge *find_road(t_graph *graph, int source, int destination);
void add_road(t_graph *graph, char *src, char *dest, int distance, int state);
void load_roads(t_graph *graph);
void add_province(t_graph *graph, char *name);
void load_provinces(t_graph *graph);
void get_graph(t_graph *graph);
void load_datas(t_user *admin, t_user *user, t_graph *graph);
int login(t_user account);
void guardar_dados(t_graph *graph);
void monitorar_rede(t_graph *graph);
void cadastrar_provincia(t_graph *graph);
void criar_ligacao(t_graph *graph);
void bloquear_rota(t_graph *graph);
void listar_provincias(t_graph *graph);
void mostrar_mapa(t_graph *graph);
void procurar_rota(t_graph *graph);
void menor_caminho(t_graph *graph);

#endif