#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>

#define COMMANDMAXLENGTH 4000
#define STRINGMAXLENGTH 12

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

int left(int n){
    return n*2+1;
}

int right(int n){
    return n*2+2;
}

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

void initialize_adjacency_list(adiacency **adjacency_list, int DoG){
    *adjacency_list = (adiacency *)malloc(DoG*sizeof(adiacency));
    assert(adjacency_list!=NULL);
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
    int l = left(n);
    int r = right(n);
    int posmin = n;

    if (l<hfd->heapsize && hfd->heap_array[l]->dist_to_source<hfd->heap_array[n]->dist_to_source){
        posmin=l;
    }

    if(r<hfd->heapsize && hfd->heap_array[r]->dist_to_source<hfd->heap_array[posmin]->dist_to_source){
        posmin=r;
    }
    if(posmin!=n){
        //SWAP
        adiacency *tmp=hfd->heap_array[posmin];
        hfd->heap_array[posmin]=hfd->heap_array[n];
        hfd->heap_array[n]=tmp;

        MinHeapify_for_dijkstra(hfd, posmin);
    }
}

void build_min_heap_for_dijkstra(heap_for_dijkstra *hfd){
    for(int i=(hfd->heapsize/2)-1;i>=0;i--){
        MinHeapify_for_dijkstra(hfd, i);
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
    while(hfd->heapsize!=0){
        current_node=cancellaMin_for_dijkstra(hfd);
        weight *current_weight=current_node->list_of_weight;
        while(current_weight!=NULL){
            if(((adiacency_list[current_weight->node].dist_to_source)>(current_node->dist_to_source+current_weight->positive_weight))
               && current_node->dist_to_source!=ULONG_MAX && current_weight->positive_weight!=0){
                adiacency_list[current_weight->node].dist_to_source=current_node->dist_to_source+current_weight->positive_weight;
                build_min_heap_for_dijkstra(hfd);

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
    int heapsize;
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

void initialize_heap_for_score(heap_for_score *hfs, int NoT){
    hfs->score_array = (score *)malloc(sizeof(score)*NoT);
    assert(hfs->score_array!=NULL);
    hfs->heapsize=0;
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

void MaxHeapify_for_score(heap_for_score *hfs, int n){
    int l = left(n);
    int r = right(n);
    int posmax = n;
    if (l<hfs->heapsize && (hfs->score_array[l].score_number > hfs->score_array[n].score_number ||
                                (hfs->score_array[l].score_number == hfs->score_array[n].score_number &&
                                 hfs->score_array[l].pos > hfs->score_array[n].pos))){
        posmax=l;
    }
    if(r<hfs->heapsize && (hfs->score_array[r].score_number > hfs->score_array[posmax].score_number ||
                               (hfs->score_array[r].score_number == hfs->score_array[posmax].score_number &&
                                hfs->score_array[r].pos > hfs->score_array[posmax].pos))){
        posmax=r;
    }
    if(posmax!=n){
        //SWAP
        swap_for_score(&hfs->score_array[posmax], &hfs->score_array[n]);

        MaxHeapify_for_score(hfs, posmax);
    }
}

int cancellaMax_for_score(heap_for_score *hfs) {
    if (hfs->heapsize < 1) {
        return INT_MIN;
    }

    int max = hfs->score_array[0].pos;
    swap_for_score(&hfs->score_array[0], &hfs->score_array[hfs->heapsize-1]);
    hfs->heapsize = hfs->heapsize - 1;
    MaxHeapify_for_score(hfs, 0);
    return max;
}

int parent(int i){
    return (i-1)/2;
}

void add_element_to_hfs(heap_for_score *hfs, unsigned long int score_number, int pos){
    int i = hfs->heapsize;
    hfs->score_array[hfs->heapsize].score_number=score_number;
    hfs->score_array[hfs->heapsize].pos=pos;
    hfs->heapsize++;
    while(i!=0 &&  (hfs->score_array[parent(i)].score_number < hfs->score_array[i].score_number ||
                    (hfs->score_array[parent(i)].score_number == hfs->score_array[i].score_number &&
                     hfs->score_array[parent(i)].pos < hfs->score_array[parent(i)].pos))){
        swap_for_score(&hfs->score_array[i], &hfs->score_array[parent(i)]);
        i=parent(i);

    }
}
void handleTopK(heap_for_score *hfs, int NoT, int graphs_counter){
    int j=0;
    char output_string[STRINGMAXLENGTH];
    if(graphs_counter!=0){
        while(j<NoT && j<hfs->heapsize){
            parseIntegerIntoString(hfs->score_array[j].pos, output_string);
            fputs(output_string, stdout);
            if(j<hfs->heapsize-1){
                fputc(' ', stdout);
            }
            j++;
        }
        fputc('\n', stdout);
    }else{
        fputc('\n', stdout);
    }
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
    adiacency *adiacency_list=NULL;

    weight *current_weight_element;

    int graphs_counter=0;

    unsigned long int current_sp;
    heap_for_score hfs;
    hfs.score_array=NULL;
    bool first_TopK=true;
    int trash=0;

    while(quit==false){
        quit_string = fgets(command,COMMANDMAXLENGTH, stdin);
        if(quit_string==NULL){
            quit=true;
            command[0]='q';
        }
        if(command[0]=='A' && command!=NULL){
            if(adiacency_list==NULL){
                initialize_adjacency_list(&adiacency_list, DoG);
            }
            if(first_use_of_dijkstra==true){
                initialize_heap_for_dijkstra(&hfd, DoG);
            }else{
                refresh_heap_for_dijkstra(&hfd, DoG);
            }
            //Start reading graph
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
            //End reading graph
            current_sp = dijkstra(adiacency_list, DoG, &hfd);
            first_use_of_dijkstra=false;
            if(hfs.score_array==NULL){
                initialize_heap_for_score(&hfs, NoT);
            }
            if(graphs_counter<NoT){
                add_element_to_hfs(&hfs, current_sp, graphs_counter);
            }else{
                if(current_sp<hfs.score_array[0].score_number){
                    trash = cancellaMax_for_score(&hfs);
                    if(trash==0){
                        //Do nothing
                    }
                    add_element_to_hfs(&hfs, current_sp, graphs_counter);
                }
            }

            graphs_counter++;
        }else if(command[0]=='T' && command!=NULL){
            if(first_TopK==true){
                first_TopK=false;
            }
            handleTopK(&hfs,NoT, graphs_counter);
        }else{
        }
    }

    return 0;
}