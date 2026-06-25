#include "gestao.h"

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


void load_datas(t_user *admin, t_user *user)
{
    strcpy(admin->username, "admin");
    strcpy(admin->password, "1234");
    admin->role = ADMIN;

    strcpy(user->username, "user");
    strcpy(user->password, "1234");
    user->role = USER;
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
    t_user admin;
    t_user user;
    int tipo_login;

    load_datas(&admin, &user);

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