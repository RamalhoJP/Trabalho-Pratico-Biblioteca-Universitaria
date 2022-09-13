#include <fstream>
#include <iostream>
#include <time.h>
using namespace std;

// Struct de livros
struct livros {
  char nome[100];
  int dia_dev;
  int mes_dev;
  int ano_dev;
};

// Struct de clientes cadastrados
struct clientes {
  char nome[100];
  int cpf;
  int matricula;
  char endereco[100];
  char telefone[100];
  char email[100];
  bool situacao = true;
  int tam_livro = 3;
  livros *livro = new livros[tam_livro];
  int pos_livro = 0;
};

// Realocamento de clientes no vetor
clientes *realloc_usuario(clientes *usuario, int tamanho) {
  clientes *novo_usuario = new clientes[tamanho + 3];
  for (int i = 0; i < tamanho; i++) {
    novo_usuario[i] = usuario[i];
  }
  delete[] usuario;
  return novo_usuario;
}

// Realocamento de livros no vetor
livros *realloc_livro(livros *livro, int tamanho, int quant) {
  livros *novo_livro = new livros[tamanho + quant];
  if (quant > 0) {
    for (int i = 0; i < tamanho; i++) {
      novo_livro[i] = livro[i];
    }
  } else {
    for (int i = 0; i < tamanho + quant; i++) {
      novo_livro[i] = livro[i];
    }
  }
  delete[] livro;
  return novo_livro;
}

// Funcao para checar se ha cpfs repetidos (Usado no cadastro)
bool verifica_cpf(clientes *usuario, int pos_usuario) {
  for (int i = 0; i < pos_usuario; i++) {
    if (usuario[i].cpf == usuario[pos_usuario].cpf)
      return false;
  }
  return true;
}

// Busca binaria no vetor de clientes
int buscarmat(clientes *usuario, int matricula, int qtd_us) {
  int pos = -1, pos_inicial = 0, pos_final = qtd_us - 1, meio = 0;
  while (pos_inicial <= pos_final) {
    meio = (pos_inicial + pos_final) / 2;
    if (matricula == usuario[meio].matricula) {
      pos = meio;
      pos_inicial = pos_final + 1;
    } else {
      if (matricula > usuario[meio].matricula) {
        pos_inicial = meio + 1;
      } else {
        pos_final = meio - 1;
      }
    }
  }
  return pos;
}

// Funcao que muda a posicao do vetor
void swap(clientes *a, clientes *b) {
  clientes t = *a;
  *a = *b;
  *b = t;
}

// Divide o vetor usando o pivo anterior
int partition(clientes *arr, int low, int high) {
  int pivot = arr[high].matricula; // pivot
  int i = (low - 1);

  for (int j = low; j <= high - 1; j++) {
    // Se o elemento atual for menor que o pivo, incrementa o menor elemento
    // Muda os elementos do vetor
    if (arr[j].matricula <= pivot) {
      i++;
      swap(&arr[i], &arr[j]);
    }
  }
  swap(&arr[i + 1], &arr[high]);
  return (i + 1);
}

// Quicksort principal
void quickSort(clientes *arr, int low, int high) {
  if (low < high) {
    // Divide o array
    int pivot = partition(arr, low, high);

    // Ordena eles separadamente
    quickSort(arr, low, pivot - 1);
    quickSort(arr, pivot + 1, high);
  }
}

// Divide o vetor usando o pivo anterior, para ordem alfabetica
int partition_alfabeta(clientes *arr, int low, int high) {
  char *pivot = arr[high].nome; // pivot
  int i = (low - 1);
  int k = 0;

  for (int j = low; j <= high - 1; j++) {
    if (arr[j].nome[k] < pivot[k]) {
      i++;
      swap(&arr[i], &arr[j]);
    } else if (arr[j].nome[k] == pivot[k]) {
      while (arr[j].nome[k] == pivot[k]) {
        k++;
        if (arr[j].nome[k] < pivot[k]) {
          i++;
          swap(&arr[i], &arr[j]);
        }
      }
      k = 0;
    }
  }

  swap(&arr[i + 1], &arr[high]);
  return (i + 1);
}

// Quicksort pra ordenar em ordem alfabetica
void quickSort_alfabeta(clientes *arr, int low, int high) {
  if (low < high) {
    int pivot = partition_alfabeta(arr, low, high);

    quickSort_alfabeta(arr, low, pivot - 1);
    quickSort_alfabeta(arr, pivot + 1, high);
  }
}

// Funcao para dar display no menu principal
void menuprinci() {
  cout << "------------------------------" << endl;
  cout << "   Biblioteca Universitaria " << endl;
  cout << "------------------------------" << endl;
  cout << "Menu Principal:" << endl;
  cout << "Digite(1) Para: Inserir um usuario " << endl;
  cout << "Digite(2) Para: Consultar livros emprestados/data de devolucão"
       << endl;
  cout << "Digite(3) Para: Listar usuários ativos" << endl;
  cout << "Digite(4) Para: Excluir usuário" << endl;
  cout << "Digite(5) Para: Efetuar empréstimo " << endl;
  cout << "Digite(6) Para: Efetuar devolução" << endl;
  cout << "Digite(7) Para: Exportar para arquivo binário" << endl;
  cout << "Digite(8) Para: Encerrar operação " << endl << endl;
  cout << "Selecionar: ";
}

