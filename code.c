#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void getParamValue(const char *data, const char *key, char *value) {
    const char *start = strstr(data, key);
    if (start) {
        start += strlen(key) + 1; // Avance après le nom du paramètre et le '='
        const char *end = strchr(start, '&');
        if (!end) {
            end = start + strlen(start); // Si pas de '&', va jusqu'à la fin
        }
        strncpy(value, start, end - start);
        value[end - start] = '\0';
    } else {
        value[0] = '\0'; // Paramètre non trouvé
    }
}

// Fonction pour décoder une chaîne encodée en URL
void urlDecode(char *src, char *dest) {
    char ch;
    while (*src) {
        if (*src == '+') {
            *dest = ' ';
        } else if (*src == '%' && sscanf(src + 1, "%2hhx", &ch) == 1) {
            *dest = ch;
            src += 2;
        } else {
            *dest = *src;
        }
        src++;
        dest++;
    }
    *dest = '\0';
}

int main() {
    char *method = getenv("REQUEST_METHOD");
    int len;
    char *postData;


    // char UNAME[] = "ADMIN";
    // char PWORD[] = "ADMIN";


    if (method && strcmp(method, "POST") == 0) {
        // Récupérer la longueur du contenu POST
        len = atoi(getenv("CONTENT_LENGTH"));

        // Allouer la mémoire pour les données POST
        postData = (char *)malloc(len + 1);

        // Lire les données POST
        fread(postData, 1, len, stdin);
        postData[len] = '\0';  

        // Décoder les données encodées en URL
        // Extraire les données spécifiques du formulaire
        char decodedData[1024];
        char login[50], password[50];
        urlDecode(postData, decodedData);
        // sscanf(strstr(decodedData, "nom=") + 6, "%[^&]", login);
        // sscanf(strstr(decodedData, "pwd=") + 9, "%[^&]", password);

        getParamValue(decodedData, "nom", login);
        getParamValue(decodedData, "pwd", password);


        // if(strcmp(UNAME,login)!=0 || strcmp(PWORD,password)!=0){
        //     printf("Status: 302 Found\n");
        //     printf("Location: /index.cgi?nom=%s&pwd=%s\n\n",login,password);
        // }

        // Afficher les résultats
        printf("Content-type: text/html\n\n");  
        printf("<html><body>");
        printf("<h1>Login: %s</h1>", login);
        printf("<h1>Password: %s</h1>", password);

        printf("</body></html>");
        

    } else {
        printf("<html><body><h1>Erreur : méthode non supportée</h1></body></html>");
    }

    return 0;
}

