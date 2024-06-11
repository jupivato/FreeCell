//feito no windows
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

typedef struct no
{
    int carta;
    char naipe;
    struct no* ant;
    struct no* prox;
} tCarta; //cartas do baralho

//cria os montinhos de mesa, naipe e temporario
//os montinhos sao vetores de ponteiro
tCarta*primMesa[8];//aponta sempre para o PRIMEIRO da pilha = ultimo que entrou
tCarta*primNaipe[4];//aponta sempre para o PRIMEIRO da pilha = ultimo que entrou
tCarta*temp[4];//so aponta para uma carta, nao recebe pilha

tCarta*aux=NULL;//pilha auxiliar de transferencia mesa-mesa
tCarta*primMonte=NULL;//marca a primeira carta do baralho ordenado
tCarta*embaralha=NULL;//marca a primeira carta do baralho desordenado

int quant=0;//quantidade de cartas no baralho

void apresentacao ()
{
    int i;
    for(i=0; i<91; i++) printf("%c ", 3);
    printf("\n");
    for(i=0; i<91; i++) printf(" %c", 4);
    printf("\n");
    for(i=0; i<91; i++) printf("%c ", 5);
    printf("\n");
    for(i=0; i<91; i++) printf(" %c", 6);
    printf("\n\n\n");
}

void inicializa ()//inicializa as pilhas
{
    int i, j;
    for (i=0; i<8; i++)
    {
        primMesa[i]=NULL;//8montinhos da mesa
    }
    for (j=0; j<4; j++)
    {
        primNaipe[i]=NULL;//4 espaços de naipe
        temp[i]=NULL;//4 espaços temporarios
    }
    apresentacao();
}

void gerarBaralho()//gera um baralho ordenado
{
    tCarta*aux=NULL;
    int i, j;
    for(i=3; i<=6; i++)//naipes
    {
        for(j=1; j<=13; j++)//numeros
        {
            tCarta*novo=(tCarta*)malloc(sizeof(tCarta));
            if(primMonte==NULL) primMonte=novo;
            novo->carta=j;//passa o numero para a carta
            novo->naipe=i;//passa o naipe para a carta
            novo->prox=NULL;
            if(aux!=NULL) aux->prox=novo;
            aux=novo;
        }
    }
}

void embaralhaBaralho() //desordena o baralho da seguinte forma: pega sempre a PRIMEIRA carta de primMonte
                        //e insere ela na posição sorteada
{
    tCarta*atual=primMonte;
    while(atual!=NULL)
    {
        tCarta*novo=atual;
        int pos=rand()%(quant+1);
        //printf("%d:%c", atual->carta, atual->naipe);
        atual=atual->prox;
        if(pos==0)//insere na primeira posição
        {
            novo->prox=embaralha;
            embaralha=novo;
        }
        else//insere em qualquer posição
        {
            tCarta*anterior=embaralha;
            int i;
            for(i=0; i<pos-1; i++)
            {
                anterior=anterior->prox;
            }
            novo->prox=anterior->prox;
            anterior->prox=novo;
        }
        quant++;
    }
}

//funções de verificação

bool checkCanMoveTemp ()
{
    int i;
    tCarta*aux;
    for(i=0; i<4; i++)
    {
        aux=temp[i];
        if(aux==NULL) return true;//se o temp estiver vazio
    }
    return false;//se nenhum esta vazio
}

bool checkIsNaipeRed (tCarta*cartinha)
{
    if(cartinha->naipe == 3 || cartinha->naipe == 4) return true;//esses numeros representam copas e ouros (vermelhas)
    else return false;
}

bool checkCanMoveAux (tCarta*cartinha) // checa se pode mover p/ aux -- as cartas sao inseridas em ordem reversa
{
    if(aux==NULL) return true;//se nao houver nenhuma carta no aux
    else//se houverem cartas no aux
    {
        if(checkIsNaipeRed(aux) && checkIsNaipeRed(cartinha)) return false;//se as duas forem vermelhas
        if(!checkIsNaipeRed(aux) && !checkIsNaipeRed(cartinha)) return false;//se as duas forem pretas
        if(cartinha->carta-aux->carta!=1) return false;//se nao forem consecutivas
    }
    return true;//se nao caiu em nenhuma das armadilhas
}

