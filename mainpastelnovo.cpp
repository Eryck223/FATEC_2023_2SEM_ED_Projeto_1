#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

// Estrutura para representar um pedido de pastel
struct Pedido {
    string tipo;
    string recheio1;
    string recheio2;
    string recheio3;
    vector<string> bebidas; // Usamos um vetor para representar as bebidas
};

// Estrutura para representar a lista de pedidos em produção
struct PedidoProducao {
    int senha;
    vector<Pedido> pedidos;
};

// Estrutura para representar o estoque de matérias-primas e bebidas
struct Estoque {
    int queijo;   // Em gramas
    int presunto; // Em gramas
    int frango;   // Em gramas
    int cocaCola; // Quantidade de Coca-Cola em unidades
    int pepsi;    // Quantidade de Pepsi em unidades
};

// Função para exibir todos os pedidos em produção
void exibirPedidosProducao(const vector<PedidoProducao>& pedidosEmProducao) {
    cout << "\nPedidos em Produção:\n";
    for (size_t i = 0; i < pedidosEmProducao.size(); ++i) {
        const PedidoProducao& pedidoProducao = pedidosEmProducao[i];

        cout << "Senha: " << pedidoProducao.senha << "\n";

        for (size_t j = 0; j < pedidoProducao.pedidos.size(); ++j) {
            const Pedido& pedido = pedidoProducao.pedidos[j];

            cout << "  Pedido " << j + 1 << " - Tipo: " << pedido.tipo;

            if (pedido.tipo == "Pizza") {
                cout << ", Queijo: " << pedido.recheio1
                     << ", Presunto: " << pedido.recheio2
                     << ", Tomate: " << pedido.recheio3;
            } else if (pedido.tipo == "Queijo" || pedido.tipo == "Frango") {
                cout << ", Recheio: " << pedido.recheio1;
            }

            cout << ", Bebidas: ";
            for (size_t k = 0; k < pedido.bebidas.size(); ++k) {
                cout << pedido.bebidas[k] << " ";
            }

            cout << endl;
        }
    }
}

// Função para entregar um pedido e removê-lo da lista
void entregarPedido(vector<PedidoProducao>& pedidosEmProducao, vector<PedidoProducao>& pedidosConcluidos) {
    int senhaEntrega;
    cout << "Digite a senha do pedido a ser entregue: ";
    cin >> senhaEntrega;

    for (vector<PedidoProducao>::iterator it = pedidosEmProducao.begin(); it != pedidosEmProducao.end(); ++it) {
        if (it->senha == senhaEntrega) {
            // Mover o pedido concluído para o vetor de pedidos concluídos
            pedidosConcluidos.push_back(*it);

            cout << "Pedido entregue com sucesso!\n";
            pedidosEmProducao.erase(it);
            return;
        }
    }

    cout << "Pedido não encontrado.\n";
}

// Função para cadastrar um novo pedido
PedidoProducao cadastrarPedido(int& proximaSenha, Estoque& estoque) {
    PedidoProducao novoPedidoProducao;
    novoPedidoProducao.senha = proximaSenha++;

    int quantidadePasteis;
    cout << "Digite a quantidade de pasteis para o pedido: ";

    // Adicionamos verificações e limpeza do buffer
    while (!(cin >> quantidadePasteis) || quantidadePasteis <= 0) {
        cin.clear();                                                        // Limpa o estado de erro
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descarta a entrada inválida
        cout << "Quantidade inválida. Digite novamente: ";
    }

    for (int i = 0; i < quantidadePasteis; ++i) {
        Pedido novoPedido;

        cout << "Digite o tipo de pastel (Pizza, Queijo, Frango, Vento): ";
        while (!(cin >> novoPedido.tipo)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Tipo inválido. Digite novamente: ";
        }

      

        // Lógica para adicionar bebidas ao pedido
        int quantidadeBebidas;
        cout << "Digite a quantidade de bebidas para o pedido: ";
        while (!(cin >> quantidadeBebidas) || quantidadeBebidas < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Quantidade inválida. Digite novamente: ";
        }

        for (int j = 0; j < quantidadeBebidas; ++j) {
            string bebida;
            cout << "Digite a bebida (Coca-Cola, Pepsi): ";
            while (!(cin >> bebida) || (bebida != "Coca-Cola" && bebida != "Pepsi")) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Bebida inválida. Digite novamente: ";
            }

            // Lógica para atualizar o estoque de bebidas
            if (bebida == "Coca-Cola") {
                if (estoque.cocaCola > 0) {
                    estoque.cocaCola -= 1;
                    novoPedido.bebidas.push_back(bebida);
                } else {
                    cout << "Estoque de Coca-Cola esgotado.\n";
                }
            } else if (bebida == "Pepsi") {
                if (estoque.pepsi > 0) {
                    estoque.pepsi -= 1;
                    novoPedido.bebidas.push_back(bebida);
                } else {
                    cout << "Estoque de Pepsi esgotado.\n";
                }
            }
        }

        novoPedidoProducao.pedidos.push_back(novoPedido);
    }

    // Atualizar o estoque com base nos pedidos
    for (size_t i = 0; i < novoPedidoProducao.pedidos.size(); ++i) {
        const Pedido& pedido = novoPedidoProducao.pedidos[i];

        if (pedido.tipo == "Pizza") {
            estoque.queijo -= 30;
            estoque.presunto -= 30;
        } else if (pedido.tipo == "Queijo") {
            estoque.queijo -= 50;
        } else if (pedido.tipo == "Frango") {
            estoque.frango -= 50;
        }
    }

    return novoPedidoProducao;
}

