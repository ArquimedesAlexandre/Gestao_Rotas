#include "../includes/gestao.h"

static int state_from_text(const char *text)
{
	if (!text)
		return (0);
	if (strcmp(text, "1") == 0 || strcmp(text, "Blocked") == 0 || strcmp(text, "blocked") == 0
		|| strcmp(text, "Bloqueada") == 0 || strcmp(text, "bloqueada") == 0)
		return (1);
	return (0);
}

const char *state_to_text(int state)
{
	if (state)
		return ("Blocked");
	return ("Active");
}

void read_line(char *buffer, size_t size)
{
	if (!fgets(buffer, size, stdin))
	{
		buffer[0] = '\0';
		return;
	}
	buffer[strcspn(buffer, "\n")] = '\0';
	buffer[strcspn(buffer, "\r")] = '\0';
}

void discard_line(void)
{
	int c;

	while ((c = getchar()) != '\n' && c != EOF)
		;
}

void print_path(int prev[MAX_PROVINCES], int source, int destination, t_graph *graph)
{
	int path[MAX_PROVINCES];
	int count;
	int current;
	int i;

	count = 0;
	current = destination;
	while (current != -1 && count < MAX_PROVINCES)
	{
		path[count++] = current;
		if (current == source)
			break;
		current = prev[current];
	}
	if (count == MAX_PROVINCES || path[count - 1] != source)
	{
		printf("Nao foi possivel reconstruir a rota.\n");
		return;
	}
	i = count - 1;
	while (i >= 0)
	{
		printf("%s", graph->provinces[path[i]].name);
		if (i > 0)
			printf(" -> ");
		i--;
	}
	printf("\n");
}

void get_users(t_user *admin, t_user *user)
{
	FILE    *fp;
	char    line[200];
	char    role[20];
	char    cwd[256];

	getcwd(cwd, sizeof(cwd));
	printf("Diretorio atual: %s\n", cwd);

	fp = fopen("DataBase/Login.txt", "r");
	if (!fp)
	{
		perror("Erro ao abrir Login.txt");
		return;
	}

	if (fgets(line, sizeof(line), fp))
	{
		sscanf(line, "%49[^,],%49[^,],%19s", admin->username, admin->password, role);
	}
	if (fgets(line, sizeof(line), fp))
	{
		sscanf(line, "%49[^,],%49[^,],%19s", user->username, user->password, role);
	}
	fclose(fp);
}

int find_province(t_graph *graph, char *name)
{
	int i;

	i = 0;
	while (i < graph->count)
	{
		if (strcmp(graph->provinces[i].name, name) == 0)
			return (i);
		i++;
	}
	return (-1);
}

t_edge *find_road(t_graph *graph, int source, int destination)
{
	t_edge *road;

	road = graph->provinces[source].roads;
	while (road)
	{
		if (road->dest == destination)
			return (road);
		road = road->next;
	}
	return (NULL);
}

void add_road(t_graph *graph, char *src, char *dest, int distance, int state)
{
	t_edge *new;
	int source;
	int destination;
	t_edge *existing;

	source = find_province(graph, src);
	destination = find_province(graph, dest);
	if (source == -1 || destination == -1)
	{
		printf("Erro: uma das provincias nao existe!\n");
		return;
	}
	existing = find_road(graph, source, destination);
	if (existing)
	{
		existing->distance = distance;
		existing->state = state;
		return;
	}
	new = malloc(sizeof(t_edge));
	if (!new)
	{
		printf("Erro de memoria ao criar rota!\n");
		return;
	}
	new->dest = destination;
	new->distance = distance;
	new->state = state;
	new->next = graph->provinces[source].roads;
	graph->provinces[source].roads = new;
}

void load_roads(t_graph *graph)
{
	FILE *fp;
	char line[100];
	char src[50];
	char dest[50];
	int distance;
	char state_text[50];
	int state;

	fp = fopen("DataBase/Roads.txt", "r");
	if (!fp)
		return;
	while (fgets(line, sizeof(line), fp))
	{
		if (sscanf(line, "%49[^,],%49[^,],%d, %49[^\n]", src, dest, &distance, state_text) != 4)
			continue;
		state = state_from_text(state_text);
		add_road(graph, src, dest, distance, state);
		add_road(graph, dest, src, distance, state);
	}
	fclose(fp);
}

void add_province(t_graph *graph, char *name)
{
	if (graph->count >= MAX_PROVINCES)
	{
		printf("Erro: numero maximo de provincias excedido!\n");
		return;
	}
	if (strlen(name) >= MAX_NAME)
	{
		printf("Erro: nome da provincia demasiado longo!\n");
		return;
	}
	strncpy(graph->provinces[graph->count].name, name, MAX_NAME - 1);
	graph->provinces[graph->count].name[MAX_NAME - 1] = '\0';
	graph->provinces[graph->count].roads = NULL;
	graph->count++;
}

void load_provinces(t_graph *graph)
{
	FILE *fp;
	char line[100];

	fp = fopen("DataBase/Provinces.txt", "r");
	if (!fp)
		return;
	graph->count = 0;
	while (fgets(line, sizeof(line), fp))
	{
		line[strcspn(line, "\n")] = '\0';
		add_province(graph, line);
	}
	fclose(fp);
}