bool checkCanMoveMesa(tCarta *tempCarta, tCarta *ultimaMesa)
{
    if(ultimaMesa!=NULL)//se tem carta na mesa
    {
        if(ultimaMesa->carta - tempCarta->carta != 1)return false;//se nao for carta consecutiva, ja retorna false
        if(checkIsNaipeRed(tempCarta) && checkIsNaipeRed(ultimaMesa)) return false;//se ambas forem vermelhas! retorna false
        if(!checkIsNaipeRed(tempCarta) && !checkIsNaipeRed(ultimaMesa)) return false;//se ambas forem vermelhas! retorna false
        return true;//se nao caiu em nenhuma das armadilhas
    }
    return true;
}

bool checkCanMoveNaipe (tCarta*cartinha)
{
    //descobre o naipe
    //printf("naipe: %c\n", cartinha->naipe);
    //printf("naipe: %d\n", cartinha->naipe);
    int numNaipe = cartinha->naipe - 3;
    //checa possibilidades
    if(primNaipe[numNaipe]==NULL)
    {
        if(cartinha->carta != 1){
            printf("E impossivel realizar essa operacao\n");
            return false;//checa se é primeira carta e se é o 13
        }
    } 
    else if(primNaipe[numNaipe]->carta-cartinha->carta!=-1)
    {   
        printf("E impossivel realizar essa operacao.\n");
        return false;//checa se é carta consecutiva
    }
    return true;//deu tudo certo
}

//funções para adicionar cartas nas pilhas

void empilhaNaipe (tCarta*cartinha)
{
    //descobre o naipe
    int numNaipe = cartinha->naipe - 3;

    //realiza operação
    tCarta*novo=(tCarta*)malloc(sizeof(tCarta));//variavel auxiliar
    novo->carta=cartinha->carta;
    novo->naipe=cartinha->naipe;
    novo->prox=primNaipe[numNaipe];
    primNaipe[numNaipe]=novo;
}


void empilhaTemp (int numMesa, tCarta*empilha)//insere cartas no temp
{
    int i;
    for(i=0; i<4; i++)
    {
        if(temp[i]==NULL)//se o temp estiver vazio
        {
            temp[i]=empilha;
            break;
        }
    }
}

void empilhaMesa (int i, tCarta*atual)
{
    tCarta*novo=(tCarta*)malloc(sizeof(tCarta));//cria posição na memoria
    novo->carta=atual->carta;
    novo->naipe = atual->naipe;
    novo->prox=NULL;
    novo->ant=NULL;
    if(primMesa[i]==NULL) primMesa[i] = novo;//testa se é a primeira posição
    else//se nao for primeira posição
    {
        primMesa[i]->ant=novo;//so para ajudar na impressao
        novo->prox=primMesa[i];//entra ANTES do primMesa[i]
        primMesa[i]=novo;//o novo se torna o novo primeiro elemento (primMesa[i])
    }
}

void empilhaAux (tCarta*atual)
{
    tCarta*novo=(tCarta*)malloc(sizeof(tCarta));//cria posição na memoria
    novo->carta=atual->carta;
    novo->naipe=atual->naipe;
    if(aux==NULL) aux=novo;//testa se é a primeira posição
    else//se nao for a primeira posição
    {
        novo->prox=aux;//entra antes do aux
        aux=novo;//aux troca de posição
    }
}

void distribuirMesa()
{
    tCarta*atual=embaralha;
    int i;
    for(i=0; i<52; i++)
    {
        empilhaMesa(i%8, atual);
        atual=atual->prox;
    }
}

//funções de remover cartas da pilha;

void desempilhaTemp(int numTemp)//retira cartas do temp
{
    temp[numTemp]=NULL;//apenas tira a carta e volta pra NULL
}

void desempilhaMesa(int numMesa)//retira cartas da mesa
{
    primMesa[numMesa]->prox->ant=NULL;
    primMesa[numMesa]=primMesa[numMesa]->prox; // o ponteiro aponta para a proxima
    //a posição na memoria nao é perdida pois o temp ou o naipe ja estao apontando para ela
}

