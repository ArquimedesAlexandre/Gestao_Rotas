#include "./includes/gestao.h"

static void menu_admin(void)
{
    printf("\n===== MENU ADMIN =====\n");
    printf("1 - Monitorizar Rede\n");
    printf("2 - Cadastrar Provincia\n");
    printf("3 - Criar ou atualizar ligacao rodoviaria\n");
    printf("4 - Bloquear rota\n");
    printf("5 - Guardar dados\n");
    printf("0 - Logout\n");
}

static void menu_user(void)
{
    printf("\n===== MENU USER =====\n");
    printf("1 - Listar provincias\n");
    printf("2 - Mostrar mapa\n");
    printf("3 - Procurar rota\n");
    printf("4 - Menor caminho\n");
    printf("0 - Logout\n");
}

int main(void)
{
    t_user admin;
    t_user user;
    t_graph graph;
    int tipo_login;

    load_datas(&admin, &user, &graph);
    printf("Dados carregados com sucesso\n");
    while (1)
    {
        printf("===== SISTEMA DE GESTAO DE ROTAS =====\n");
        printf("1 - Admin\n");
        printf("2 - User\n");
        printf("0 - Sair\n");
        scanf("%d", &tipo_login);
        if (tipo_login == 0)
            break;
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
    }
    return (0);
}
