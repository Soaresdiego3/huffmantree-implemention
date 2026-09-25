Huffman Tree em C 🌳

Implementação inicial de uma Huffman Tree em linguagem C, desenvolvida durante meus estudos de estruturas de dados, ponteiros, struct, union e alocação dinâmica de memória.

O projeto está sendo desenvolvido de forma gradual, acompanhando uma implementação apresentada em vídeo e escrevendo o código durante o estudo para compreender não apenas o algoritmo, mas também como os dados são organizados na memória.

Status: projeto em desenvolvimento. Nesta etapa, o foco está na construção e conexão dos elementos básicos da árvore. A compressão completa de arquivos ainda não foi implementada.

📌 Objetivo

O principal objetivo deste projeto é compreender, na prática, como uma árvore de Huffman pode ser representada e manipulada em C.

Durante esta etapa, os principais conceitos estudados são:

struct
union
typedef
enum
ponteiros
ponteiros para ponteiros
endereços de memória
alocação dinâmica com malloc
liberação de memória com free
assert
macros do pré-processador
árvores binárias
relacionamento entre nós pai e filhos
frequência de caracteres

🌳 Estrutura da árvore

A árvore possui dois tipos principais de elementos:

Folha (leaf)

Uma folha representa um caractere e sua frequência.

struct s_leaf
{
    int64 freq;
    int8 kind;
    tree *up;
    int8 c;
};
Nó (node)

Um nó interno representa a combinação de dois elementos da árvore.

struct s_node
{
    int64 freq;
    int8 kind;
    tree *up;
    tree *left;
    tree *right;
};

A estrutura pode ser representada inicialmente assim:

              Node
             /    \
          Leaf    Leaf
           A        B

Cada elemento possui um ponteiro up, que permite acessar seu nó pai.

Os nós internos também possuem:

left  → filho esquerdo
right → filho direito

🧩 union u_tree

O projeto utiliza uma union para permitir que uma estrutura tree represente tanto uma folha quanto um nó:

union u_tree
{
    struct s_node n;
    struct s_leaf l;
};

O campo kind informa qual tipo de elemento está sendo armazenado:

enum
{
    Leaf,
    Node
};

Conceitualmente:

                  tree
                    |
             ┌──────┴──────┐
             │             │
           Leaf           Node
             │             │
        caractere      left / right
        frequência       frequência

A union permite que os diferentes tipos compartilhem a mesma região de memória.

Por isso, é importante saber qual tipo está armazenado antes de acessar seus campos.

⚠️ Acesso correto à union

Uma das principais correções realizadas durante o desenvolvimento foi justamente o acesso aos campos da union.

Como tree pode representar um Leaf ou um Node, não é seguro simplesmente assumir que determinado membro deve ser utilizado.

Para resolver isso, foi criada a função:

static int64 tree_freq(tree *t)
{
    assert(t);

    if(t->n.kind == Leaf)
        return t->l.freq;

    if(t->n.kind == Node)
        return t->n.freq;

    assert(0);

    return 0;
}

Agora, o programa verifica o kind antes de acessar freq.

Assim:

                 tree
                   |
              verifica kind
                   |
           ┌───────┴───────┐
           ↓               ↓
         Leaf             Node
           ↓               ↓
       t->l.freq       t->n.freq

Isso também foi aplicado ao ponteiro up.

A função:

static void set_up(tree *child, tree *parent)

verifica se o filho é um Leaf ou um Node antes de armazenar o ponteiro para o pai.

Essa correção tornou o código mais explícito e seguro em relação à utilização da union.

🔗 Construção da árvore

Uma folha é criada com:

l1 = mkleaf((int8)'a');

Sua frequência pode então ser definida:

l1->freq = 4;

Outra folha é criada:

l2 = mkleaf((int8)'b');

Depois, as duas folhas são conectadas através de:

n = mknode($t l1, $t l2);

O resultado é:

             Node
          freq = 5
           /    \
          /      \
       'a'        'b'
       4           1

A frequência do nó é calculada pela soma das frequências dos filhos:

4 + 1 = 5

🧠 A parte mais difícil: tree **

Um dos conceitos mais importantes desta etapa foi compreender a função conn():

void conn(tree *parent, bool isleft, tree *child)

Dentro dela existe:

tree **connector;

A diferença entre:

tree *

e:

tree **

é fundamental.

tree *

Representa um ponteiro para uma tree.

tree * ───────► tree
tree **

Representa um ponteiro para uma variável que contém um ponteiro para uma tree.

tree ** ───────► tree * ───────► tree

Na função conn(), isso é utilizado para escolher entre o ponteiro left e o ponteiro right:

if(isleft)
{
    connector = (tree **)&parent->n.left;
}
else
{
    connector = (tree **)&parent->n.right;
}

Depois:

*connector = child;

faz com que o ponteiro correspondente passe a apontar para o filho.

Visualmente:

                  parent
                 /      \
              left      right
               |
               └──────────► child

Esse foi um dos pontos mais importantes do projeto porque envolve diretamente:

ponteiros;
endereço de memória;
operador &;
operador *;
estruturas;
árvores.
🔄 Ponteiro up

Além de conectar o pai aos filhos, o projeto também mantém a ligação inversa.

Quando um filho é conectado:

set_up(child, parent);

o filho passa a conhecer seu pai.

Assim, temos:

                 parent
                /      \
               ↓        ↓
            child     child
               ↑        ↑
               └── up ──┘

Dessa forma, a árvore possui relacionamentos nos dois sentidos:

parent
  ↓
child

child
  ↑
parent
🧹 Alocação dinâmica

Os elementos da árvore são criados dinamicamente utilizando malloc.

Por exemplo:

p = $el alloc(size);

O macro:

#define alloc(x) malloc($i (x))

é utilizado para simplificar a chamada.

Depois que os elementos não são mais necessários, a memória é liberada:

free(n);
free(l1);
free(l2);

Esse gerenciamento é importante porque os nós da árvore são criados em memória dinâmica.

🧪 Teste atual

O main() cria duas folhas:

'a' → frequência 4
'b' → frequência 1

Depois cria um nó que conecta as duas:

             (5)
            /   \
         'a'    'b'
          4      1

O programa utiliza show() para exibir as informações da árvore.

A saída permite verificar:

tipo do elemento;
caractere;
frequência;
pai;
filho esquerdo;
filho direito.

📚 O que estou aprendendo

O objetivo deste projeto não é apenas implementar o algoritmo de Huffman.

O projeto está sendo utilizado para entender como estruturas de dados podem ser representadas diretamente na memória utilizando C.

A árvore de Huffman é o objetivo final, mas o caminho envolve conceitos fundamentais da linguagem:

Memória
   ↓
Ponteiros
   ↓
Structs
   ↓
Union
   ↓
Alocação dinâmica
   ↓
Árvores
   ↓
Recursão
   ↓
Bits
   ↓
Compressão

📌 Status do projeto

Atualmente, o projeto possui a estrutura básica necessária para criar e conectar folhas e nós de uma árvore.

Também foi feita uma correção importante nos acessos à union, garantindo que o programa identifique se determinado elemento é um Leaf ou um Node antes de acessar seus campos específicos.

A implementação ainda não realiza a compressão de arquivos.

O foco atual é entender profundamente os fundamentos antes de avançar para as próximas etapas do algoritmo de Huffman.

📖 Referência

Projeto desenvolvido como parte dos estudos de linguagem C e estruturas de dados, acompanhando uma implementação de Huffman Tree apresentada em vídeo.

A implementação está sendo construída de forma incremental, com foco no aprendizado dos conceitos envolvidos.
