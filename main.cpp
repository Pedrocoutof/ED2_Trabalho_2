#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <conio.h>
#include <sstream>
#include <string.h>

#define NUM_MIN_DE_REGISTRO 50000 // TAMANHO DE LINHAS DO ARQUIVO '.csv' QUE USAMOS PARA TESTES //

#define TAM_TABELA_HASH 150 // TAMANHO DA TABELA HASH

using namespace std;

int numeroRegistrosGravados = 0;
int movimentacoes = 0;
int comparacoes = 0;

int valorAleatorio(){
   if(numeroRegistrosGravados == 0)
        return rand() % NUM_MIN_DE_REGISTRO;

    return rand() % numeroRegistrosGravados;
}

// ------------------ Estrutura usada para armazenar os reviews ------------------ //
typedef struct{

    char review_id [90];        //  string identificadora única
    char review_text[1502];     //  o review postado pelo usuário
    char app_version[20];       //  string indicando a versão do aplicativo a que o review se refere
    char posted_date[20];       //  data e hora de postagem
    int  upvotes;               //  número de votos favoráveis ao review

}TiktokReviews;



/// -------------- Funções de manipulação do TiktokReviews -------------- ///

void imprimeConteudoTiktokReviews(TiktokReviews tiktokReviews){

    cout <<     "Review ID : "       <<      tiktokReviews.review_id      << endl;
    cout <<     "Review Text : "     <<      tiktokReviews.review_text    << endl;
    cout <<     "Up Votes : "        <<      tiktokReviews.upvotes       << endl;
    cout <<     "App Version : "     <<      tiktokReviews.app_version    << endl;
    cout <<     "Posted Date : "     <<      tiktokReviews.posted_date    << endl << endl;

}

void gravaTiktokReviewEmBinario(FILE * arq_bin, TiktokReviews tiktokReviews){

    if(!fwrite(tiktokReviews.review_id,   sizeof(tiktokReviews.review_id),   1, arq_bin)){
        exit(10);
    };
    if(!fwrite(tiktokReviews.review_text, sizeof(tiktokReviews.review_text), 1, arq_bin)){
        exit(10);
    };
    if(!fwrite(&tiktokReviews.upvotes,    sizeof(tiktokReviews.upvotes),     1, arq_bin)){
        exit(10);
    };
    if(!fwrite(tiktokReviews.app_version, sizeof(tiktokReviews.app_version), 1, arq_bin)){
        exit(10);
    };
    if(!fwrite(tiktokReviews.posted_date, sizeof(tiktokReviews.posted_date), 1, arq_bin)){
        exit(10);
    };
}

// Função usada para ler todo arquivo '.csv' e gravar em '.bin'
void gravaCsvEmBinario(FILE * arq_csv, FILE * arq_bin){

    rewind(arq_csv);    // REINICIA O PONTEIRO DO '.csv' //
    rewind(arq_bin);    // REINICIA O PONTEIRO DO '.bin' //
    char ch;

    for(ch = fgetc(arq_csv); ch != '\n'; ch = fgetc(arq_csv)){}    // PULA A PRIMEIRA LINHA DO '.csv' JÁ QUE A PRIMEIRA LINHA TEM INFORMAÇÕES DE COMO O ARQUIVO É SEPARADO //


    while(ch!=EOF){
        cout << "Numero de Registros gravados : "  << numeroRegistrosGravados << endl;
        TiktokReviews tiktokReviews;
        tiktokReviews.review_text[0] = '\0';
        tiktokReviews.app_version[0] = '\0';


    /// LEITURA REVIEW_ID
    fscanf(arq_csv, "%[^,]", tiktokReviews.review_id);
    //cout << "Review_id : " << tiktokReviews.review_id << endl;
    ch = fgetc(arq_csv);
    ch = fgetc(arq_csv);

    /// LEITURA REVIEW_TEXT
    if(ch == '"') // VERIFICA SE POSSUI ASPAS
        {
        bool verificaPrimeiroCaractereNumero = true;
        int i = 0;
        ch = fgetc(arq_csv);
        while(!(ch >= 48 && ch <= 57) || verificaPrimeiroCaractereNumero){
            verificaPrimeiroCaractereNumero = false;
            while(ch != ','){
                    while(ch != '"'){
                        tiktokReviews.review_text[i] = ch;
                        i++;
                        ch = fgetc(arq_csv);

                    }
                    ch = fgetc(arq_csv);
            }
            ch = fgetc(arq_csv);
    }
    fseek(arq_csv, -1, SEEK_CUR);

    tiktokReviews.review_text[i] = '\0';
        }
    // SE NAO POSSUIR LE ATÉ A VIRGULA
    else{
        fseek(arq_csv, -1, SEEK_CUR);
        fscanf(arq_csv, "%[^,]", tiktokReviews.review_text);
        fseek(arq_csv, +1, SEEK_CUR);
    }
    //cout << "Review_text : " << tiktokReviews.review_text << endl;


    /// LEITURA UPVOTES
    fscanf(arq_csv, "%d[^,]", &tiktokReviews.upvotes);
    //cout << "Upvotes : " << tiktokReviews.upvotes << endl;
    ch = fgetc(arq_csv);

    /// LEITURA APP_VERSION
    fscanf(arq_csv, "%[^,]", tiktokReviews.app_version);
    //cout << "App_Version : " << tiktokReviews.app_version << endl;
    ch = fgetc(arq_csv);

    /// LEITURA POSTED_DATE
    fscanf(arq_csv, "%[^\n]", tiktokReviews.posted_date);
    //cout << "Posted_Date : " << tiktokReviews.posted_date << endl << endl;
    ch = fgetc(arq_csv);


    gravaTiktokReviewEmBinario(arq_bin, tiktokReviews);

    //printaConteudoTiktokReviews(tiktokReviews);
    numeroRegistrosGravados++;
    }

    fclose(arq_bin);
    fclose(arq_csv);

}

