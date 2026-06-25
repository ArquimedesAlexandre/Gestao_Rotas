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
    char username[50];
    char password[50];
    char role[20];

    fp = fopen("users.txt", "r");
    if (!fp)
        return ;

    fscanf(fp,
        "%49[^,],%49[^,],%19s",
        username,
        password,
        role);

    strcpy(admin->username, username);
    strcpy(admin->password, password);
    admin->role = ADMIN;

    fscanf(fp,
        "%49[^,],%49[^,],%19s",
        username,
        password,
        role);

    strcpy(user->username, username);
    strcpy(user->password, password);
    user->role = USER;

    fclose(fp);
}

void load_roads(t_graph *graph)
{
    FILE *fp;
    char line[100];

    char src[50];
    char dest[50];
    int distance;

    fp = fopen("roads.txt", "r");
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

void load_provinces(t_graph *graph)
{
    FILE *fp;
    char line[100];

    fp = fopen("provinces.txt", "r");
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

    printf("Username: ");
    scanf("%49s", username);

    printf("Password: ");
    scanf("%49s", password);

    if (strcmp(username, account.username) == 0
        && strcmp(password, account.password) == 0)
        return (1);
    return (0);
}


int main(void)
{
    t_user  admin;
    t_user  user;
    t_graph graph;
    int     tipo_login;

    load_datas(&admin, &user, &graph);

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