#include "./includes/gestao.h"

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

    printf("\n===== CONTEÚDO DO FICHEIRO =====\n");
    if (fgets(line, sizeof(line), fp))
    {
        printf("Linha 1: %s", line);

        sscanf(line,
            "%49[^,],%49[^,],%19s",
            admin->username,
            admin->password,
            role);
    }

    if (fgets(line, sizeof(line), fp))
    {
        printf("Linha 2: %s", line);
        sscanf(line,
            "%49[^,],%49[^,],%19s",
            user->username,
            user->password,
            role);
    }

    fclose(fp);

    printf("\n===== DADOS CARREGADOS =====\n");
    printf("Admin -> username='%s' password='%s'\n",
        admin->username, admin->password);

    printf("User  -> username='%s' password='%s'\n",
        user->username, user->password);
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


int main(void)
{
    t_user  admin;
    t_user  user;
    t_graph graph;
    int     tipo_login;
    int c;

    load_datas(&admin, &user, &graph);
    printf("Dados carregados com sucesso]\n");
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
                        criar_ou_atualizar_ligacao(&graph);
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