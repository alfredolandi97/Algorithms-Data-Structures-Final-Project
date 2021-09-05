#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>

#define COMMANDMAXLENGTH 4000
#define STRINGMAXLENGTH 12
#define FIRST_DIM_HEAP_ARRAY 300000

typedef struct weight {
    unsigned long int positive_weight;
    struct weight *next;
    int node;
}weight;

typedef struct adiacency{
    unsigned long int dist_to_source;
    weight * list_of_weight;
}adiacency;

typedef struct heap_for_dijkstra{
    adiacency **heap_array;
    int heapsize;
}heap_for_dijkstra;

int power(int base, int exp){
    int res = 1;
    for(int i=0;i<exp;i++){
        res*=base;
    }

    return res;
}

int parseString(char *string, int dim){
    int res = 0;

    for(int i=0;i<dim;i++){
        res += power(10, dim-i-1)*(string[i]-'0');
    }

    return res;
}

void handleFirstLine(char *string, int *DoG, int *NoT){
    int i=0, counter=0;
    char tmp_string[STRINGMAXLENGTH];
    bool isDoG = true, quit = false;


    while (quit==false){
        if(string[i]!=',' && string[i]!=' ' && string[i]!='\n'){
            tmp_string[counter]=string[i];
            counter++;
        }else{
            if(isDoG){
                *DoG=parseString(tmp_string, counter);
            }else{
                *NoT=parseString(tmp_string, counter);
                quit=true;
            }
            isDoG=false;
            counter=0;
        }
        i++;
    }
}

void initialize_heap_for_dijkstra(heap_for_dijkstra* hfd, int dimension){
    hfd->heap_array=(adiacency **) malloc(sizeof(adiacency *)*dimension);
    assert(hfd->heap_array!=NULL);
    hfd->heapsize=dimension;
}

void refresh_heap_for_dijkstra(heap_for_dijkstra* hfd, int dimension){
    hfd->heapsize=dimension;
}

void MinHeapify_for_dijkstra(heap_for_dijkstra *hfd, int n){
    int l = 2*n;
    int r = 2*n+1;
    int posmax;
    if (l<hfd->heapsize && hfd->heap_array[l]->dist_to_source<hfd->heap_array[n]->dist_to_source){
        posmax=l;
    }else{
        posmax=n;
    }
    if(r<hfd->heapsize && hfd->heap_array[r]->dist_to_source<hfd->heap_array[n]->dist_to_source){
        posmax=r;
    }
    if(posmax!=n){
        //SWAP
        adiacency *tmp=hfd->heap_array[posmax];
        hfd->heap_array[posmax]=hfd->heap_array[n];
        hfd->heap_array[n]=tmp;

        MinHeapify_for_dijkstra(hfd, posmax);
    }
}

adiacency *cancellaMin_for_dijkstra(heap_for_dijkstra *hfd) {
    if (hfd->heapsize < 1) {
        return NULL;
    }

    adiacency *min = hfd->heap_array[0];
    hfd->heap_array[0] = hfd->heap_array[hfd->heapsize - 1];
    hfd->heapsize = hfd->heapsize - 1;
    MinHeapify_for_dijkstra(hfd, 0);
    return min;
}

unsigned long int dijkstra(adiacency *adiacency_list, int adiacency_list_dim, heap_for_dijkstra *hfd){
    adiacency *current_node;
    while(hfd->heapsize>0){
        current_node=cancellaMin_for_dijkstra(hfd);
        weight *current_weight=current_node->list_of_weight;
        while(current_weight!=NULL){
            if(((adiacency_list[current_weight->node].dist_to_source)>(current_node->dist_to_source+current_weight->positive_weight))
               && current_node->dist_to_source!=ULONG_MAX && current_weight->positive_weight!=0){
                adiacency_list[current_weight->node].dist_to_source=current_node->dist_to_source+current_weight->positive_weight;
                MinHeapify_for_dijkstra(hfd, current_weight->node);
            }
            current_weight=current_weight->next;
        }
    }

    unsigned long int res = 0;
    for(int i=0;i<adiacency_list_dim;i++){
        if(adiacency_list[i].dist_to_source==ULONG_MAX){
            res+=0;
        }else{
            res+=adiacency_list[i].dist_to_source;
        }
    }

    return res;
}