void desempilhaNaipe (int numNaipe)//retira cartas da mesa
{
    primNaipe[numNaipe]->prox->ant=NULL;
    primNaipe[numNaipe]=primNaipe[numNaipe]->prox; // o ponteiro aponta para a proxima
    //a posição na memoria nao é perdida pois o temp ou o naipe ja estao apontando para ela
}

void desempilhaAux ()
{
    aux=aux->prox;
}

//funções de movimento

void moveMesaNaipe()
{
    int numMesa;

    printf("Numero da mesa: ");
    scanf("%d", &numMesa);

    if(numMesa<1 || numMesa>8) printf("Opcao invalida.\n");
    else
    {
        if(checkCanMoveNaipe(primMesa[numMesa-1]))
        {
            empilhaNaipe(primMesa[numMesa-1]);
            desempilhaMesa(numMesa-1);
        }
    }
}

void moveMesaTemp()//move cartas da mesa para o temp
{
    int numMesa;
    printf("Numero da mesa: ");
    scanf("%d", &numMesa);

    if(numMesa<1 || numMesa>8) printf("Opcao Invalida.\n");
    else
    {
        if(checkCanMoveTemp)
        {
            empilhaTemp((numMesa-1), primMesa[numMesa-1]);//primeiro insere no temp
            desempilhaMesa(numMesa-1);//depois retira da mesa, senao perde a localização do ponteiro
        }
        else printf("E impossivel realizar essa operacao.\n");
    }
}

void moveMesaMesa ()
{
    int numMesaOrigem, numMesaDestino, op, numCartas, quantOrigem=0;
    printf("Numero da mesa de origem: ");
    scanf("%d", &numMesaOrigem);
    printf("Numero da mesa de destino: ");
    scanf("%d", &numMesaDestino);

    if(numMesaOrigem<1 || numMesaOrigem>8 || numMesaDestino<1 || numMesaDestino>8) printf("E impossivel realizar essa operacao.\n");
    else
    {
        printf("(1) Mover apenas uma carta\n");
        printf("(2) Mover mais cartas\n");
        printf("Opcao: ");
        scanf("%d", &op);

        if(op==1)
        {
            if(checkCanMoveMesa(primMesa[numMesaOrigem-1], primMesa[numMesaDestino-1]))
            {
                empilhaMesa(numMesaDestino-1, primMesa[numMesaOrigem-1]);
                desempilhaMesa(numMesaOrigem-1);
            }
            else printf("E impossivel realizar essa operacao.\n");
        }
        else if (op==2)
        {
            tCarta*cont;
            cont=primMesa[numMesaOrigem-1];
            while(cont!=NULL)
            {
                quantOrigem++;
                cont=cont->prox;
            }

            printf("Numero de cartas desejado: ");
            scanf("%d", &numCartas);

            if(numCartas>quantOrigem) printf("E impossivel realizar essa operacao.\n");
            else
            {
                int i;
                for (i=0; i<numCartas; i++)
                {
                    if(checkCanMoveAux(primMesa[numMesaOrigem-1]))
                    {
                            empilhaAux(primMesa[numMesaOrigem-1]);
                            desempilhaMesa(numMesaOrigem-1);
                    }
                    else
                    {
                        printf("E impossivel realizar essa operacao.\n");
                        while(aux!=NULL)
                        {
                                empilhaMesa(numMesaOrigem-1, aux);
                                desempilhaAux();
                        }
                        break;
                    } 
                }
                while(aux!=NULL)
                {
                    empilhaMesa((numMesaDestino-1), aux);
                    desempilhaAux();
                }
            }
        }
        else printf("E impossivel realizar essa operacao.\n");
    }
}

void moveTempMesa()
{
    int numTemp, numMesa;

    printf("Numero da temp:");
    scanf("%d", &numTemp);

    printf("Numero da Mesa:");
    scanf("%d", &numMesa);

    if(temp[numTemp]==NULL) printf("E impossivel realizar essa operacao.\n");
    else
    {
        if(checkCanMoveMesa(temp[numTemp-1], primMesa[numMesa-1]))
        {
            empilhaMesa(numMesa-1, temp[numTemp-1]);
            desempilhaTemp(numTemp-1);
        }
        else
        {
            printf("E impossivel realizar essa operacao.\n");
        }
    }
}