// Função para gerar e salvar o relatório de vendas em um arquivo de texto
void gerarRelatorioVendasArquivo(const vector<PedidoProducao>& pedidosConcluidos, const Estoque& estoque) {
    ofstream arquivo("relatorio_vendas.txt");

    if (arquivo.is_open()) {
        arquivo << "\nRELATÓRIO DE VENDAS\n";
        arquivo << "====================\n";

        // Bebidas
        arquivo << "BEBIDAS\n";
        arquivo << "=======\n";
        // Lógica para contar a quantidade de cada bebida vendida
        // ...

        // Pastéis
        arquivo << "\nPASTÉIS\n";
        arquivo << "=======\n";
        // Lógica para contar a quantidade de cada tipo de pastel vendido
        // ...

        // Estoque restante
        arquivo << "\nESTOQUE RESTANTE\n";
        arquivo << "=================\n";
        arquivo << "Coca-Cola: " << estoque.cocaCola << " unidades\n";
        arquivo << "Pepsi: " << estoque.pepsi << " unidades\n";
        arquivo << "Queijo: " << estoque.queijo << " gramas\n";
        arquivo << "Presunto: " << estoque.presunto << " gramas\n";
        arquivo << "Frango: " << estoque.frango << " gramas\n";

        cout << "Relatório de vendas salvo em 'relatorio_vendas.txt'.\n";

        arquivo.close();
    } else {
        cout << "Não foi possível abrir o arquivo para salvar o relatório de vendas.\n";
    }
}

int main() {
    Estoque estoque = {50000, 20000, 10000, 20, 15};
    vector<PedidoProducao> pedidosEmProducao;
    vector<PedidoProducao> pedidosConcluidos;
    int proximaSenha = 1;

    int opcao;
    do {
        cout << "\n--- Menu ---\n";
        cout << "1. Cadastrar Pedido\n";
        cout << "2. Editar Pedido\n";
        cout << "3. Exibir Pedidos\n";
        cout << "4. Entregar Pedido\n";
        cout << "5. Gerar Relatório de Vendas\n";
        cout << "6. Sair\n";
        cout << "Escolha uma opção: ";
        while (!(cin >> opcao) || opcao < 1 || opcao > 6) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Opção inválida. Digite novamente: ";
        }

        switch (opcao) {
        case 1:
            pedidosEmProducao.push_back(cadastrarPedido(proximaSenha, estoque));
            break;
        case 3:
            exibirPedidosProducao(pedidosEmProducao);
            break;
        case 4:
            entregarPedido(pedidosEmProducao, pedidosConcluidos);
            break;
        case 5:
            gerarRelatorioVendasArquivo(pedidosConcluidos, estoque);
            break;
        case 6:
            cout << "Saindo do programa. Até logo!\n";
            break;
        default:
            cout << "Opção inválida. Tente novamente.\n";
        }
    } while (opcao != 6);

    return 0;
}