// Função que acessa registro na posição 'i' e imprime no console
void acessaRegistro(int i){

    FILE * arq_bin = fopen("tiktok_app_reviews.bin", "r+b");

    if(arq_bin == NULL){
        cout << "Erro! Arquivo não encontrado no diretorio : " << arq_bin;
        exit(404);
    }
    rewind(arq_bin);
    TiktokReviews tiktokReviews;

    fseek(arq_bin, i * sizeof(TiktokReviews), SEEK_CUR);

    fread(&tiktokReviews.review_id, sizeof(tiktokReviews.review_id), 1, arq_bin);
    fread(&tiktokReviews.review_text, sizeof(tiktokReviews.review_text), 1, arq_bin);
    fread(&tiktokReviews.upvotes, sizeof(tiktokReviews.upvotes), 1, arq_bin);
    fread(&tiktokReviews.app_version, sizeof(tiktokReviews.app_version), 1, arq_bin);
    fread(&tiktokReviews.posted_date, sizeof(tiktokReviews.posted_date), 1, arq_bin);

    for(int i = 0; i < 14; i ++){
        printf("%c", 461);
    }
    cout << "  ACESSANDO REGISTRO : " << i << "  ";

    for(int i = 0; i < 14; i ++){
        printf("%c", 461);
    }
    cout << endl;
    imprimeConteudoTiktokReviews(tiktokReviews);
    fclose(arq_bin);
}

// Função que retorna registro gravado na posição 'i' do arquivo binário
TiktokReviews retornaRegistro(int i){

    FILE * arq_bin = fopen("tiktok_app_reviews.bin", "rb");

    if(arq_bin == NULL){
        cout << "Erro! Arquivo não encontrado no diretorio : " << arq_bin;
        exit(404);
    }

    TiktokReviews tiktokReviews;

    fseek(arq_bin, i * sizeof(TiktokReviews), 1);

    fread(&tiktokReviews.review_id, sizeof(tiktokReviews.review_id), 1, arq_bin);
    fread(&tiktokReviews.review_text, sizeof(tiktokReviews.review_text), 1, arq_bin);
    fread(&tiktokReviews.upvotes,  sizeof(tiktokReviews.upvotes), 1, arq_bin);
    fread(&tiktokReviews.app_version, sizeof(tiktokReviews.app_version), 1, arq_bin);
    fread(&tiktokReviews.posted_date, sizeof(tiktokReviews.posted_date), 1, arq_bin);


    fclose(arq_bin);

    return tiktokReviews;
}

