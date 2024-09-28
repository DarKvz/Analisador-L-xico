#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_SYMBOLS 100


typedef struct {
    char nome[20];    
    char lexema[50];  
    int linha;        
    int coluna;       
} Token;


typedef struct {
    char simbolo[50]; 
    char tipo[20];    
} Simbolo;


Simbolo tabela_simbolos[MAX_SYMBOLS];
int ts_count = 0;
Token tokens[MAX_TOKENS];
int token_count = 0;


void inicializar_tabela_simbolos() {
    const char *palavras_reservadas[] = {
        "program", "var", "integer", "real", "begin", "end",
        "if", "then", "else", "while", "do"
    };
    for (int i = 0; i < sizeof(palavras_reservadas) / sizeof(palavras_reservadas[0]); i++) {
        strcpy(tabela_simbolos[ts_count].simbolo, palavras_reservadas[i]);
        strcpy(tabela_simbolos[ts_count++].tipo, "palavra reservada");
    }
}


int eh_palavra_reservada(char *lexema) {
    for (int i = 0; i < ts_count; i++) {
        if (strcmp(tabela_simbolos[i].simbolo, lexema) == 0) {
            return 1; 
        }
    }
    return 0;
}


void adicionar_identificador_tabela_simbolos(char *lexema) {
    for (int i = 0; i < ts_count; i++) {
        if (strcmp(tabela_simbolos[i].simbolo, lexema) == 0) {
            return; 
        }
    }
    strcpy(tabela_simbolos[ts_count].simbolo, lexema);
    strcpy(tabela_simbolos[ts_count++].tipo, "identificador");
}


void analisar_codigo(char *codigo) {
    int linha = 1, coluna = 0, i = 0;
    char lexema[50];
    int j = 0;
    
    while (codigo[i] != '\0') {
        char c = codigo[i];
        
        
        if (isalpha(c)) { 
            j = 0;
            while (isalnum(c)) {
                lexema[j++] = c;
                i++;
                c = codigo[i];
                coluna++;
            }
            lexema[j] = '\0';
            if (eh_palavra_reservada(lexema)) {
                strcpy(tokens[token_count].nome, "KEYWORD");
            } else {
                strcpy(tokens[token_count].nome, "IDENTIFIER");
                adicionar_identificador_tabela_simbolos(lexema);
            }
            strcpy(tokens[token_count].lexema, lexema);
            tokens[token_count].linha = linha;
            tokens[token_count++].coluna = coluna;
        } else if (isdigit(c)) { 
            j = 0;
            while (isdigit(c)) {
                lexema[j++] = c;
                i++;
                c = codigo[i];
                coluna++;
            }
            lexema[j] = '\0';
            strcpy(tokens[token_count].nome, "NUMBER");
            strcpy(tokens[token_count].lexema, lexema);
            tokens[token_count].linha = linha;
            tokens[token_count++].coluna = coluna;
        } else if (isspace(c)) { 
            if (c == '\n') {
                linha++;
                coluna = 0;
            } else {
                coluna++;
            }
            i++;
        } else { 
            switch (c) {
                case '+': strcpy(tokens[token_count].nome, "OP_AD"); break;
                case '-': strcpy(tokens[token_count].nome, "OP_MIN"); break;
                case '*': strcpy(tokens[token_count].nome, "OP_MUL"); break;
                case '/': strcpy(tokens[token_count].nome, "OP_DIV"); break;
                case '=': strcpy(tokens[token_count].nome, "OP_EQ"); break;
                case ';': strcpy(tokens[token_count].nome, "SMB_SEM"); break;
                case ':': strcpy(tokens[token_count].nome, "OP_ASS"); break;
                case '(': strcpy(tokens[token_count].nome, "SMB_OPA"); break;
                case ')': strcpy(tokens[token_count].nome, "SMB_CPA"); break;
                default:
                    printf("Erro: Caracter desconhecido '%c' na linha %d coluna %d\n", c, linha, coluna);
                    i++;
                    continue;
            }
            lexema[0] = c;
            lexema[1] = '\0';
            strcpy(tokens[token_count].lexema, lexema);
            tokens[token_count].linha = linha;
            tokens[token_count++].coluna = coluna;
            coluna++;
            i++;
        }
    }
}


void salvar_tokens_arquivo(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de saída.\n");
        return;
    }
    for (int i = 0; i < token_count; i++) {
        fprintf(file, "Token: %-10s Lexema: %-10s Linha: %d Coluna: %d\n", 
                tokens[i].nome, tokens[i].lexema, tokens[i].linha, tokens[i].coluna);
        printf("Token: %-10s Value: %-10s Line: %d Column: %d\n", 
                tokens[i].nome, tokens[i].lexema, tokens[i].linha, tokens[i].coluna);
    }
    fclose(file);
}

int main() {
    
    inicializar_tabela_simbolos();

    
    char codigo[] = 
        "program Exemplo;\n"
        "var x, y : integer;\n"
        "z : real;\n"
        "begin\n"
        "   x := 10;\n"
        "   y := 20;\n"
        "   if x > y then\n"
        "      x := x - 1;\n"
        "end.\n";

 
    analisar_codigo(codigo);

   
    salvar_tokens_arquivo("output.lex");

    return 0;
}