// Clear no sistema
void limpatela() {
#if defined _WIN32
  system("cls");
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__) ||    \
    defined(__APPLE__)
  system("clear");
#endif
}

int main() {
  // Declarando variaveis
  int helper = 0;
  int opcao = 0;
  int tam_vetor = 3;
  int tam_usuario = 3;
  int qtd_usuario = 0;
  int pos_usuario = 0;
  int sent2 = 0, sent5 = 0;
  int aux_matricula;
  int aux_livro = 0;
  clientes *usuario = new clientes[tam_vetor];
  struct tm *p;
  time_t seconds;
  ofstream arquivo("recibo.dat");

  // Pegando a data do sistema
  time(&seconds);
  p = localtime(&seconds);

  // Iniciacao do menu
  menuprinci();
  while (opcao != 8) {
    cin >> opcao;

    // Primeira opcao
    if (opcao == 1) {
      limpatela();

      // Verificando se o precisa realocar
      if (pos_usuario == tam_vetor - 1) {
        usuario = realloc_usuario(usuario, tam_usuario);
        tam_usuario += 3;
      }

      // Inserindo os dados do usuario em cadastro
      cout << "Insira seus dados: " << endl;
      cin.ignore();
      cout << "Nome do usuário:" << endl;
      cin.getline(usuario[pos_usuario].nome, 100);
      cout << "CPF:" << endl;
      cin >> usuario[pos_usuario].cpf;
      cout << "Número de matrícula:" << endl;
      cin >> usuario[pos_usuario].matricula;
      cin.ignore();
      cout << "Endereço:" << endl;
      cin.getline(usuario[pos_usuario].endereco, 100);
      cout << "Telefone:" << endl;
      cin.getline(usuario[pos_usuario].telefone, 100);
      cout << "E-mail:" << endl;
      cin.getline(usuario[pos_usuario].email, 100);

      // Verificando se o cpf informado já está em uso
      if (verifica_cpf(usuario, pos_usuario)) {
        pos_usuario++;
        qtd_usuario++;
        cout << "Cadastro realizado com sucesso!" << endl;
      } else {
        cout << "Não foi possível realizar o cadastro, CPF em uso." << endl;
      }

      // Ordena o vetor de clientes
      quickSort(usuario, 0, qtd_usuario - 1);
      cin.get();
    }

    // Segunda opcao
    if (opcao == 2) {
      limpatela();

      // Informando matricula para consulta e busca de matricula
      cout << "Número da matrícula:" << endl;
      cin >> aux_matricula;
      helper = buscarmat(usuario, aux_matricula, qtd_usuario);

      cout << "Livros a serem devolvidos:" << endl;
      if (helper != -1) {
        for (int i = 0; i < usuario[helper].pos_livro; i++) {
          sent2++;
          cout << "\"" << usuario[helper].livro[i].nome
               << "\" - Devolução prevista em: "
               << usuario[helper].livro[i].dia_dev << "/"
               << usuario[helper].livro[i].mes_dev << "/"
               << usuario[helper].livro[i].ano_dev << endl;
        }
        cout << "Saldo de livros: " << usuario[helper].pos_livro << endl;
      } else {
        cout << "Matrícula não cadastrada!" << endl;
      }
      if (sent2 == 0) {
        cout << "Não há livros a serem devolvidos!";
      }

      helper = 0;
      sent2 = 0;
      cin.ignore();
      cin.get();
    }

    // Terceira opcao
    if (opcao == 3) {
      limpatela();

      // Ordena em ordem alfabetica para listagem
      quickSort_alfabeta(usuario, 0, qtd_usuario - 1);

      // Listagem
      for (int i = 0; i < qtd_usuario; i++) {
        if (usuario[i].situacao) {
          cout << usuario[i].nome << " ";
          cout << usuario[i].matricula << " ";
          cout << usuario[i].email << endl;
        }
      }

      // Ordena em ordem de matricula novamente
      quickSort(usuario, 0, qtd_usuario - 1);
      cin.ignore();
      cin.get();
    }

    // Quarta opcao
    if (opcao == 4) {
      limpatela();

      // Informando matricula para consulta e busca de matricula
      cout << "Número da matrícula:" << endl;
      cin >> aux_matricula;
      helper = buscarmat(usuario, aux_matricula, qtd_usuario);

      // Testando exceções
      if (helper == -1) {
        cout << "Usuario inexistente" << endl;
      } else {
        cout << "Nome do usuario: " << usuario[helper].nome << endl;
        cout << "Confirma a exclusão? (1) - Sim / (2) - Não" << endl;
        cin >> opcao;
        if (opcao == 1) {
          // Situacao recebe falso e o usuario fica inativo
          usuario[helper].situacao = false;
          cout << "Nome do usuario: " << usuario[helper].nome << endl;
          cout << "Situação: inativo" << endl;
        } else {
          cout << "Nome do usuario: " << usuario[helper].nome << endl;
          cout << "Situação: ativo" << endl;
        }
      }

      cin.ignore();
      cin.get();
    }

    // Quinta opcao
    if (opcao == 5) {
      limpatela();

      // Informando matricula para consulta e busca de matricula
      cout << "Número de Matrícula: " << endl;
      cin >> aux_matricula;
      helper = buscarmat(usuario, aux_matricula, qtd_usuario);

      // Verificando atraso
      if(usuario[helper].situacao){
        for (int i = 0; i < usuario[helper].pos_livro; i++) {
          if (usuario[helper].livro[i].ano_dev < p[0].tm_year + 1900) {
            i = usuario[helper].pos_livro;
            sent5++;
          } else if (usuario[helper].livro[i].mes_dev < p[0].tm_mon + 1) {
            i = usuario[helper].pos_livro;
            sent5++;
          } else if (usuario[helper].livro[i].dia_dev < p[0].tm_mday &&
                     usuario[helper].livro[i].mes_dev == p[0].tm_mon + 1) {
            i = usuario[helper].pos_livro;
            sent5++;
          }
        }
  
        // Caso não esteja atrasado acontece o emprestimo
        if (sent5 == 0) {
          // Testando se precisa realocar
          if (usuario[helper].pos_livro == usuario[helper].tam_livro - 1) {
            usuario[helper].livro = realloc_livro(usuario[helper].livro,
                                                  usuario[helper].tam_livro, 3);
            usuario[helper].tam_livro += 3;
          }
  
          // Inserindo dados
          cout << "Nome do livro a ser cadastrado:" << endl;
          cin.ignore();
          cin.getline(usuario[helper].livro[usuario[helper].pos_livro].nome, 100);
          cout << "Data de devolução:(dd mm aaaa)" << endl;
          cin >> usuario[helper].livro[usuario[helper].pos_livro].dia_dev;
          cin >> usuario[helper].livro[usuario[helper].pos_livro].mes_dev;
          cin >> usuario[helper].livro[usuario[helper].pos_livro].ano_dev;
          cout << "Empréstimo realizado com sucesso!" << endl;
  
          usuario[helper].pos_livro++;
        } else {
          cout << "Não foi possível realizar o empréstimo, há um livro atrasado!"
               << endl;
        }
      }else{
        cout << "Operacao nao realizada! O usuario está inativo.";
      }

      cin.ignore();
      cin.get();
      sent5 = 0;
      helper = 0;
    }

    // Sexta opcao
    if (opcao == 6) {
      limpatela();

      // Informando matricula para consulta e busca de matricula
      cout << "Número de Matrícula: " << endl;
      cin >> aux_matricula;
      helper = buscarmat(usuario, aux_matricula, qtd_usuario);

      // Listagem dos livros para escolha do usuario
      cout << "Escolha um livro para devolver:" << endl;
      if (usuario[helper].pos_livro == 0) {
        cout << "Não há livros.";
      } else {
        for (int i = 0; i < usuario[helper].pos_livro; i++) {
          cout << "(" << i << ") " << usuario[helper].livro[i].nome << endl;
        }
      }

      cin >> aux_livro;

      // Realizando exclusao, apenas se houver livro naquela posicao
      if (aux_livro < usuario[helper].pos_livro && aux_livro >= 0) {
        for (int i = aux_livro; i < usuario[helper].pos_livro - 1; i++) {
          usuario[helper].livro[i] = usuario[helper].livro[i + 1];
        }
        usuario[helper].pos_livro--;

        cout << "Livro devolvido com sucesso !";
      } else {
        cout << "Livro invalido!";
      }

      cin.ignore();
      cin.get();
    }

    // Setima opcao
    if (opcao == 7) {
      limpatela();
      if (qtd_usuario > 0){
         for(int i=0;i<qtd_usuario;i++){
			 arquivo.write((const char *)(&usuario[i]), sizeof(clientes));
			 arquivo.write((const char *)(usuario[i].livro), sizeof(livros));
		 }
		cout << "Arquivo exportado com sucesso";
      } else {
        cout << "Não há usuários cadastrados";
      }
      cin.ignore();
      cin.get();
    }

    // Casos defaults
    if (opcao > 8 || opcao < 1) {
      cout << "Opcao invalida!!!" << endl;
    }

    // Limpando e mostrando o menu novamente
    limpatela();
    menuprinci();
  }
  for (int i = 0; i < qtd_usuario; i++) {
    delete[] usuario[i].livro;
  }
  delete[] usuario;
  arquivo.close();
}