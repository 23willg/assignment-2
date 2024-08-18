#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

#define CHECK_ERR(err, msg)                           \
    if (err != CL_SUCCESS)                            \
    {                                                 \
        fprintf(stderr, "%s failed: %d\n", msg, err); \
        exit(EXIT_FAILURE);                           \
    }

void NaiveMatrixMultiply(Matrix *input0, Matrix *input1, Matrix *result)
{
    //@@ Insert code to implement naive matrix multiply here
    int r_row = (*result).shape[0];
    int r_col = (*result).shape[1];
    int i0_row = (*input0).shape[0];
    int i1_row = (*input1).shape[0];
    int i0_col = (*input0).shape[1];
    int i1_col = (*input1).shape[1];


    for (int row = 0; row < i0_row; row++)
    {
        for (int col = 0; col < i1_col; col++)
        {
            for (int i = 0; i < i0_col; i++)
            {
		int i0_pos = row * i0_col + i;
		int i1_pos = i * i1_col + col;
		int r_pos = row * i1_col + col;
		//fprintf(stderr, " i0_pos%d\n", i0_pos); 
		//fprintf(stderr, " i1_pos%d\n", i1_pos); 
		//fprintf(stderr, " r_pos%d\n\n", r_pos); 
                (*result).data[r_pos] += ( (*input0).data[i0_pos] * (*input1).data[i1_pos] );
            }
        }
    }
    
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s <input_file_0> <input_file_1> <answer_file> <output_file>\n", argv[0]);
        return -1;
    }

    const char *input_file_a = argv[1];
    const char *input_file_b = argv[2];
    const char *input_file_c = argv[3];
    const char *input_file_d = argv[4];

    // Host input and output vectors and sizes
    Matrix host_a, host_b, host_c, answer;
    
    cl_int err;

    err = LoadMatrix(input_file_a, &host_a);
    CHECK_ERR(err, "LoadMatrix");

    err = LoadMatrix(input_file_b, &host_b);
    CHECK_ERR(err, "LoadMatrix");

    err = LoadMatrix(input_file_c, &answer);
    CHECK_ERR(err, "LoadMatrix");

    int rows, cols;
    //@@ Update these values for the output rows and cols of the output
    //@@ Do not use the results from the answer matrix
    rows = host_a.shape[0];
    cols = host_b.shape[1];

    // Allocate the memory for the target.
    host_c.shape[0] = rows;
    host_c.shape[1] = cols;
    host_c.data = (float *)malloc(sizeof(float) * host_c.shape[0] * host_c.shape[1]);

    // Call your matrix multiply.
    NaiveMatrixMultiply(&host_a, &host_b, &host_c);

    // // Call to print the matrix
    //PrintMatrix(&host_c);

    // Check the result of the matrix multiply
    CheckMatrix(&answer, &host_c);

    // Save the matrix
    SaveMatrix(input_file_d, &host_c);

    // Release host memory
    free(host_a.data);
    free(host_b.data);
    free(host_c.data);
    free(answer.data);

    return 0;
}