void gravaTiktokReviewEmTxt(FILE * arq_txt, TiktokReviews tiktokReviews){
            fprintf(arq_txt, "%s", tiktokReviews.review_id);
            imprimeConteudoTiktokReviews(tiktokReviews);
            fprintf(arq_txt, "%c", 44);
            fprintf(arq_txt, "%s", tiktokReviews.review_text);
            fprintf(arq_txt, "%c", 44);
            fprintf(arq_txt, "%d", tiktokReviews.upvotes);
            fprintf(arq_txt, "%c", 44);
            fprintf(arq_txt, "%s", tiktokReviews.app_version);
            fprintf(arq_txt, "%c", 44);
            fprintf(arq_txt, "%s", tiktokReviews.posted_date);
            fprintf(arq_txt, "%c", '\n');
}

// Função que gera um menu no console para interação com o usuário para testar a importação
void menuTesteImportacao(){
printf("%c", 201);
    for(int i = 0; i < 55; i ++){
        printf("%c", 461);
    }
    printf("%c\n", 443);
    printf("%c", 186);
    printf("\t\t\t\t\t\t\t");
    printf("%c\n", 186);
    printf("%c", 186);
    printf("\t\t\tVoce Deseja:\t\t\t");
    printf("%c", 186);
    printf("\n%c", 186);
    printf("\t\t\t            \t\t\t");
    printf("%c\n", 186);
    printf("%c", 186);
    printf("\t\t1 - Imprimir no Console\t\t\t");
    printf("%c\n", 186);
    printf("%c", 186);
    printf("\t\t2 - Guardar no arquivo texto\t\t");
    printf("%c\n", 186);
    printf("%c", 186);
    printf("\t\t\t\t\t\t\t");
    printf("%c\n", 186);
    printf("%c", 186);
    printf("\t\t\t\t\t\t\t");
    printf("%c\n", 186);
    printf("%c", 200);

    for(int i = 0; i < 55; i ++){
        printf("%c", 461);
    }
    printf("%c\n", 188);

}

void testaImportacao(){

    int resp;
    scanf("%d", &resp);

    srand(time(nullptr));

    if(resp == 1){
        for(int i = 0; i < 10; i++){
                acessaRegistro(valorAleatorio());
        }
    }

    else if(resp == 2){
        FILE * arq_txt = fopen("registrosAleatorios.txt", "w");
        for(int i = 0; i < 100; i++){
            TiktokReviews tiktokReviews = retornaRegistro(valorAleatorio());
            gravaTiktokReviewEmTxt(arq_txt, tiktokReviews);
        }
    }


}

// Função que gera um menu no console para interação com o usuário para uso da funcao 'acessaRegistro'
int menuAcessaRegistro(){
    printf("    Digite o Valor do registro que deseja acessar:     ");
    int resp;
    scanf(" %d", &resp);
    return resp;

}

// ---------- Fim das funções de manipulação do TiktokReviews ---------- //


/// ------------------ Funções de ordenação ------------------ ///
void troca(TiktokReviews &a, TiktokReviews &b)
{
    TiktokReviews aux = a;
    a = b;
    b = aux;
}

// ------- HeapSort ------- //
void peneira(TiktokReviews *tiktokReviews, int raiz, int fundo) {
	int pronto, filhoMax;
	TiktokReviews aux;

	pronto = 0;
	while ((raiz*2 <= fundo) && (!pronto)) {
        comparacoes++;
		if (raiz*2 == fundo) {
			filhoMax = raiz * 2;
		}
		else if (tiktokReviews[raiz * 2].upvotes > tiktokReviews[raiz * 2 + 1].upvotes) {
			filhoMax = raiz * 2;
		}
		else {
			filhoMax = raiz * 2 + 1;
		}

	if (tiktokReviews[raiz].upvotes < tiktokReviews[filhoMax].upvotes) {
        movimentacoes++;
		aux = tiktokReviews[raiz];
		tiktokReviews[raiz] = tiktokReviews[filhoMax];
		tiktokReviews[filhoMax] = aux;
		raiz = filhoMax;
    }
	else {
      pronto = 1;
	}
  }
}

void heapsort(TiktokReviews *tiktokReviews, int n) {
    movimentacoes = 0;
    comparacoes = 0;
	int i;
	TiktokReviews aux;

	for (i = (n / 2); i >= 0; i--) {
		peneira(tiktokReviews, i, n - 1);
	}

	for (i = n-1; i >= 1; i--) {
		aux = tiktokReviews[0];
		tiktokReviews[0] = tiktokReviews[i];
		tiktokReviews[i] = aux;
		peneira(tiktokReviews, 0, i-1);
	}
}


