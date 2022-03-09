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

void add(struct site *A, char *content, char *URL, char *title, int checksum, int length, int acc, int i) {

    A[i].checksum = checksum;
    A[i].length = length;
    A[i].acc = acc;

    strcpy(A[i].URL, URL);
    strcpy(A[i].title, title);

    A[i].content = malloc((length + 1) * sizeof(char));
    strcpy(A[i].content, content);
}

int cmp_task2(char *A_content, int A_acc, char *B_content, int B_acc) {
    
    /// 0 = nu are nevoie de swap, 1 = are nevoie de swap

    int result = strcmp(A_content, B_content);

    if (result < 0) {

        return 0;
    }
        
    if (result > 0) {
        
        return 1;
    }

    if(A_acc > B_acc) {

        return 0;
    }

    return 1;
}

int cmp_task3(char *A_content, int A_acc, char *B_content, int B_acc) {

    if(A_acc > B_acc) {

        return 0;
    }

    return 1;
}

void my_sort(struct site *A, int list_size, char URL_list[][51], int acc_list[], int initial_index[], int (*cmp)(char *A_content, int A_acc, char *B_content, int B_acc)) {

    int i = 0, j;
    char *par_start_i, *par_start_j;

    for (; i < list_size - 1; i++) {

        par_start_i = strchr(strstr(A[initial_index[i]].content, "<p"), '>') + 1;

        /// izolarea continutului pentru comparare

        char par_i[strlen(par_start_i)];
        strcpy(par_i, par_start_i);
        par_i[strlen(par_i) - 12] = '\0';

        for (j = i + 1; j < list_size; j++) {
            
            /// copierea metodei anterioare

            par_start_j = strchr(strstr(A[initial_index[j]].content, "<p"), '>') + 1;

            char par_j[strlen(par_start_j)];
            strcpy(par_j, par_start_j);
            par_j[strlen(par_j) - 12] = '\0';
            
            if(cmp(par_start_i, acc_list[i], par_start_j, acc_list[j]) == 1) {

                char aux[51];
                strcpy(aux, URL_list[i]);
                strcpy(URL_list[i], URL_list[j]);
                strcpy(URL_list[j], aux);
                
                int int_aux = acc_list[i];
                acc_list[i] = acc_list[j];
                acc_list[j] = int_aux;

                int_aux = initial_index[i];
                initial_index[i] = initial_index[j];
                initial_index[j] = int_aux;
            }
        }

    }
}


/* functie pentru verificarea cuvintelor din 
cautare (daca nu fac parte din alte cuvinte) */

int check(char c) {

    if(c == ' '
    || (c >= '!' && c <= '/') 
    || (c >= ':' && c <= '@') 
    || (c >= '[' && c <= '`')
    || (c >= '{' && c <= '~')
    || c == '\n'
    ) {

        return 1;
    }

    return 0;
}

int main() {

    /// initializare structura

    struct site *A = init();
    int A_size = 3, file_no = -1, i = 0;
    
    FILE *master_file;
    master_file = fopen("master.txt", "r");

    char *file_name;
    file_name = malloc(31 * sizeof(char));

    /// citirea fisierelor din master + utilizarea lor

    while (fgets(file_name, 31, master_file)) {

        file_no++;
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


    /// determinarea selectiei de cautare

    char *line = malloc(sizeof(char)), c;
    int line_length = 0;

    while ((c = getchar())) {
        
        line[line_length] = c;
        line = realloc (line, (++line_length + 1) * sizeof(char));

        if (c == '\n' || c == '\0') {

            break;
        }
    }

    line[line_length] = '\0';

    /* checked = vectorul tine cont
    daca site-ul cu indicele i a fost deja verificat
    acc_list = vector cu numarul de accesari
    intial_index = pastreaza indicele intial al site-ului (din master)
    list_size = numarul de site-uri care corespund cautarii
    URL_list = vector cu URL-ul site-urilor care corespund cautarii */

    int checked[file_no + 1], acc_list[file_no + 1], 
        initial_index[file_no + 1], list_size = 0;
    char URL_list[file_no + 1][51], *p = strtok(line, " \n");

    memset(checked, 0, sizeof(checked));

    while (p) {

        for (i = 0; i <= file_no; i++) {

            char *par_start = strchr(strstr(A[i].content, "<p"), '>') + 1;

            /* se verifica daca ale lui p cuvinte sunt prezente 
            + nu fac parte dintr-un alt cuvant
            se adauga elemente in vectorii anterior mentionati,
            ce vor fi folositi pentru sortare */

            if (
            (strstr(par_start, p) != NULL 
            && check(strstr(par_start, p)[strlen(p)]) == 1 
            && check((strstr(par_start, p) - 1)[0]) == 1)

            &&
            checked[i] == 0) {

                checked[i] = 1;
                initial_index[list_size] = i;
                strcpy(URL_list[list_size], A[i].URL);
                acc_list[list_size++] = A[i].acc;
            }
        }
    
        p = strtok(NULL, " \n");
    }

    int (*cmps[2])(char*, int, char*, int) = {cmp_task2, cmp_task3};

    my_sort(A, list_size, URL_list, acc_list, initial_index, cmps[0]);

    /// afisarea site-urilor care corespund cautarii, sortate

    for (i = 0; i < list_size; i++) {

            puts(URL_list[i]);
    }

    /// eliberarea memoriei

    for (i = 0; i <= file_no; i++) {

        free(A[i].content);
    }

    free(A);
    free(line);
    free(file_name);
    fclose(master_file);

    return 0;
}
