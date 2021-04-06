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