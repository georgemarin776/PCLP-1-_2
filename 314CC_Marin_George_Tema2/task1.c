#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum color{white, black, red, green, blue, yellow};

struct site {

    char *content;
    int checksum, length, acc;
    char URL[51], title[51];
};

struct site* init() {

    struct site * A;
    A = (struct site *)malloc(3 * sizeof(struct site));

    return A;
}

void add_size(struct site **A, int *A_size) {

    *A_size +=3;
    *A = realloc(*A, *A_size * sizeof(struct site));
}

void add(struct site *A, char *content, char *URL, char *title, 
    int checksum, int length, int acc, int i) {

    A[i].checksum = checksum;
    A[i].length = length;
    A[i].acc = acc;

    strcpy(A[i].URL, URL);
    strcpy(A[i].title, title);

    A[i].content = malloc((length + 1) * sizeof(char));
    strcpy(A[i].content, content);
}

int main() {

    /// initializare structura

    struct site *A = init();
    int A_size = 3, file_no = -1;
    
    FILE *master_file;
    master_file = fopen("master.txt", "r");

    char *file_name;
    file_name = malloc(31 * sizeof(char));

    /// citirea fisierelor din master + utilizarea lor

    while (fgets(file_name, 30, master_file)) {

        file_no ++;
        file_name = strtok(file_name, "\n");
        
        FILE *file;
        file = fopen(file_name, "r");
        
        char *first_line, *content, *p, URL[51], title[51];
        int checksum, length, acc;

        first_line = malloc(101 * sizeof(char));

        fgets(first_line, 101, file);
        first_line = strtok(first_line, "\n");

        /// URL

        p = strtok(first_line, " ");
        strcpy(URL, p);

        /// lungime

        p = strtok(NULL, " ");
        length = atoi(p);

        /// acc

        p = strtok(NULL, " ");
        acc = atoi(p);

        /// checksum

        p = strtok(NULL, " ");
        checksum = atoi(p);

        /// formare continut prin citirea linie cu linie

        content = malloc((length + 1) * sizeof(char));
        content[0] = '\0';

        char *line;
        line = malloc(101 * sizeof(char));

        while (fgets(line, 101, file)) {

            strcat(content, line);
        }

        /// titlu

        strncpy(title, strstr(content, "<title>") + 7, 49);
        title[49] = '\0';
        title[strstr(title, "</title") - title] = '\0';

        /// alocare memorie in plus, daca este necesara

        if(file_no >= A_size) {

            add_size(&A, &A_size);
        }

        /// adaugarea componentelor determinate anterior in structura

        add(A, content, URL, title, checksum, length, acc, file_no);

        free(line);
        free(content);
        free(first_line);
        fclose(file);
    }

    /// afisarea site-urilor 

    int i = 0;
    for (; i <= file_no; i++) {

        printf("%s %d %s\n", A[i].URL, A[i].acc, A[i].title);
        free(A[i].content);
    }

    /// eliberarea memoriei

    free(A);
    free(file_name);
    fclose(master_file);

    return 0;
}
