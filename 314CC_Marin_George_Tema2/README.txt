Detalii valabile pentru toate task-urile:

Am utilizat o structura de tip site, ce ne permite sa stocam:

- continut (char *, ce va fi, ulterior, alocat dinamic)
- checksum, length, acc (int, avand urmatoarele semnificatii: 
	* checksum
	* lungime <dimensiune in octeti>
	* numarul de accesari)
- URL, title (char *, cu o dimensiune stabilita inca din incipit, conform cerintei)

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Task 1
~ 3h


Principiul solutiei este urmatorul:

1. Citirea site-urilor si adaugarea lor in memorie:
	- citirea din master.txt a numelor fisierelor
	- deschiderea fisierelor si citirea continuturilor
	- formatarea continuturilor in vederea adaugarii datelor in structura
		* cu eventualele alocari de memorie suplimentare
	- eliberarea memoriei pentru char* utilizate in citire si inchiderea fisierelor
2. Afisarea site-urilor si eliberarea memoriei (initial A[i].content, ulterior A).

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Task 2
~ 4h 


Principiul soultiei este urmatorul:

1. Citirea site-urilor si adaugarea lor in memorie.
	- citirea din master.txt a numelor fisierelor
	- deschiderea fisierelor si citirea continuturilor
	- formatarea continuturilor in vederea adaugarii datelor in structura
		* cu eventualele alocari de memorie suplimentare
	- eliberarea memoriei pentru char* utilizate in citire si inchiderea fisierelor
2. Determinarea selectiei de cautare.
3. Sortarea site-urilor care corespund selectiei de cautare.
	- par_start memoreaza un pointer catre inceputul paragrafului (fix dupa tag-ul de
		deschidere a acestuia)
	- conditiile aditionale din if-uri (check-urile) verifica daca ale lui p cuvinte sunt 
		prezente independent sau fac parte din alte cuvinte ("ion" in "avion")
	- pe masura ce sunt gasite site-uri care corespund selectiei, URL-urile sunt
		adaugate in URL_list si checked[i] memoreaza faptul ca site-ul cu
		indicele i a fost deja verificat
	- sortarea acestora este realizata printr-un bubble sort putin eficientizat,
		avand ca decizie de sortare functia cmp_task2, ce corespunde
		criteriilor task-ului 2 (continut si accesari)
4. Afisarea site-urilor mentionate si eliberarea memoriei (initial A[i].content, ulterior A).

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Task 3
~ 4h

Principiul solutiei este urmatorul:

1. Citirea site-urilor si adaugarea lor in memorie.
	- citirea din master.txt a numelor fisierelor
	- deschiderea fisierelor si citirea continuturilor
	- formatarea continuturilor in vederea adaugarii datelor in structura
		* cu eventualele alocari de memorie suplimentare
2. Determinarea selectiei de cautare (cu secventa si cuvant de exclus, daca acestea exista).
	- se verifica prezenta secventei si a cuvantului de exclus si, daca exista, sunt
		eliminate din selectia de cautare (le vom trata separat)
3. Sortarea site-urilor care corespund selectiei de cautare.
	- par_start memoreaza un pointer catre inceputul paragrafului (fix dupa tag-ul de
		deschidere a acestuia)
	- conditiile aditionale din if-uri (check-urile) verifica daca ale lui p cuvinte (sau secventa
		de cautat) sunt prezente independent sau fac parte din alte 
		cuvinte ("ion" in "avion") si, in cazul in care exista cuvant de exclus,
		verifica daca nu este prezent (sau nu trece de vreuna dintre verificari,
		in sensul in care face parte din alte cuvinte, de exemplu)
	- pe masura ce sunt gasite site-uri care corespund selectiei, URL-urile sunt
		adaugate in URL_list si checked[i] memoreaza faptul ca site-ul cu
		indicele i a fost deja verificat
	- sortarea acestora este realizata printr-un bubble sort putin eficientizat,
		avand ca decizie de sortare functia cmp_task3, ce corespunde
		criteriilor task-ului 3 (accesari)
4. Afisarea site-urilor mentionate si eliberarea memoriei (initial A[i].content, ulterior A).

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Task 4
~ 3h


Principiul solutiei este urmatorul:

1. Citirea site-urilor si adaugarea lor in memorie.
	- citirea din master.txt a numelor fisierelor
	- deschiderea fisierelor si citirea continuturilor
	- formatarea continuturilor in vederea adaugarii datelor in structura
		* cu eventualele alocari de memorie suplimentare
	- eliberarea memoriei pentru char* utilizate in citire si inchiderea fisierelor
2. Verificarea site-urilor si determinarea indicilor.
	- se compara liniile citite (URL-urile primite cu A[i].URL, in vederea identificarii
		indicilor site-urilor pentru calcularea checksum-urilor)
3. Calcularea checksum-ului pentru fiecare dintre site-urile gasite.
	- pentru fiecare caracter din A[i].content, se realizeaza o shiftare
		circulara (la stanga sau dreapta) si, intre toate rezultatele
		se aplica ^. checksum-ul rezultat este comparat cu cel
		primit si este afisat un mesaj in conformitate cu cerinta
		enuntului
4. Afisarea verificarilor si eliberarea memoriei (initial A[i].content, ulterior A).

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Task 5
~ 8h


Principiul solutiei este urmatorul:

Exista 4 pagini - first_page (1), search_page (2), search_query (3), print_page(4), pe fiecare
adaugandu-se legenda. Trecerea de pe o pagina pe alta se poate face conform enuntului:

1 -> 2
2 -> 3
3 -> 4, 3 -> 2
4 -> 3

"first_page" contine titlul numele motorului de cautare si ofera 2 optiuni:
	- cautare (C)
	- iesire (Q)

"search_page" permite introducerea selectiei de cautare si alegerea tipului
de cautare (simpla <S> sau avansata <A>), dupa apasarea tastei "Enter", moment in
care se poate si selecta iesirea din program (Q).

"search_query" afiseaza lista site-urilor care corespund selectiei de cautare, iar
ciclarea prin pagina de rezultate se face cu ajutorul tastelor KEY_UP si KEY_DOWN
Alegerea site-ului de afisat se realizeaza prin apasarea tastei "ENTER", ce determina
aparitia paginii "print_page", ce ilustreaza pagina html (colorata, daca este cazul).

Filtrarea site-urilor este realizata prin intermediul algoritmului de la task3, 
caci acesta poate functiona si fara secventa sau cuvant de exclus. Am considerat
ca este o metoda mai usor de implementat, utilizandu-ne doar de cmp_task2
pentru sortarea simpla (aferenta task2).

Culorile sunt gestionate in "print_page".

1. Citirea site-urilor si adaugarea lor in memorie este identica task-urilor anterioare.
2. Am preferat initializarea ferestrei de ncurses prin apelul functiei "first_page",
	si am trimis diferiti parametrii pentru a usura lucrul cu datele.
3. Paginile urmatoare apar ca apelari ale functiilor corespunzatoare.
4. Eliberarea memoriei (initial A[i].content, ulterior A).