void get_graph(t_graph *graph)
{
	load_provinces(graph);
	load_roads(graph);
}

void load_datas(t_user *admin, t_user *user, t_graph *graph)
{
	get_users(admin, user);
	get_graph(graph);
}

int login(t_user account)
{
	char username[50];
	char password[50];

	while (1)
	{
		printf("Username (0 para sair): ");
		scanf("%49s", username);
		if (strcmp(username, "0") == 0)
			return (0);
		printf("Password: ");
		scanf("%49s", password);
		if (strcmp(username, account.username) == 0 && strcmp(password, account.password) == 0)
		{
			printf("Login efetuado com sucesso!\n");
			return (1);
		}
		printf("Credenciais invalidas! Tente novamente.\n\n");
	}
}

void listar_provincias(t_graph *graph)
{
	int i;

	printf("\n===== PROVINCIAS =====\n");
	i = 0;
	while (i < graph->count)
	{
		printf("%d - %s\n", i + 1, graph->provinces[i].name);
		i++;
	}
}

void mostrar_mapa(t_graph *graph)
{
	int i;

	printf("\n===== MAPA RODOVIARIO =====\n");
	i = 0;
	while (i < graph->count)
	{
		t_edge *road = graph->provinces[i].roads;
		while (road)
		{
			if (i < road->dest)
			{
				printf("%s <-> %s | %d km | %s\n",
					graph->provinces[i].name,
					graph->provinces[road->dest].name,
					road->distance,
					state_to_text(road->state));
			}
			road = road->next;
		}
		i++;
	}
}

void procurar_rota(t_graph *graph)
{
	char src[100];
	char dest[100];
	int start;
	int target;
	int visited[MAX_PROVINCES] = {0};
	int prev[MAX_PROVINCES];
	int queue[MAX_PROVINCES];
	int front;
	int back;
	int i;

	printf("Digite a provincia de origem: ");
	discard_line();
	read_line(src, sizeof(src));
	printf("Digite a provincia de destino: ");
	read_line(dest, sizeof(dest));
	start = find_province(graph, src);
	target = find_province(graph, dest);
	if (start == -1 || target == -1)
	{
		printf("O nome de uma das provincias nao existe!\n");
		return;
	}
	i = 0;
	while (i < MAX_PROVINCES)
	{
		prev[i] = -1;
		i++;
	}
	front = 0;
	back = 0;
	visited[start] = 1;
	queue[back++] = start;
	while (front < back)
	{
		int current = queue[front++];
		t_edge *road = graph->provinces[current].roads;

		while (road)
		{
			if (!road->state && !visited[road->dest])
			{
				visited[road->dest] = 1;
				prev[road->dest] = current;
				queue[back++] = road->dest;
			}
			road = road->next;
		}
	}
	if (!visited[target])
	{
		t_edge *direct = find_road(graph, start, target);

		if (direct && direct->state)
		{
			printf("Existe uma rota direta entre essas provincias, mas esta bloqueada!\n");
			return;
		}
		printf("Nao existe rota ativa entre essas provincias!\n");
		return;
	}
	printf("Rota encontrada:\n");
	print_path(prev, start, target, graph);
}

void menor_caminho(t_graph *graph)
{
	char src[100];
	char dest[100];
	int start;
	int target;
	int dist[MAX_PROVINCES];
	int prev[MAX_PROVINCES];
	int visited[MAX_PROVINCES] = {0};
	int i;
	int step;

	printf("Digite a provincia de origem: ");
	discard_line();
	read_line(src, sizeof(src));
	printf("Digite a provincia de destino: ");
	read_line(dest, sizeof(dest));
	start = find_province(graph, src);
	target = find_province(graph, dest);
	if (start == -1 || target == -1)
	{
		printf("O nome de uma das provincias nao existe!\n");
		return;
	}
	i = 0;
	while (i < MAX_PROVINCES)
	{
		dist[i] = 2147483647;
		prev[i] = -1;
		i++;
	}
	dist[start] = 0;
	step = 0;
	while (step < graph->count)
	{
		int current = -1;
		int best = 2147483647;
		t_edge *road;

		i = 0;
		while (i < graph->count)
		{
			if (!visited[i] && dist[i] < best)
			{
				best = dist[i];
				current = i;
			}
			i++;
		}
		if (current == -1)
			break;
		visited[current] = 1;
		road = graph->provinces[current].roads;
		while (road)
		{
			if (!road->state && dist[current] != 2147483647 && dist[current] + road->distance < dist[road->dest])
			{
				dist[road->dest] = dist[current] + road->distance;
				prev[road->dest] = current;
			}
			road = road->next;
		}
		step++;
	}
	if (dist[target] == 2147483647)
	{
		printf("Nao existe caminho ativo entre essas provincias!\n");
		return;
	}
	printf("Menor caminho: %d km\n", dist[target]);
	print_path(prev, start, target, graph);
}