void moveTempNaipe ()
{
    int numTemp;
    printf("Numero da temp:");
    scanf("%d", &numTemp);

    if(temp[numTemp]==NULL) printf("E impossivel realizar essa operacao.\n");
    else
    {
        if(numTemp<1 || numTemp>4) printf("E impossivel realizar essa operacao.\n");
        else
        {
            if(checkCanMoveNaipe(temp[numTemp-1]))
            {
                empilhaNaipe(temp[numTemp-1]);
                desempilhaTemp(numTemp-1);
            }
            else printf("E impossivel realizar essa operacao.\n");
        }
    }
}

void moveNaipeMesa ()
{
    int numNaipe, numMesa;

    printf("(1)= %c\n", 3);
    printf("(2)= %c\n", 4);
    printf("(3)= %c\n", 5);
    printf("(4)= %c\n", 6);
    printf("Numero do naipe:");
    scanf("%d", &numNaipe);

    printf("Numero da mesa: ");
    scanf("%d", &numMesa);

    if(numNaipe<1 || numNaipe>4 || numMesa<1 || numMesa>8) printf("E impossivel realizar essa operacao.\n");
    else
    {
        if(checkCanMoveMesa(primNaipe[numNaipe-1], primMesa[numMesa-1]))
        {
            empilhaMesa((numMesa-1), primNaipe[numNaipe-1]);
            desempilhaNaipe(numNaipe-1);
        }
        else  printf("E impossivel realizar essa operacao.\n");
    }

}

void moveNaipeTemp ()
{
    int numNaipe;
    printf("(1)= %c\n", 3);
    printf("(2)= %c\n", 4);
    printf("(3)= %c\n", 5);
    printf("(4)= %c\n", 6);
    printf("Numero do naipe:");
    scanf("%d", &numNaipe);

    if(numNaipe<1 || numNaipe>4) printf("E impossivel realizar essa operacao.\n");
    else 
    {
        if(checkCanMoveTemp)
        {
            empilhaTemp((numNaipe-1), primNaipe[numNaipe-1]);
            desempilhaNaipe(numNaipe-1);
        }
        else printf("E impossivel realizar essa operacao.\n");
    }
}

//imprime

void imprimeBaralho() //imprime baralho embaralhado
{
    tCarta*atual=embaralha;
    int i=0;
    while(atual!=NULL){
        switch (atual->carta)//o switch ajusta as cartas que mostram letras (K, Q, J , A)
        {
        case 13:
            printf("[ %c/%c] ", 'K', atual->naipe);
            break;
        case 12:
            printf("[ %c/%c] ", 'Q', atual->naipe);
            break;
        case 11:
            printf("[ %c/%c] ", 'J', atual->naipe);
            break;
        case 1:
            printf("[ %c/%c] ", 'A', atual->naipe);
            break;            
        default:
            printf("[%2d/%c] ", atual->carta, atual->naipe);
        }
        atual=atual->prox;
        i++;
        if(i%13==0)printf("\n");
    }
    printf("\n");
}