typedef struct score {
    int pos;
    unsigned long int score_number;
}score;

typedef struct heap_for_score{
    score *score_array;
    int tmp_heapsize;
    int heapsize;
    int length;
}heap_for_score;

void parseIntegerIntoString(int num, char *string){
    int i=0, k=0;
    char tmp[STRINGMAXLENGTH];

    if(num==0){
        string[0]='0';
        string[1]='\0';
    }else{
        while(num>0){
            tmp[i]=num%10+'0';
            i++;
            num=num/10;
        }

        for(int j=i-1;j>=0;j--){
            string[k]=tmp[j];
            k++;
        }

        string[i]='\0';
    }

}

void initialize_heap_for_score(heap_for_score *hfs){
    hfs->score_array = (score *)malloc(sizeof(score)*FIRST_DIM_HEAP_ARRAY);
    assert(hfs->score_array!=NULL);
    hfs->length=FIRST_DIM_HEAP_ARRAY;
    hfs->heapsize=0;
}

void make_bigger_heap_array(heap_for_score *hfs){
    hfs->score_array=(score *)realloc(hfs->score_array, 2*hfs->length*sizeof(struct score));
    hfs->length=2*hfs->length;
}

void swap_for_score(score *a, score *b){
    score tmp;
    tmp.score_number=a->score_number;
    tmp.pos=a->pos;
    a->score_number=b->score_number;
    a->pos=b->pos;
    b->score_number=tmp.score_number;
    b->pos=tmp.pos;
}

void MinHeapify_for_score(heap_for_score *hfs, int n){
    int l = 2*n;
    int r = 2*n+1;
    int posmax;
    if (l<hfs->tmp_heapsize && (hfs->score_array[l].score_number < hfs->score_array[n].score_number ||
                                (hfs->score_array[l].score_number == hfs->score_array[n].score_number &&
                                 hfs->score_array[l].pos < hfs->score_array[n].pos))){
        posmax=l;
    }else{
        posmax=n;
    }
    if(r<hfs->tmp_heapsize && (hfs->score_array[r].score_number < hfs->score_array[n].score_number ||
                               (hfs->score_array[r].score_number == hfs->score_array[n].score_number &&
                                hfs->score_array[r].pos < hfs->score_array[n].pos))){
        posmax=r;
    }
    if(posmax!=n){
        //SWAP
        swap_for_score(&hfs->score_array[posmax], &hfs->score_array[n]);

        MinHeapify_for_score(hfs, posmax);
    }
}

void build_min_heap_for_score(heap_for_score *hfs){
    for(int i=hfs->heapsize/2;i>=0;i--){
        MinHeapify_for_score(hfs, i);
    }
}

void refresh_heap_for_score(heap_for_score *hfs){
    hfs->tmp_heapsize=hfs->heapsize;
    build_min_heap_for_score(hfs);
}

int cancellaMin_for_score(heap_for_score *hfs) {
    if (hfs->tmp_heapsize < 1) {
        return -1;
    }

    int min = hfs->score_array[0].pos;
    swap_for_score(&hfs->score_array[0], &hfs->score_array[hfs->tmp_heapsize-1]);
    hfs->tmp_heapsize = hfs->tmp_heapsize - 1;
    MinHeapify_for_score(hfs, 0);
    return min;
}

void handleTopK(heap_for_score *hfs, int NoT, bool first_TopK){
    if(first_TopK==false){
        refresh_heap_for_score(hfs);
    }
    int j=0, pos_score;
    char output_string[STRINGMAXLENGTH];
    hfs->tmp_heapsize=hfs->heapsize;
    while(j<NoT && j<hfs->heapsize){
        pos_score = cancellaMin_for_score(hfs);
        parseIntegerIntoString(pos_score, output_string);
        fputs(output_string, stdout);
        if(j<hfs->heapsize-1){
            fputc(' ', stdout);
        }
        j++;
    }
    fputc('\n', stdout);
}

