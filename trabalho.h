typedef struct arguments {
    int** matrix;
    int num;
    int row;
    int col;
    int option;
    int round;
} args;

void swap(int *num1, int *num2);
void random_array(int array[], int n);
void *search_matrix(void *arguments);