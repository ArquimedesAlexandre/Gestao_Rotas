#include "../includes/gestao.h"

static int save_provinces_file(t_graph *graph)
{
	FILE *fp;
	int i;

	fp = fopen("DataBase/Provinces.txt", "w");
	if (!fp)
	{
		perror("Erro ao guardar Provinces.txt");
		return (0);
	}
	i = 0;
	while (i < graph->count)
	{
		fprintf(fp, "%s\n", graph->provinces[i].name);
		i++;
	}
	fclose(fp);
	return (1);
}

static int save_roads_file(t_graph *graph)
{
	FILE *fp;
	int source;

	fp = fopen("DataBase/Roads.txt", "w");
	if (!fp)
	{
		perror("Erro ao guardar Roads.txt");
		return (0);
	}
	source = 0;
	while (source < graph->count)
	{
		t_edge *road = graph->provinces[source].roads;

		while (road)
		{
			if (source < road->dest)
			{
				fprintf(fp, "%s,%s,%d,%s\n",
					graph->provinces[source].name,
					graph->provinces[road->dest].name,
					road->distance,
					state_to_text(road->state));
			}
			road = road->next;
		}
		source++;
	}
	fclose(fp);
	return (1);
}

static int write_report_file(t_graph *graph)
{
	FILE *fp;
	int source;
	int unique_roads;
	int blocked_roads;

	fp = fopen("DataBase/Relatorio.txt", "w");
	if (!fp)
	{
		perror("Erro ao guardar Relatorio.txt");
		return (0);
	}
	unique_roads = 0;
	blocked_roads = 0;
	fprintf(fp, "RELATORIO DA REDE\n");
	fprintf(fp, "Total de provincias: %d\n", graph->count);
	source = 0;
	while (source < graph->count)
	{
		t_edge *road = graph->provinces[source].roads;

		while (road)
		{
			if (source < road->dest)
			{
				unique_roads++;
				if (road->state)
					blocked_roads++;
			}
			road = road->next;
		}
		source++;
	}
	fprintf(fp, "Total de rotas: %d\n", unique_roads);
	fprintf(fp, "Rotas bloqueadas: %d\n\n", blocked_roads);
	source = 0;
	while (source < graph->count)
	{
		fprintf(fp, "Provincia: %s\n", graph->provinces[source].name);
		t_edge *road = graph->provinces[source].roads;

		while (road)
		{
			if (source < road->dest)
			{
				fprintf(fp, "  %s <-> %s | %d km | %s\n",
					graph->provinces[source].name,
					graph->provinces[road->dest].name,
					road->distance,
					state_to_text(road->state));
			}
			road = road->next;
		}
		fprintf(fp, "\n");
		source++;
	}
	fclose(fp);
	return (1);
}

void monitorar_rede(t_graph *graph)
{
	t_graph *aux;

	aux = graph;
	for (int i = 0; i < aux->count; i++)
	{
		printf("Provincia: %s\n", aux->provinces[i].name);
		t_edge *road = aux->provinces[i].roads;
		while (road)
		{
			printf("  -> %s (distancia: %d) (estado: %s)\n",
				aux->provinces[road->dest].name,
				road->distance,
				state_to_text(road->state));
			road = road->next;
		}
	}
}

void cadastrar_provincia(t_graph *graph)
{
	char name[100];

	printf("Digite o nome da nova provincia: ");
	discard_line();
	read_line(name, sizeof(name));
	add_province(graph, name);
}

void criar_ligacao(t_graph *graph)
{
	char src[100];
	char dest[100];
	int distance;
	int state;
	int pos_src;
	int pos_dest;

	printf("Digite o nome da provincia de origem: ");
	discard_line();
	read_line(src, sizeof(src));
	printf("Digite o nome da provincia de destino: ");
	read_line(dest, sizeof(dest));
	printf("Digite a distancia entre as provincias: ");
	scanf("%d", &distance);
	printf("Digite o estado da rota (0 - normal, 1 - bloqueada): ");
	scanf("%d", &state);

	pos_src = find_province(graph, src);
	pos_dest = find_province(graph, dest);
	if (pos_src == -1 || pos_dest == -1)
	{
		printf("O nome de uma das provincias nao existe!\n");
		return;
	}
	add_road(graph, src, dest, distance, state);
	add_road(graph, dest, src, distance, state);
}

void bloquear_rota(t_graph *graph)
{
	char src[100];
	char dest[100];
	int pos_src;
	int pos_dest;
	t_edge *road;
	t_edge *reverse_road;

	printf("digite o nome da provincia de origem: ");
	discard_line();
	read_line(src, sizeof(src));
	printf("digite o nome da provincia de destino: ");
	read_line(dest, sizeof(dest));

	pos_src = find_province(graph, src);
	pos_dest = find_province(graph, dest);

	if (pos_src == -1 || pos_dest == -1)
	{
		printf("O nome de uma das provincias nao existe!\n");
		return;
	}
	road = find_road(graph, pos_src, pos_dest);
	if (!road)
	{
		printf("Nao existe rota entre essas provincias!\n");
		return;
	}
	road->state = 1;
	reverse_road = find_road(graph, pos_dest, pos_src);
	if (reverse_road)
		reverse_road->state = 1;

	printf("Rota bloqueada com sucesso!\n");
}

void guardar_dados(t_graph *graph)
{
	int ok;

	ok = save_provinces_file(graph);
	ok = save_roads_file(graph) && ok;
	ok = write_report_file(graph) && ok;
	if (ok)
		printf("Dados guardados com sucesso e relatorio gerado em DataBase/Relatorio.txt\n");
}
