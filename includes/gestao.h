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

int find_province(t_graph *graph, char *name);
t_edge *find_road(t_graph *graph, int source, int destination);

#endif