// ------- QuickSort -------//
TiktokReviews medianaDeTres(TiktokReviews *v, int ini, int fim)
{
    int meio = (ini+fim)/2;

    comparacoes++;
    if(v[ini].upvotes > v[fim].upvotes){
        movimentacoes++;
        troca(v[ini], v[fim]);
    }

    comparacoes++;
    if(v[meio].upvotes > v[fim].upvotes){
        movimentacoes++;
        troca(v[meio], v[fim]);
    }


    comparacoes++;
    if(v[ini].upvotes > v[meio].upvotes){
        movimentacoes++;
        troca(v[ini], v[meio]);
    }

    movimentacoes++;
    troca(v[meio], v[fim]);
    return v[fim];
}

int particiona(TiktokReviews *v, int ini, int fim)
{
    TiktokReviews pivo = medianaDeTres(v, ini, fim);

    int i = ini, j = fim-1;
    while(true)
    {

        comparacoes++;
        while(i < fim && v[i].upvotes < pivo.upvotes)
            i++;

        comparacoes++;
        while(j >= ini && v[j].upvotes > pivo.upvotes)
            j--;
        comparacoes++;
        if(i < j)
        {
        movimentacoes++;
            troca(v[i], v[j]);
            i++;
            j--;
        }
        else
            break;
    }
    movimentacoes++;
    troca(v[i], v[fim]);
    return i;
}

void quickSortRec(TiktokReviews *v, int ini, int fim)
{
    if(ini < fim)
    {
        int p = particiona(v, ini, fim);
        quickSortRec(v, ini, p-1);
        quickSortRec(v, p+1, fim);
        return;
    }
}

void quickSort(TiktokReviews *v, int n)
{
    movimentacoes = 0;
    comparacoes = 0;
    quickSortRec(v, 0, n-1);
}


// ------- CombSort ------- //
void comb_sort(TiktokReviews * v, int tam)
{
    comparacoes = 0;
    movimentacoes = 0;
	int i, j, intervalo, trocado = 1;
	TiktokReviews aux;
	intervalo = tam;
	while (intervalo > 1 || trocado == 1)
	{
		intervalo = intervalo * 10 / 13;
        comparacoes++;
		if (intervalo == 9 || intervalo == 10) intervalo = 11;

		comparacoes++;
		if (intervalo < 1) intervalo = 1;

		trocado = 0;
		for (i = 0, j = intervalo; j < tam; i++, j++)
		{
		    comparacoes++;
			if (v[i].upvotes > v[j].upvotes)
			{
				aux = v[i];
				v[i] = v[j];
				v[j] = aux;
				trocado = 1;
				movimentacoes++;
			}
		}
	}
}

// ------------------ Fim das funções de ordenação ------------------ //



/// ------------------ Hashing ------------------ ///

// -- Estrutura usada para armazenar os Nós -- //
typedef struct no {
    TiktokReviews tiktokReviews;
    struct no *proximo;
} No;


// -- Estrutura usada para armazenar Lista -- //
typedef struct {
    No *inicio;
    int tam;
} Lista;

Lista * tabelaHash[TAM_TABELA_HASH];

Lista * criarLista() {

    Lista *l = (Lista *) malloc(sizeof(Lista));
    l->inicio = NULL;
    l->tam = 0;
    return l;
}

void inserirInicio(TiktokReviews p, Lista *lista) {
    No *no = (No *) malloc(sizeof(No));
    no->tiktokReviews = p;
    no->proximo = lista->inicio;
    lista->inicio = no;
    lista->tam++;
}

No* buscarNo(char chave[20], No *inicio) {

    while(inicio != NULL) {
        if(inicio->tiktokReviews.app_version == chave)
            return inicio;
        else
            inicio = inicio->proximo;
    }
    return NULL;
}

void imprimirLista(No *inicio) {
    while(inicio != NULL) {
        imprimeConteudoTiktokReviews(inicio->tiktokReviews);
        inicio = inicio->proximo;
    }
}

