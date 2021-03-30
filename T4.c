#include <stdio.h>
#include <stdlib.h>

typedef struct data data;
struct data {
    int bloco_inicial;
    int num;
    char op;
};

data insert(data new_block);
data block_merge(data new_block, data old_block);
void delete_from_array(int position, data* array, int array_size);
void sort_array(data* array, int array_size);

data* final_queue;
int final_q_size = 0;
data* insert_queue;
int insert_q_size = 0;

int main() {

    while(1) {
        int bloco_inicial, num;
        char op;
        data entry, new;

        scanf("%d", &bloco_inicial);

        if(bloco_inicial == -1)
            break;
        else {
            scanf("%d %c", &num, &op);
            entry.bloco_inicial = bloco_inicial;
            entry.num = num;
            entry.op = op;
            new = insert(entry);
            insert_q_size++;
            if(insert_q_size == 1)
                insert_queue = malloc(sizeof(data));
            else
                insert_queue = realloc(insert_queue,insert_q_size*sizeof(data));
            insert_queue[insert_q_size-1] = new;
            printf("[");
            for(int i=0;i<final_q_size;i++)
                printf("%d ", final_queue[i].bloco_inicial);
            printf("]\n");
        }
    }

    sort_array(final_queue, final_q_size);
    printf("Fila:\n");
    for(int i=0; i<final_q_size; i++) 
        printf("%d %d %c\n", final_queue[i].bloco_inicial, final_queue[i].num, final_queue[i].op);

    free(final_queue);
    free(insert_queue);
    return 0;
}

void sort_array(data* array, int array_size) {
    for(int i=0; i<array_size; i++) {
        for(int j=i+1;j<array_size; i++) {
            if(array[i].bloco_inicial > array[j].bloco_inicial) {
                data aux = array[i];
                array[i] = array[j];
                array[j] = aux;
            }
        }
    }
}

void delete_from_array(int position, data* array, int array_size) {
    printf("%d %d", position, array_size);
    for(int i = position-1; i < array_size-1; i++)
        array[i] = array[i+1];

    
    printf("%d size", array_size);
    array_size--;
    printf("%d size", array_size);
    array = realloc(array,array_size*sizeof(data));
}

data insert(data new_block) {
    int merge = 0;
    int i, j;
    data aux;
    if (!final_queue) {
        printf("inserting: %d\n", new_block.bloco_inicial);
        final_queue = (data*)malloc(sizeof(data));
        final_queue[0] = new_block;
        final_q_size++;
        return new_block;
    } else {
        for(i=0; i<insert_q_size; i++) {
            aux = insert_queue[i];
            printf("bloco = %d\n", aux.bloco_inicial);
            if(aux.op == new_block.op && aux.num + new_block.num <= 64) {
                if(aux.bloco_inicial <= (new_block.bloco_inicial + new_block.num) && aux.bloco_inicial > new_block.bloco_inicial) {
                    merge = 1;
                    data operation = block_merge(new_block, aux);
                    for(j=0;j<final_q_size;j++) {
                        if(final_queue[j].bloco_inicial == aux.bloco_inicial && final_queue[j].num == aux.num && final_queue[j].op == aux.op && final_queue[j].num < 64)
                            printf("tentando deletar 1\n");
                            delete_from_array(j,final_queue,final_q_size);
                    }
                    printf("tentando deletar 2\n");
                    delete_from_array(i,insert_queue,insert_q_size);
                    insert(operation);
                }
                else if(new_block.bloco_inicial <= (aux.bloco_inicial + aux.num) && new_block.bloco_inicial > aux.bloco_inicial) {
                    merge = 1;
                    data operation = block_merge(new_block, aux);
                    for(j=0;j<final_q_size;j++) {
                        if(final_queue[j].bloco_inicial == aux.bloco_inicial && final_queue[j].num == aux.num && final_queue[j].op == aux.op && final_queue[j].num < 64)
                            printf("tentando deletar 1\n");
                            delete_from_array(j,final_queue,final_q_size);
                    }
                    printf("tentando deletar 2\n");
                    delete_from_array(i,insert_queue,insert_q_size);
                    insert(operation);
                }
            }
        }
    };
    if(merge == 0){
        final_q_size++;
        final_queue = realloc(final_queue, (final_q_size)*sizeof(data));
        final_queue[final_q_size-1] = new_block;
        return new_block;
    }
}


data block_merge(data new_block, data old_block) {
    printf("merging: %d + %d\n", new_block.bloco_inicial, old_block.bloco_inicial);
    //condições de merge:
    //bloco inicial[old] + numero de blocos) == bloco inicial[new]
    //100 5, 105 5 => novo bloco inicia em 100, quantidade de blocos = old num + new num
    if(old_block.bloco_inicial + old_block.num == new_block.bloco_inicial){
        printf("case 1\n");
        old_block.num += new_block.num;
    }
    //o bloco novo está dentro do espaço do bloco antigo:
    //100 5, 101 2 => [101,103] dentro do intervalo [100,105]
    else if((old_block.bloco_inicial + old_block.num) > (new_block.bloco_inicial + new_block.num) && old_block.bloco_inicial < new_block.bloco_inicial) {
        printf("case 2\n");
        old_block.num = old_block.num;
    }
    //(bloco inicial[old] + numero de blocos) == (bloco inicial[new] + numero de blocos)
    //100 5, 95 10 => novo bloco inicial é o menor dos iniciais, num é do menor
    else if(new_block.bloco_inicial + new_block.num == old_block.bloco_inicial + old_block.num) {
        printf("case 3\n");
        if(new_block.bloco_inicial < old_block.bloco_inicial) {
            old_block.bloco_inicial = new_block.bloco_inicial;
            old_block.num = new_block.num;
        }
    }
    //bloco inicial[new] > bloco inicial[old]
    //100 2, 101 5 => 100 6 (101-100 = 1 + 5 = 6)
    //100 5, 104 2 => 100 6 (104-100 = 4 + 2 = 6)
    //bloco inicial = bloco antigo
    //numero de blocos = |bloco1 - bloco2| + numero de blocos[new]
    else if(new_block.bloco_inicial > old_block.bloco_inicial) {
        printf("case 4\n");
        old_block.num = (new_block.bloco_inicial - old_block.bloco_inicial) + new_block.num;
    }
    //bloco inicial[old] > bloco inicial[new]
    //104 2, 100 5 => 100 6 (104-100 = 4 + 2 = 6)
    //101 5, 100 2 => 100 6 (101-100 = 1 + 5 = 6)
    //bloco inivial = bloco antigo
    //numero de blocos = |bloco1 - bloco2| + numero de blocos[old]
    else if(old_block.bloco_inicial > new_block.bloco_inicial) {
        printf("case 6\n");
        old_block.num = (old_block.bloco_inicial - new_block.bloco_inicial) + old_block.num;
        old_block.bloco_inicial = new_block.bloco_inicial;
    }
    //os dois blocos iniciam no mesmo bloco
    //100 == 100 => num = o maior deles
    else if(old_block.bloco_inicial == new_block.bloco_inicial) {
        printf("case 7\n");
        if(old_block.num < new_block.num) {
            old_block.num = new_block.num;
        }
    }

    printf("pós merge: %d %d\n", old_block.bloco_inicial, old_block.num);    
    return old_block;
}