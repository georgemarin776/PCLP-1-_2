#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

enum color{white = 7, black = 0, red = 1, green = 2, blue = 4, yellow = 3};

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

void my_sort(struct site *A, int list_size, char URL_list[][51], 
int acc_list[], int initial_index[], 
    int (*cmp)(char *A_content, int A_acc, char *B_content, int B_acc)) {

    int i = 0, j;
    char *par_start_i, *par_start_j;

    for (; i < list_size - 1; i++) {

        par_start_i = strchr(strstr(A[initial_index[i]].content, "<p"), '>') 
            + 1;

        /// izolarea continutului pentru comparare

        char par_i[strlen(par_start_i)];
        strcpy(par_i, par_start_i);
        par_i[strlen(par_i) - 12] = '\0';

        for (j = i + 1; j < list_size; j++) {

            /// copierea metodei anterioare

            par_start_j = strchr(strstr(A[initial_index[j]].content, "<p"),
                '>') + 1;

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
    || c == '\n' || c == '\0'
    ) {

        return 1;
    }

    return 0;
}

void search_page(struct site *A, int file_no);
void search_query(struct site *A, char URL_list[][51], int initial_index[],
    int list_size, int file_no, char aux[]);

void print_page(struct site *A, char URL_list[][51], char  URL[],
    int initial_index[], int list_size, int file_no, char aux[]) {

    initscr();
    cbreak();
    noecho();
    clear();

    int y_Max, x_Max;
    int i = 0, c;

	getmaxyx(stdscr, y_Max, x_Max);

    /// adaugare legenda

    for (; i < x_Max; i++) {

		mvprintw(y_Max - 6, i, "#");
    }

    mvprintw(y_Max - 5, 0, "Press:\n \"B\" to return to the searching tab.");
    mvprintw(y_Max - 3, 0, " \"Q\" to quit the program.");

    /// determinarea indicelui site-ului de afisat

    for (i = 0; i <= file_no; i++) {

        if(strstr(A[i].URL, URL) != NULL)
            break;
    }

    ///afisarea titlului bolduit

    attron(A_BOLD);

    mvprintw(0, 0, A[i].title);
    
    attroff(A_BOLD);

    ///determinarea paragrafului, pentru verificarea stilului

    char *par_start_cpy = strchr(strstr(A[i].content, "<p"), '>') + 1, 
        *par_start = malloc((strlen(par_start_cpy) + 1) * sizeof(char));

    strcpy(par_start, par_start_cpy);

    par_start[strlen(par_start) - 12] = '\0';

    if (strstr(A[i].content, "<p style=") != NULL) {

        char *style_start = strstr(A[i].content, "<p style=") + 9;
        enum color fg = white, bg = black;

        if(strstr(style_start, "\"color") != NULL) {
            
            switch(style_start[7]) {

            case 'w':
                fg = white;
                break;
            case 'r':
                fg = red;
                break;
            case 'g':
                fg = green;
                break;
            case 'y':
                fg = yellow;
                break;
            case 'b':
                if (style_start[9] == 'u')
                    fg = blue;
                else
                    fg = black;
            }
        }

        if(strstr(style_start, "background-color") != NULL) {

            char *bg_start = strstr(style_start, "background-color") + 17;
            switch(bg_start[0]) {

            case 'w':
                bg = white;
                break;
            case 'r':
                bg = red;
                break;
            case 'g':
                bg = green;
                break;
            case 'y':
                bg = yellow;
                break;
            case 'b':
                if (bg_start[2] == 'u')
                    bg = blue;
                else
                    bg = black;
            }
        }
        
        /// scrierea paragrafului colorat

        start_color();

        init_pair(1, fg, bg);

        attron(COLOR_PAIR(1));

        mvprintw(2, 0, par_start);

        attroff(COLOR_PAIR(1));
    }

    else {

        mvprintw(2, 0, par_start);
    }

    /// continuarea afisarii - apasarea tastei 'B' sau 'Q'

    while((c = getch())) {

        if (c == 'Q') {
                
            endwin();
            return;
        }

        if (c == 'B') {
            
            search_query(A, URL_list, initial_index, list_size, file_no, aux);
            endwin();
            break;
        }
    }

}

void search_query(struct site *A, char URL_list[][51], int initial_index[], int list_size, int file_no, char aux[]) {

    initscr();
    cbreak();
    noecho();
    clear();
    keypad(stdscr, true);

    int y_Max, x_Max;
    int i;

	getmaxyx(stdscr, y_Max, x_Max);
    
    ///adaugare legenda si chenar pentru cautare

    for (i = 0; i < x_Max; i++) {

	    mvprintw(y_Max - 6, i, "#");
    } 

    for (i = 3; i < x_Max - 3; i++) {

        mvprintw(1, i, "*");
    }
			
		
	for (i = 3; i < x_Max - 3; i++) {

        mvprintw(3, i, "*");
    }
	
	mvprintw(2, 3, "|");
	mvprintw(2, x_Max - 4, "|");
    mvprintw(2, 4, "%s", aux);

    mvprintw(y_Max - 5, 0, "Press:\n \"B\" to return to the searching tab.");
    mvprintw(y_Max - 3, 0, " \"Q\" to quit the program.");

    ///scrierea meniului + highlight pe site-ul curent

    int c, choice, highlight = 0, ok = 1;

    while(list_size !=0 && ok == 1) {

        for (i = 0; i < list_size; i++) {

            if(i == highlight) {

                attron(A_REVERSE);
            }

            mvprintw(5 + i, 4, "%s @ %s", A[initial_index[i]].title, 
                URL_list[i]);
            attroff(A_REVERSE);
        }

        ///ciclarea prin meniu si alegerea optiunilor

        choice = getch();

        switch(choice) {
                
            case KEY_UP:
                if (highlight > 0)
                    highlight--;
                break;
            case KEY_DOWN:
                if (highlight < list_size - 1)
                    highlight++;
                break;
            case 'Q':
                endwin();
                return;
            case 'B':
                clear();
                search_page(A, file_no);
                break;
            case 10:
                print_page(A, URL_list, URL_list[highlight], 
                    initial_index, list_size, file_no, aux);
                ok = 0;
                break;
            default:
                break;
        }
                
    }

    /// daca niciun site nu corespunde cautarii:

    if (list_size == 0) {

         mvprintw(5, 4, "No sites found.");
         while((c = getch())) {

            if (c == 'Q') {
                
                endwin();
                return;
            }

            if (c == 'B') {

                search_page(A, file_no);
                endwin();
                break;
            }
        }
    }

}

void search_page(struct site *A, int file_no) {

    initscr();
    cbreak();
    noecho();
    clear();

    int y_Max, x_Max;

	getmaxyx(stdscr, y_Max, x_Max);

    ///adaugare legenda si chenar pentru cautare

    char *line = malloc(1 * sizeof(char)), c;
    int line_length = 0, i;
    char *seq, *exclude;
    int exclude_ok = 0, seq_ok = 0;

    for (i = 0; i < x_Max; i++) {

			mvprintw(y_Max - 6, i, "#");
    }   

	mvprintw(y_Max - 5, 0, "Press:\n \"Enter\" to set the search query.");
	mvprintw(y_Max - 3, 0, " \"S\" for a simple search.\n \"A\" for an advanced search");
	mvprintw(y_Max - 1, 0, " \"Q\" to quit the program.");
		
	for (i = 3; i < x_Max - 3; i++) {

        mvprintw(y_Max/2 - 1, i, "*");
    }
			
		
	for (i = 3; i < x_Max - 3; i++) {

        mvprintw(y_Max/2 + 1, i, "*");
    }
			

	mvprintw(y_Max/2, 3, "|");
	mvprintw(y_Max/2, x_Max - 4, "|");
		
	move(y_Max/2, 4);


    /// adaugare secventa pentru cautare

    while ((c = getch())) {
        
        if(c == 127) {
            
            line[--line_length] = '\0';
            mvprintw(y_Max/2, 4 + line_length, " ");
            move(y_Max/2, 4 + line_length);

        }

        else {

		    if (c == 10) {
                
                line[line_length] = '\0';
                mvprintw(y_Max/2 + 4, 4, 
                    "Input the search type (\"S\" / \"A\"): ");
			    break;
		    }

            else {
                
                line[line_length] = c;
		        line = realloc (line, (line_length + 2) * sizeof(char));
                mvprintw(y_Max/2, 4 + line_length++, "%c", c);
            }
            
        }
		
	}

    line[line_length] = '\0';

    char *aux = malloc((line_length + 1) * sizeof(char));
        
    strcpy(aux, line);

    /* Algoritmul de cautare este cel utilizat la task 3,
    caci poate functiona si fara secventa sau cuvant de eliminat. */

    int ok = 1;

	while((c = getch()) && ok == 1) {

        if (c == 'Q') {

		    endwin();
            return;
	    }
		
        if (c == 'A' || c == 'S') {
                
            if(strchr(line, '"') != NULL) {
            
                seq_ok = 1;
                int mark_length = strchr(strchr(line, '"') + 1, '"') - strchr(line, '"') - 1;
                seq = malloc((mark_length + 1) * sizeof(char));
                strncpy(seq, strchr(line, '"') + 1, mark_length);
                seq[mark_length] = '\0';
                strcpy(strchr(line, '"'), strchr(strchr(line, '"') + 1, '"') + 1);
            }

            line_length = strlen(line);

            if(strchr(line, '-') != NULL) {

                exclude_ok = 1;
                int i, exclude_length = 0;

                for(i = strchr(line, '-') - line + 1; i < line_length && line[i] != ' '; i++) {
                        
                    exclude_length ++;
                }

                exclude_length--;
                exclude = malloc((exclude_length + 1) * sizeof(char));
                strncpy(exclude, strchr(line, '-') + 1, exclude_length);
                exclude[exclude_length] = '\0';
                strcpy(strchr(line, '-'), strchr(line, '-') + exclude_length + 1);
            }

            line_length = strlen(line);
        
            int checked[file_no + 1], acc_list[file_no + 1],  initial_index[file_no + 1], list_size = 0;
            char URL_list[file_no + 1][51];

            memset(checked, 0, sizeof(checked));

            for (i = 0; i <= file_no && seq_ok == 1; i++) {

                char *par_start = strchr(strstr(A[i].content, "<p"), '>') + 1, 
                    *seq_exists = strstr(par_start, seq);

                if (seq_exists != NULL && checked[i] == 0) {
                
                    if (check(seq_exists[strlen(seq)]) == 1 && check((seq_exists - 1)[0]) == 1) {

                        if (exclude_ok == 1) {
                        
                            char *exclude_exists = strstr(par_start, exclude);

                            if (exclude_exists != NULL) {

                                if (check(exclude_exists[strlen(exclude)]) == 0 || check((exclude_exists - 1)[0]) == 0) {

                                    checked[i] = 1;
                                    initial_index[list_size] = i;
                                    strcpy(URL_list[list_size], A[i].URL);
                                    acc_list[list_size++] = A[i].acc;
                                }
                            }

                            else {

                                checked[i] = 1;
                                initial_index[list_size] = i;
                                strcpy(URL_list[list_size], A[i].URL);
                                acc_list[list_size++] = A[i].acc;
                            }
                        }

                        else {

                            checked[i] = 1;
                            initial_index[list_size] = i;
                            strcpy(URL_list[list_size], A[i].URL);
                            acc_list[list_size++] = A[i].acc;
                        }
                        
                    }
                
                }

            }

            char *p = strtok(line, " \n");

            while (p) {
            
                for (i = 0; i <= file_no; i++) {

                    char *par_start = strchr(strstr(A[i].content, "<p"), 
                        '>') + 1, 
                        *p_exists = strstr(par_start, p);

                    if (p_exists != NULL && checked[i] == 0) {
                    
                        if (check(p_exists[strlen(p)]) == 1 &&
                            check((p_exists - 1)[0]) == 1) {

                            if (exclude_ok == 1) {
                        
                                char *exclude_exists = 
                                    strstr(par_start, exclude);

                                if (exclude_exists != NULL) {

                                    if (check(exclude_exists
                                    [strlen(exclude)]) == 0 ||
                                        check((exclude_exists - 1)[0]) == 0) {

                                        checked[i] = 1;
                                        initial_index[list_size] = i;
                                        strcpy(URL_list[list_size], A[i].URL);
                                        acc_list[list_size++] = A[i].acc;
                                    }
                                }

                                else {

                                    checked[i] = 1;
                                    initial_index[list_size] = i;
                                    strcpy(URL_list[list_size], A[i].URL);
                                    acc_list[list_size++] = A[i].acc;

                                }
                            }

                            else {

                                checked[i] = 1;
                                initial_index[list_size] = i;
                                strcpy(URL_list[list_size], A[i].URL);
                                acc_list[list_size++] = A[i].acc;
                            }       
                        
                        }
                
                    }
                }

                    p = strtok(NULL, " \n");
            }

            int (*cmps[2])(char*, int, char*, int) = {cmp_task2, cmp_task3};

            /* Cautarea simpla foloseste cautarea de la task2, iar
            cea avansata pe cea de la task3. */

            if(c == 'S') {

                my_sort(A, list_size, URL_list, 
                    acc_list, initial_index, cmps[0]);
            }

            else {

                my_sort(A, list_size, URL_list, 
                    acc_list, initial_index, cmps[1]);
            }

            search_query(A, URL_list, initial_index, list_size, file_no, aux);
            ok = 0;
            break;
        }
    }

    endwin();
    
    free(aux);
    free(line);
}

void first_page(struct site *A, int file_no) {

    initscr();
	cbreak();
	noecho();
	
	int y_Max, x_Max;
    int i;

	getmaxyx(stdscr, y_Max, x_Max);
	
    /// adaugarea legendei si a titlului

	mvprintw(y_Max/2, x_Max/2 - 9, "Browserul lui Biju");
	
	for (i = 0; i < x_Max; i++) {

		mvprintw(y_Max - 6, i, "#");
    }

	mvprintw(y_Max-5, 0, 
        "Press:\n \"C\" to initiate a search.\n \"Q\" to quit the program.");
	
	move(y_Max/2 + 1, x_Max/2);
	
	
	int first_option;

    /// selectarea optiunilor
    
    while ((first_option = getch())) {

        if (first_option == 'Q') {

            endwin();
            return;
        }
    
        if (first_option == 'C') {

            search_page(A, file_no);
            return;
        }
    }

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

    /// initializare primei pagini

    first_page(A, file_no);

    /// eliberarea memoriei

    for(i = 0; i <= file_no; i++) {

        free(A[i].content);
    }

    free(A);
    fclose(master_file);
    return 0;
}