#include "./includes/gestao.h"

void menu_admin(void)
{
    printf("\n===== MENU ADMIN =====\n");
    printf("1 - Adicionar estrada\n");
    printf("2 - Remover estrada\n");
    printf("3 - Atualizar distancia\n");
    printf("4 - Listar provincias\n");
    printf("5 - Mostrar grafo\n");
    printf("6 - Guardar dados\n");
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
    FILE *fp;
    char line[100];
    char role[20];

    fp = fopen("DataBase/Login.txt", "r");
    if (!fp)
    {
        printf("Erro ao abrir Login.txt\n");
        return;
    }

    if (fgets(line, sizeof(line), fp))
    {
        sscanf(line,
            "%49[^,],%49[^,],%19s",
            admin->username,
            admin->password,
            role);
        admin->role = ADMIN;
    }

    if (fgets(line, sizeof(line), fp))
    {
        sscanf(line,
            "%49[^,],%49[^,],%19s",
            user->username,
            user->password,
            role);
        user->role = USER;
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

void add_road(t_graph *graph,
              char *src,
              char *dest,
              int distance)
{
    t_edge *new;
    int source;
    int destination;

    source = find_province(graph, src);
    destination = find_province(graph, dest);

    if (source == -1 || destination == -1)
        return ;

    new = malloc(sizeof(t_edge));

    new->dest = destination;
    new->distance = distance;

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

    fp = fopen("DataBase/Roads.txt", "r");
    if (!fp)
        return ;

    while (fgets(line, sizeof(line), fp))
    {
        sscanf(line,
               "%49[^,],%49[^,],%d",
               src,
               dest,
               &distance);
        add_road(graph, src, dest, distance);
        add_road(graph, dest, src, distance);
    }
    fclose(fp);
}

void add_province(t_graph *graph, char *name)
{
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

    while (1)
    {
        printf("Username (0 para sair): ");
        scanf("%49s", username);

        if (strcmp(username, "0") == 0)
            return (0);

        printf("Password: ");
        scanf("%49s", password);

        if (strcmp(username, account.username) == 0
            && strcmp(password, account.password) == 0)
        {
            printf("Login efetuado com sucesso!\n");
            return (1);
        }

        printf("Credenciais invalidas! Tente novamente.\n\n");
    }
}


int main(void)
{
    t_user  admin;
    t_user  user;
    t_graph graph;
    int     tipo_login;

    load_datas(&admin, &user, &graph);
    printf("Dados carregados com sucesso");
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
                        printf("Adicionar estrada\n");
                        break;
                    case 2:
                        printf("Remover estrada\n");
                        break;
                    case 3:
                        printf("Atualizar distancia\n");
                        break;
                    case 4:
                        printf("Listar provincias\n");
                        break;
                    case 5:
                        printf("Mostrar grafo\n");
                        break;
                    case 6:
                        printf("Guardar dados\n");
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
                        printf("Listar provincias\n");
                        break;
                    case 2:
                        printf("Mostrar mapa\n");
                        break;
                    case 3:
                        printf("Procurar rota\n");
                        break;
                    case 4:
                        printf("Menor caminho\n");
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