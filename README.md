# Algorithms-Data-Structures-Final-Project
Obiettivo del progetto è realizzare un "graph ranker", ovvero un programma che dati in input n grafi e k numero di grafi migliori (non necessariamente vengono
prima inseriti tutti i grafi e alla fine viene chiesto il numero di grafi migliori in quanto i due comandi sono indipendenti e posso essere digitati in qualunque momento)
stampa i k migliori grafi con somma dei cammini minimi, dalla sorgente verso qualunque altro nodo, più piccola possibile.
Il file base.txt, presente nella root principale, fornisce un semplice esempio del funzionamento basilare del programma; per test più complessi, che stressano sia
la capacità del programma di fare un uso ottimizzato della memoria che quella di terminare l'esecuzione in un tempo ragionevole, si raccomanda di generare casi test di più
specifici tramite i due generatori in python caricati nella cartella testcase_generator.

Per eseguire il programma si consigliano i seguenti comandi (ovviamente bisogna avere un compilatore C installato sulla propria macchina):
1. gcc -o exec main.c (compilazione)
2. ./exec < base.txt (esecuzione su Linux, fortemente consigliata)

Per ulteriori chiarimenti in merito alle specifiche si rimanda al file "Presentazione Prova Finale 2021.pdf" presente anch'esso nella root principale.