void imprimeMesa() //imprime todas as cartas distribuidas na mesa
{
    int i;
    printf("[TEMP]= ");
    for (i=0; i<4; i++)
    {
        if(temp[i]==NULL) printf("[    ] ");
        else
        {
            switch (temp[i]->carta)//o switch ajusta as cartas que mostram letras (K, Q, J , A)
            {
            case 13:
                printf("[ %c/%c] ", 'K', temp[i]->naipe);
                break;
            case 12:
                printf("[ %c/%c] ", 'Q', temp[i]->naipe);
                break;
            case 11:
                printf("[ %c/%c] ", 'J', temp[i]->naipe);
                break;
            case 1:
                printf("[ %c/%c] ", 'A', temp[i]->naipe);
                break;
            
            default:
                printf("[%2d/%c] ", temp[i]->carta, temp[i]->naipe);
            }
        }
    }

    printf("\n\n\n");

    tCarta*marcapos;
    for (i=0; i<4; i++)
    {
        printf("[NAIPE %c]= ", i+3);
        marcapos=primNaipe[i];
        while(marcapos!=NULL) 
        {
            switch (marcapos->carta)//o switch ajusta as cartas que mostram letras (K, Q, J , A)
            {
            case 13:
                printf("[ %c/%c] ", 'K', marcapos->naipe);
                break;
            case 12:
                printf("[ %c/%c] ", 'Q', marcapos->naipe);
                break;
            case 11:
                printf("[ %c/%c] ", 'J', marcapos->naipe);
                break;
            case 1:
                printf("[ %c/%c] ", 'A', marcapos->naipe);
                break;
            
            default:
                printf("[%2d/%c] ", marcapos->carta, marcapos->naipe);
            }
            marcapos = marcapos->prox;
        }
        printf("\n");
    }
    
    printf("\n\n");

    for(i=0; i<8; i++)
    {
        printf("[MESA %d]= ", (i+1));
        tCarta*atual=primMesa[i];
        while(atual->prox!=NULL) atual=atual->prox;
        while(atual!=NULL)
        {
            switch (atual->carta)//o switch ajusta as cartas que mostram letras (K, Q, J , A)
            {
            case 13:
                printf("[ %c/%c] ", 'K', atual->naipe);
                break;
            case 12:
                printf("[ %c/%c] ", 'Q', atual->naipe);
                break;
            case 11:
                printf("[ %c/%c] ", 'J', atual->naipe);
                break;
            case 1:
                printf("[ %c/%c] ", 'A', atual->naipe);
                break;
            default:
                printf("[%2d/%c] ", atual->carta, atual->naipe);
            }
            atual=atual->ant;
        }
        printf("\n");
    }
}

void clear () //da free em todas as variaveis alocadas na memoria apos o fim do jogo
{
    tCarta*aux;
    tCarta*apaga;
    int i;
    for(i=1; i<4; i++)
    {
        apaga=temp[i];
        free(apaga);
    }
    for(i=3; i<6; i++)
    {
        apaga = primNaipe[i];
        while(apaga!=NULL)
        {
            aux=apaga->prox;
            free(apaga);
            apaga=aux;
        }
    }
    for(i=0; i<8; i++)
    {
        apaga = primMesa[i];
        while(apaga!=NULL)
        {
            aux=apaga->prox;
            free(apaga);
            apaga=aux;
        }
    }
    apaga=embaralha;
    while(apaga!=NULL)
    {
        aux=apaga->prox;
        free(apaga);
        apaga=aux;
    }
}

void Play ()
{
    int op = 0;
    printf("\n");
    while(op!=8){
        imprimeMesa();

        printf("(1)Move Mesa-Naipe\n");
        printf("(2)Move Mesa-Temp\n");
        printf("(3)Move Mesa-Mesa\n");
        printf("(4)Move Temp-Mesa\n");
        printf("(5)Move Temp-Naipe\n");
        printf("(6)Move Naipe-Mesa\n");
        printf("(7)Move Naipe-Temp\n");
        printf("(8)Sair\n");
        printf("Opcao: ");
        scanf("%d", &op);
        getchar();
        switch(op){
            case 1:
                moveMesaNaipe();
                break;
            case 2:
                moveMesaTemp();
                break;
            case 3:
                moveMesaMesa();
                break;
            case 4:
                moveTempMesa();
                break;
            case 5:
                moveTempNaipe();
                break;
            case 6:
                moveNaipeMesa();
                break;
            case 7:
                moveNaipeTemp();
            case 8:
                printf("\n\n\nJogo finalizado!!\n\n\n");
                apresentacao();
                clear();
                break;
            default:
                printf("Opcao Invalida.\n");
                break;
        }
    }
}

void avisos()
{
    printf("\n\n=>Na mesa as cartas sao movidas pelo lado direito\n");
    printf("=>No naipe as cartas sao movidas pelo lado esquerdo\n");
    printf("=>Qualquer carta pode ser movida para uma mesa vazia\n\n");
}

int main(int argc, char **argv)
{
    int op=0;
    srand(time(NULL));
    inicializa();
    avisos();
    gerarBaralho();
    embaralhaBaralho();
    //imprimeBaralho ();
    distribuirMesa();
    Play();
    return 0;
}