void inicializar(){
    int i;
    for(i = 0; i < TAM_TABELA_HASH; i++)
        tabelaHash[i] = criarLista();
}

int funcaoHashString(char str[]){
    int i, tamS = strlen(str);
    unsigned int hash = 0;

    for(i = 0; i < tamS; i++)
        hash += str[i] * (i + 1); // somatório do código ASCII vezes a posição
    return hash % TAM_TABELA_HASH;
}

int funcaoHashInt(int chave){
    return chave % TAM_TABELA_HASH;
}

void insereTabela(TiktokReviews tiktokReviews){
    int indice = funcaoHashString(tiktokReviews.app_version);
    inserirInicio(tiktokReviews, tabelaHash[indice]);
}

TiktokReviews * buscarAppVersionTabela(char chave [20]){
    int indice = funcaoHashString(chave);
    No *no = buscarNo(chave, tabelaHash[indice]->inicio);
    if(no)
        return &no->tiktokReviews;
    else
        return NULL;
}

void hashLista(){
    TiktokReviews vetor[80];

    for(int i = 0; i < 80; i++)
        vetor[i] = retornaRegistro(valorAleatorio());

    inicializar();

    for(int i = 0; i < 80; i++){
        cout << "============= " << i << " =============" << endl;
        imprimeConteudoTiktokReviews(vetor[i]);
        insereTabela(vetor[i]);
    }

    cout << endl << endl << "Conteudo Tabela Hash" << endl;

    printf("\n---------------------TABELA-------------------------\n");
    for(int i = 0; i < TAM_TABELA_HASH; i++){
        printf("%d Lista tamanho: %d\n", i, tabelaHash[i]->tam);
        imprimirLista(tabelaHash[i]->inicio);
    }
    printf("---------------------FIM TABELA-----------------------\n");


    cout << "Digite qual versao deseja pesquisar na tabela: ";
    char respString[20];
    cin >> respString;

    imprimirLista(tabelaHash[funcaoHashString(respString)]->inicio);
}

// ------------------ Fim das funções de Hashing ------------------ //


int menuParte2(){
    cout << "O que voce deseja?" << endl;
    int resp;
    cout  << endl << "1 - Teste de Ordenacao" << endl;
    cout  <<         "2 - Hash"               << endl;
    cout  <<         "3 - Modulo de Teste"    << endl;
    cin >> resp;
    return resp;
}

void ordenacao(){

    FILE * arq_saida = fopen("saida.txt", "a");
    srand(time(nullptr));
    cout <<"Voce deseja ordenar um vetor de: " << endl;
    int tam;
    cout << " >> ";
    cin >> tam;

    TiktokReviews * vetor = new TiktokReviews[tam];

    cout << "Aguarde enquanto gera o vetor com registros aleatorios..." << endl;
    for(int i = 0; i < tam; i++){
        vetor[i] = retornaRegistro(valorAleatorio());
    }

    cout << "Deseja ordenar o vetor utilizando qual metodo?" << endl;
    cout << "1 - HeapSort" << endl;
    cout << "2 - QuickSort" << endl;
    cout << "3 - CombSort" << endl;

    cout << " >> ";
    int resp;
    cin >> resp;
    clock_t inicio, fim;
    double tempoGasto;

    if(resp == 1){
        inicio = clock();
        heapsort(vetor, tam);
        fim = clock();
        tempoGasto = (double)(inicio - fim) / CLOCKS_PER_SEC;
        fprintf(arq_saida, "\n- HeapSort de tamanho: %d", tam);
        fprintf(arq_saida, "\nNumero de Comparaçoes = %d", comparacoes);
        fprintf(arq_saida, "\nNumero de Movimentacoes = %d", movimentacoes);
        fprintf(arq_saida, "\nTempo Gasto = %lf\n", (double)tempoGasto);
    }

    else if(resp == 2){
        inicio = clock();
        quickSort(vetor, tam);
        fim = clock();
        tempoGasto = (double)(inicio - fim) / CLOCKS_PER_SEC;
        fprintf(arq_saida, "\n- QuickSort de tamanho: %d", tam);
        fprintf(arq_saida, "\nNumero de Comparaçoes = %d", comparacoes);
        fprintf(arq_saida, "\nNumero de Movimentacoes = %d", movimentacoes);
        fprintf(arq_saida, "\nTempo Gasto = %lf\n", (double)tempoGasto);
    }
    else if(resp == 3){
        inicio = clock();
        comb_sort(vetor, tam);
        fim = clock();
        tempoGasto = (double)(inicio - fim) / CLOCKS_PER_SEC;
        fprintf(arq_saida, "\n- CombSort de tamanho: %d", tam);
        fprintf(arq_saida, "\nNumero de Comparaçoes = %d", comparacoes);
        fprintf(arq_saida, "\nNumero de Movimentacoes = %d", movimentacoes);
        fprintf(arq_saida, "\nTempo Gasto = %lf\n", (double)tempoGasto);

    }

    delete []vetor;
}

