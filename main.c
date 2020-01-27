#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

#define ROUND_UP(N, S) (((N) + (S) - 1) / (S))

#define NOF_REP 1
#define MAX_POW_2  26

/*! \brief  Create random 32bit number
 *  \return returns random 32bit number
 *  Uses rand() function to create a random 32 bit number using two calls
 */
static uint32_t random32(void){
    return (rand() ^ (rand() << 15));
}

/*! \brief  Create a randomized array for random walks
 *  \param buffer Pointer to allocated memory segment
 *  \param nofElem Number of elements in array
 *  \return returns error
 */
static int linkRandom(uint32_t * buffer, uint32_t nofElem){

    // Seed random
    srand(time(0));

    // Link sequentially
    for(uint32_t i = 0; i< nofElem-1; i++){
        buffer[i] = i+1;
    }
    buffer[nofElem-1] = 0;

    // Shuffle array
    for (uint32_t i = 0; i<nofElem;i++){
        uint32_t rndi, tmp1, tmp2, tmp3;
        rndi = random32()%nofElem;
        if (rndi == i) continue;

        tmp1 = buffer[i];
        tmp2 = buffer[rndi];
        tmp3 = buffer[tmp2];
        if (i== tmp2) continue;

        // Reassign links
        buffer[i] = tmp2;
        buffer[rndi] = tmp3;
        buffer[tmp2] = tmp1;
    }
    return 0;
}

static uint32_t power(uint32_t base, uint32_t exponent)
{
    uint32_t result = 1;

    for(uint32_t i = 0; i<exponent; ++i)
    {
       result*=base;
    }

    return result;
}

static void linkSeq(uint32_t* array, uint32_t nof_elem)
{
    for(uint32_t i = 0 ; i<nof_elem-1; ++i)
    {
       array[i] = i+1;
    }
    array[nof_elem-1] = 0;
}

int main()
{
    struct timespec start, end;
    uint32_t* array;


    for(int i = 10 ; i < MAX_POW_2; i++)
    {

        uint32_t nof_elem = power(2,i)/sizeof(uint32_t);

        array = NULL;
        array = (uint32_t*) malloc(nof_elem*sizeof(uint32_t));
        if(array == NULL)
        {
            printf("Could not allocate memory\n");
            exit(0);
        }

        linkRandom(array, nof_elem);
        //linkSeq(array, nof_elem);

        double agg_diff_ns = 0.0;
        for(int32_t rep = -2 ; rep < NOF_REP ; ++rep)
        {
            uint32_t k = nof_elem;
            uint32_t currIndex = 0;
            clock_gettime(CLOCK_MONOTONIC, &start);
            while( k-- ) currIndex = array[currIndex];
            clock_gettime(CLOCK_MONOTONIC, &end);

            double start_ns = start.tv_sec * 1e6 + start.tv_nsec * 1e-3;
            double end_ns = end.tv_sec * 1e6 + end.tv_nsec * 1e-3;

            if(rep >= 0)
            {
                agg_diff_ns += (end_ns-start_ns);
            }
        }

        double diff_ns = agg_diff_ns/NOF_REP*1e3;
        //printf("diff_agg: %f\n", agg_diff_ns);
        printf("%f ns/elem, %lu\n", diff_ns/(nof_elem), nof_elem*sizeof(uint32_t)/1024);
        free(array);
    }

    return 0;
}
