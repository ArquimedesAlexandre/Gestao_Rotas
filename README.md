# Gestao_Rotas
Sistema de gestao de Rotas em c para fins academicos
🔑 1. Tela Inicial e Fluxo de Autenticação

Quando o programa é aberto, ele funciona como uma barreira de segurança. O utilizador não vê as opções do mapa de imediato; precisa primeiro escolher o seu nível de acesso:

Opção 1: Entrar como Passageiro —Direciona para o Menu de Consulta (Acesso Livre).
Opção 2: Entrar como Administrador —O sistema pede o Nome de Utilizador e a Palavra-passe. Se estiverem corretos, liberta o Painel de Gestão. Caso contrário, exibe "Acesso Negado" e volta à tela inicial.
Opção 3: Desligar o Sistema — Encerra o programa em segurança.

🧳 2. Menu do Passageiro (Modo Visualização)

Este menu foi desenhado para o cliente final (quem vai viajar). Ele é totalmente seguro porque não permite alterar os dados do mapa, apenas ler.

Funcionalidade 1 — Consultar Cidades Atendidas: Lista apenas os nomes de todas as províncias e localidades que fazem parte da rede de transportes naquele momento.

Funcionalidade 2 — Ver Mapa Rodoviário Completo: Mostra todas as estradas ativas e as respetivas distâncias em quilómetros (ex: Luanda <---> Benguela | 250 km).

Funcionalidade 3 — Planear Rota Mais Curta: O passageiro digita de onde sai e para onde vai. O sistema ativa internamente o algoritmo, mostra as cidades onde ele terá de passar e calcula o total de quilómetros que vai percorrer.

Funcionalidade 4 — Voltar: Faz o log out deste painel e regressa à Tela Inicial.

🛠️ 3. Menu do Administrador (Modo Gestão)

Este menu é exclusivo para os gestores da infraestrutura rodoviária de Angola. Aqui residem as funcionalidades que modificam o grafo e alteram o ficheiro de dados:

Funcionalidade 1 — Monitorizar Rede: Uma visão geral que combina a lista de cidades e todas as conexões atuais para o gestor saber o estado do mapa.

Funcionalidade 2 — Cadastrar Nova Província/Cidade: Adiciona uma nova localização geográfica ao sistema. Ela entra no mapa inicialmente isolada, até que se crie uma estrada para ela.

Funcionalidade 3 — Criar ou Atualizar Ligação Rodoviária: Liga duas cidades definindo a distância entre elas. Se a estrada já existia mas a distância mudou (por exemplo, devido a uma nova variante ou calçadas), o sistema atualiza o valor antigo.

Funcionalidade 4 — Bloquear Rota (Remover Estrada): Caso uma estrada sofra um corte por desabamento, obras ou chuva intensa, o administrador desativa essa ligação. O sistema remove essa aresta, forçando as rotas dos passageiros a calcularem desvios automáticos por outras províncias.

Funcionalidade 5 — Guardar Alterações no Ficheiro: Grava fisicamente no disco todas as alterações feitas no menu. Se o computador for desligado, os dados novos não se perdem.

Funcionalidade 6 — Terminar Sessão: Bloqueia novamente as ferramentas de gestão e volta para a Tela Inicial.