void moduloTeste(){
    FILE * arq_teste = fopen("teste.txt", "w");

    srand(time(nullptr));

    int resp;
    cout << "Deseja ordenar o vetor utilizando qual metodo?" << endl;
    cout << "1 - HeapSort" << endl;
    cout << "2 - QuickSort" << endl;
    cout << "3 - CombSort" << endl;
    cout << " >> ";
    cin >> resp;

    fprintf(arq_teste ,"Vetor ordenado vetor utilizando o metodo: ");

    TiktokReviews vetor[100];

    for(int i = 0; i < 100; i++)
        vetor[i] = retornaRegistro(valorAleatorio());

    if(resp == 1){
        heapsort(vetor, 100);
        fprintf(arq_teste, "HeapSort\n");
    }

    else if(resp == 2){
        quickSort(vetor, 100);
        fprintf(arq_teste, "QuickSort\n");
    }

    else if(resp == 3){
        comb_sort(vetor, 100);
        fprintf(arq_teste, "CombSort\n");
    }

    for(int i = 0; i < 100; i++){
        fprintf(arq_teste, "\n=============Indice: %d=============", i);
        fprintf(arq_teste, "\nReview Id : %s", vetor[i].review_id);
        fprintf(arq_teste, "\nReview Text : %s", vetor[i].review_text);
        fprintf(arq_teste, "\nUpvotes : %d", vetor[i].upvotes);
        fprintf(arq_teste, "\nApp Version : %s", vetor[i].app_version);
        fprintf(arq_teste, "\nPosted Date : %s", vetor[i].posted_date);
    }

    fprintf(arq_teste, "\n===========================================================\n");
    fprintf(arq_teste, "Hashing: \n");

    inicializar();

    for(int i = 0; i < 100; i++)
        insereTabela(vetor[i]);

    for(int i = 0; i < 100; i++)
        if(tabelaHash[i]->tam != 0)
            fprintf(arq_teste, "App Version : %s - Numero de Repeticoes : %d\n", tabelaHash[i]->inicio->tiktokReviews.app_version, tabelaHash[i]->tam);

}

int main(int argc, char * argv[])
{
    //char diretorio_csv[] = "H:\\UFJF\\2021.3\\ED2\\Trabalhos\\Arquivos CSV\\tiktok_app_reviews_reduzido_100000.csv";
    //FILE * arq_csv = fopen(diretorio_csv, "r");

    FILE * arq_csv = fopen(argv[1], "r");

    if(arq_csv == NULL){
        cout << "Erro! Arquivo <.csv> nao encontrado no diretorio";
        exit(404);
    }

    char diretorio_bin[] = "tiktok_app_reviews.bin";
    FILE * arq_bin = fopen(diretorio_bin, "r+b");


    if(arq_bin == NULL){
        cout << endl << "Arquivo <" << diretorio_bin << "> nao encontrado." << endl;
        cout << "Gerando <" << diretorio_bin << ">." << endl;
        FILE * arq_bin = fopen(diretorio_bin, "w+b");
        gravaCsvEmBinario(arq_csv, arq_bin);
        fclose(arq_bin);
    }

/*** TRABALHO PARTE 1
    acessaRegistro(menuAcessaRegistro());

    menuTesteImportacao();

    testaImportacao();
*/


/*** TRABALHO PARTE 2 */

    int resp = 1;
    resp = menuParte2();

    if(resp == 1)
        ordenacao();

    else if(resp == 2)
        hashLista();

    else if(resp == 3)
        moduloTeste();

    cout << endl << endl << endl << "Pressione qualquer tecla para sair...";
    getch();

    return 0;
}

