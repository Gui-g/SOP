#include <stdio.h>
#include <stdlib.h>

typedef struct data data;
struct data {
    int bloco_inicial;
    int num;
    char op;
};

void insert(data new_block);
data block_merge(data new_block, data old_block);
void delete_from_final_array(int position);

data* final_queue;
int final_q_size = 0;

int main() {

    while(1) {
        int bloco_inicial, num;
        char op;
        data entry;

        scanf("%d", &bloco_inicial);

        if(bloco_inicial == -1)
            break;
        else {
            scanf("%d %c", &num, &op);
            entry.bloco_inicial = bloco_inicial;
            entry.num = num;
            entry.op = op;
            insert(entry);
        }
    }

    for(int i=0; i<final_q_size; i++) {
        for(int j=i+1;j<final_q_size; j++) {
            if(final_queue[i].bloco_inicial > final_queue[j].bloco_inicial) {
                data aux = final_queue[i];
                final_queue[i] = final_queue[j];
                final_queue[j] = aux;
            }
        }
    }

    printf("Fila:\n");
    for(int i=0; i<final_q_size; i++) 
        printf("%d %d %c\n", final_queue[i].bloco_inicial, final_queue[i].num, final_queue[i].op);

    free(final_queue);
    return 0;
}

void delete_from_final_array(int position) {
    for(int i = position; i < (final_q_size)-1; i++)
        final_queue[i] = final_queue[i+1];
    
    (final_q_size)--;
    final_queue = realloc(final_queue,(final_q_size)*sizeof(data));
}

void insert(data new_block) {
    int merge = 0;
    int i;
    data aux;
    if (!final_queue) {
        final_queue = (data*)malloc(sizeof(data));
        final_queue[0] = new_block;
        final_q_size++;
        return;
    } else {
        for(i=0; i<final_q_size; i++) {
            aux = final_queue[i];
            if(new_block.op == 'r') {
                if(aux.op == new_block.op && aux.num + new_block.num <= 64) {
                    if(aux.bloco_inicial <= (new_block.bloco_inicial + new_block.num) && aux.bloco_inicial > new_block.bloco_inicial) {
                        merge = 1;
                        data operation = block_merge(new_block, aux);
                        delete_from_final_array(i);
                        insert(operation);
                    }
                    else if(new_block.bloco_inicial <= (aux.bloco_inicial + aux.num) && new_block.bloco_inicial > aux.bloco_inicial) {
                        merge = 1;
                        data operation = block_merge(new_block, aux);
                        delete_from_final_array(i);
                        insert(operation);
                    }
                }
            } else if (new_block.op == 'w') {
                if(aux.op == new_block.op && aux.num + new_block.num <= 64) {
                    if(aux.bloco_inicial == (new_block.bloco_inicial + new_block.num)) {
                        merge = 1;
                        data operation = block_merge(new_block, aux);
                        delete_from_final_array(i);
                        insert(operation);
                    }
                    else if(new_block.bloco_inicial == (aux.bloco_inicial + aux.num)) {
                        merge = 1;
                        data operation = block_merge(new_block, aux);
                        delete_from_final_array(i);
                        insert(operation);
                    }
                }
            } else 
                printf("operação invalida, instrução não inserida");
        }
    };
    if(merge == 0){
        final_q_size++;
        final_queue = realloc(final_queue, (final_q_size)*sizeof(data));
        final_queue[final_q_size-1] = new_block;
    }
}


data block_merge(data new_block, data old_block) {
    //condições de merge:
    //o bloco novo está dentro do espaço do bloco antigo:
    //100 5, 101 2 => [101,102] dentro do intervalo [100,104]
    if((old_block.bloco_inicial + old_block.num - 1) > (new_block.bloco_inicial + new_block.num - 1) && old_block.bloco_inicial < new_block.bloco_inicial) {
        old_block.num = old_block.num; //do nothing
    }
    //bloco inicial[new] > bloco inicial[old]
    //Subreposição: 100 3, 101 5 => 100 6 (101-100 = 1 + 5 = 6)
    //Adjacência: 100 5, 105 6 => 100 11 (105-100 = 5 + 6 = 11)
    //bloco inicial = bloco antigo
    //numero de blocos = |bloco1 - bloco2| + numero de blocos[new]
    else if(new_block.bloco_inicial > old_block.bloco_inicial) {
        old_block.num = (new_block.bloco_inicial - old_block.bloco_inicial) + new_block.num;
    }
    //bloco inicial[old] > bloco inicial[new]
    //Sobreposição: 104 2, 100 6 => 100 6 (104-100 = 4 + 2 = 6)
    //Adjacência: 100 6, 95 5 => 95 11 (100-95 = 5 + 6 = 11)
    //bloco inicial = bloco novo
    //numero de blocos = |bloco1 - bloco2| + numero de blocos[old]
    else if(old_block.bloco_inicial > new_block.bloco_inicial) {
        old_block.num = (old_block.bloco_inicial - new_block.bloco_inicial) + old_block.num;
        old_block.bloco_inicial = new_block.bloco_inicial;
    }
    //os dois blocos iniciam no mesmo bloco
    //100 == 100 => num = o maior deles
    else if(old_block.bloco_inicial == new_block.bloco_inicial) {
        if(old_block.num < new_block.num) {
            old_block.num = new_block.num;
        }
    }
  
    return old_block;
}