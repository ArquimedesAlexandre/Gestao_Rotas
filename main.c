#include "./includes/gestao.h"

static int state_from_text(const char *text)
{
    if (!text)
        return (0);
    if (strcmp(text, "1") == 0 || strcmp(text, "Blocked") == 0 || strcmp(text, "blocked") == 0
        || strcmp(text, "Bloqueada") == 0 || strcmp(text, "bloqueada") == 0)
        return (1);
    return (0);
}

static const char *state_to_text(int state)
{
    if (state)
        return ("Blocked");
    return ("Active");
}

static void print_path(int prev[MAX_PROVINCES], int source, int destination, t_graph *graph)
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

void guardar_dados(t_graph *graph)
{
    int ok;

    ok = save_provinces_file(graph);
    ok = save_roads_file(graph) && ok;
    ok = write_report_file(graph) && ok;
    if (ok)
        printf("Dados guardados com sucesso e relatorio gerado em DataBase/Relatorio.txt\n");
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
    scanf("%99s", src);
    printf("Digite a provincia de destino: ");
    scanf("%99s", dest);
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
    scanf("%99s", src);
    printf("Digite a provincia de destino: ");
    scanf("%99s", dest);
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

void menu_admin(void)
{
    printf("\n===== MENU ADMIN =====\n");
    printf("1 - Monitorizar Rede\n");
    printf("2 - Cadastrar Província\n");
    printf("3 - Criar ou atualizar ligação rodoviaria\n");
    printf("4 - Bloquear rota\n");
    printf("5 - Guardar dados\n");
    printf("0 - Logout\n");
}

void menu_user(void)
{
    printf("\n===== MENU USER =====\n");
    printf("1 - Listar provincias\n");
    printf("2 - Mostrar mapa\n");
    printf("3 - Procurar rota\n");
    printf("4 - Menor caminho\n");
    printf("0 - Logout\n");
}


void get_users(t_user *admin, t_user *user)
{
    FILE    *fp;
    char    line[200];
    char    role[20];
    char    cwd[256];

    getcwd(cwd, sizeof(cwd));
    printf("Diretório atual: %s\n", cwd);

    fp = fopen("DataBase/Login.txt", "r");
    if (!fp)
    {
        perror("Erro ao abrir Login.txt");
        return;
    }

    if (fgets(line, sizeof(line), fp))
    {

        sscanf(line,
            "%49[^,],%49[^,],%19s",
            admin->username,
            admin->password,
            role);
    }

    if (fgets(line, sizeof(line), fp))
    {
        sscanf(line,
            "%49[^,],%49[^,],%19s",
            user->username,
            user->password,
            role);
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

void add_road(t_graph *graph,
              char *src,
              char *dest,
              int distance,
              int state)
{
    t_edge *new;
    int source;
    int destination;
    t_edge *existing;

    source = find_province(graph, src);
    destination = find_province(graph, dest);

    if (source == -1 || destination == -1){
        printf("Erro: uma das provincias nao existe!\n");
        return ;
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
        return ;

    while (fgets(line, sizeof(line), fp))
    {
        if (sscanf(line,
               "%49[^,],%49[^,],%d, %49[^\n]",
               src,
               dest,
               &distance,
               state_text) != 4)
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
    strcpy(graph->provinces[graph->count].name, name);
    graph->provinces[graph->count].roads = NULL;
    graph->count++;
}

void load_provinces(t_graph *graph)
{
    FILE *fp;
    char line[100];

    fp = fopen("DataBase/Provinces.txt", "r");
    if (!fp)
        return ;

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
    int c;

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

void    monitorar_rede(t_graph *graph){
    t_graph *aux = graph;

    for (int i = 0; i < aux->count; i++)
    {
        printf("Provincia: %s\n", aux->provinces[i].name);
        t_edge *road = aux->provinces[i].roads;
        while (road)
        {
            printf("  -> %s (distancia: %d) (estado: %s)\n", aux->provinces[road->dest].name, road->distance, state_to_text(road->state));
            road = road->next;
        }
    }
}

void    cadastrar_provincia(t_graph *graph){
    char name[100];

    printf("Digite o nome da nova provincia: ");
    scanf("%99s", name);
    add_province(graph, name);
}

void    criar_ligacao(t_graph *graph){
    char src[100];
    char dest[100];
    int distance;
    int state;

    printf("Digite o nome da provincia de origem: ");
    scanf("%99s", src);
    printf("Digite o nome da provincia de destino: ");
    scanf("%99s", dest);
    printf("Digite a distancia entre as provincias: ");
    scanf("%d", &distance);
    printf("Digite o estado da rota (0 - normal, 1 - bloqueada): ");
    scanf("%d", &state);

    add_road(graph, src, dest, distance, state);
    add_road(graph, dest, src, distance, state);
}

void    bloquear_rota(t_graph *graph){
    char src[100];
    char dest[100];
    int pos_src;
    int pos_dest;
    t_edge *road;
    t_edge *reverse_road;

    printf("digite o nome da provincia de origem: ");
    scanf("%99s", src);
    printf("digite o nome da provincia de destino: ");
    scanf("%99s", dest);

    pos_src = find_province(graph, src);
    pos_dest = find_province(graph, dest);

    if (pos_src == -1 || pos_dest == -1){
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

int main(void)
{
    t_user  admin;
    t_user  user;
    t_graph graph;
    int     tipo_login;
    int c;

    load_datas(&admin, &user, &graph);
    printf("Dados carregados com sucesso\n");
    printf("===== SISTEMA DE GESTAO DE ROTAS =====\n");
    printf("1 - Admin\n");
    printf("2 - User\n");
    printf("0 - Sair\n");

    scanf("%d", &tipo_login);
    if (tipo_login == 1)
    {
        if (login(admin))
        {
           int opcao;
            do
            {
                menu_admin();
                printf("Opcao: ");
                scanf("%d", &opcao);
                switch (opcao)
                {
                    case 1:
                        monitorar_rede(&graph);
                        break;
                    case 2:
                        cadastrar_provincia(&graph);
                        break;
                    case 3:
                        criar_ligacao(&graph);
                        break;
                    case 4:
                        bloquear_rota(&graph);
                        break;
                    case 5:
                        guardar_dados(&graph);
                        break;
                    case 0:
                        printf("Logout efetuado.\n");
                        break;
                    default:
                        printf("Opcao invalida.\n");
                }

            } while (opcao != 0);
        }
        else
            printf("Credenciais invalidas!\n");
    }
   else if (tipo_login == 2)
    {
        if (login(user))
        {
            int opcao;
            do
            {
                menu_user();
                printf("Opcao: ");
                scanf("%d", &opcao);
                switch (opcao)
                {
                    case 1:
                        listar_provincias(&graph);
                        break;
                    case 2:
                        mostrar_mapa(&graph);
                        break;
                    case 3:
                        procurar_rota(&graph);
                        break;
                    case 4:
                        menor_caminho(&graph);
                        break;
                    case 0:
                        printf("Logout efetuado.\n");
                        break;
                    default:
                        printf("Opcao invalida.\n");
                }

            } while (opcao != 0);
        }
        else
        printf("Credenciais invalidas!\n");
    }
    return (0);
}