int main() {
    int DoG; //Dimension of Graph
    int NoT; //Number of Tops
    char command[COMMANDMAXLENGTH];
    char *quit_string;
    bool quit = false, first_use_of_dijkstra=true;
    int i, counter=0, j=0, weight_counter, parsed_weight;
    char tmp_string[STRINGMAXLENGTH];
    heap_for_dijkstra hfd;
    hfd.heap_array=NULL;

    quit_string = fgets(command,COMMANDMAXLENGTH, stdin);
    handleFirstLine(command, &DoG, &NoT);
    adiacency adiacency_list[DoG];

    weight *current_weight_element;

    int graphs_counter=0;

    unsigned long int current_sp;
    heap_for_score hfs;
    hfs.score_array=NULL;
    bool first_score_elemet=true, first_TopK=true;

    while(quit==false){
        quit_string = fgets(command,COMMANDMAXLENGTH, stdin);
        if(quit_string==NULL){
            quit=true;
            command[0]='q';
        }
        if(command[0]=='A' && command!=NULL){
            if(first_use_of_dijkstra==true){
                initialize_heap_for_dijkstra(&hfd, DoG);
            }else{
                refresh_heap_for_dijkstra(&hfd, DoG);
            }
            while(j<DoG) {
                quit_string = fgets(command, COMMANDMAXLENGTH, stdin);
                weight_counter=0;
                if(graphs_counter==0){
                    adiacency_list[j].list_of_weight=NULL;
                }
                i=0;
                if(j==0) {
                    adiacency_list[0].dist_to_source = 0;
                    hfd.heap_array[0] = &adiacency_list[0];
                }else {
                    adiacency_list[j].dist_to_source = ULONG_MAX;
                    hfd.heap_array[j] = &adiacency_list[j];
                }
                while (weight_counter<DoG) {
                    if (command[i] != ',' && command[i] != ' ' && command[i] != '\n') {
                        tmp_string[counter] = command[i];
                        counter++;
                    } else {
                        if(weight_counter!=j){
                            parsed_weight = parseString(tmp_string, counter);
                            if(graphs_counter==0){
                                weight *weight_element = (weight *) malloc(sizeof(weight));
                                weight_element->positive_weight = parsed_weight;
                                weight_element->node = weight_counter;
                                weight_element->next = adiacency_list[j].list_of_weight;
                                adiacency_list[j].list_of_weight = weight_element;
                            }else{
                                if(weight_counter==0 || (j==0 && weight_counter==1)){
                                    current_weight_element= adiacency_list[j].list_of_weight;
                                }
                                current_weight_element->positive_weight=parsed_weight;
                                current_weight_element->node=weight_counter;
                                current_weight_element=current_weight_element->next;
                            }
                        }
                        weight_counter++;
                        counter = 0;
                    }
                    i++;
                }
                j++;
            }
            j=0;
            current_sp = dijkstra(adiacency_list, DoG, &hfd);
            //fprintf() per testare dijkstra
            //fprintf(stdout, "graph %d = %lu\n", graphs_counter, current_sp);
            first_use_of_dijkstra=false;
            if(hfs.score_array==NULL){
                initialize_heap_for_score(&hfs);
            }
            if(graphs_counter>hfs.length){
                make_bigger_heap_array(&hfs);
            }
            if(first_score_elemet==false) {
                if (current_sp < hfs.score_array[0].score_number) {
                    hfs.score_array[hfs.heapsize].score_number = hfs.score_array[0].score_number;
                    hfs.score_array[hfs.heapsize].pos = hfs.score_array[0].pos;
                    hfs.score_array[0].score_number = current_sp;
                    hfs.score_array[0].pos = graphs_counter;
                } else {
                    hfs.score_array[hfs.heapsize].score_number = current_sp;
                    hfs.score_array[hfs.heapsize].pos = graphs_counter;
                }
                hfs.heapsize++;
            }else {
                hfs.score_array[0].score_number = current_sp;
                hfs.score_array[0].pos = graphs_counter;
                first_score_elemet = false;
                hfs.heapsize++;
            }
            graphs_counter++;
        }else if(command[0]=='T' && command!=NULL){
            //Da commentare se si vuole testare solo il dijkstra
            handleTopK(&hfs,NoT, first_TopK);
            if(first_TopK==true){
                first_TopK=false;
            }
        }else{
        }
    }

    return 0;
}