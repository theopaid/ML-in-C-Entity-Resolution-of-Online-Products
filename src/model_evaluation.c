#include "../hdr/includes.h"

void modelEvaluation(HashTable *hashTable, Vector *evaluationPairsVector, double *w_array)
{
    double passCounter = 0.0;
    int pairscount = vectorItemsCount(evaluationPairsVector);
    for (int i = 0; i < pairscount; i++)
    {
        char *leftSpecId = ((PairInfo *)evaluationPairsVector->items[i])->leftSpecId;
        char *rightSpecId = ((PairInfo *)evaluationPairsVector->items[i])->rightSpecId;
        int isMatch = ((PairInfo *)evaluationPairsVector->items[i])->isMatch;

        double *tf_idfSum = calcTF_IDFsum(findTF_IDFvector(hashTable, leftSpecId), findTF_IDFvector(hashTable, rightSpecId));

        double p_x = p_logistic_function(f_linear_functionV2(TF_IDF_SIZE, tf_idfSum, w_array));
        printf("p_x: %s, %s, %d, %f\n", leftSpecId, rightSpecId, ((PairInfo *)evaluationPairsVector->items[i])->isMatch, p_x);

        if (fabs(1.0 - p_x) <= fabs(0.0 - p_x))
        {
            if (isMatch == 1)
            {
                passCounter++;
                printf("%s , %s : PASS [score: %f]\n", leftSpecId, rightSpecId, p_x);
            }
            else
            {
                printf("%s , %s : FAIL [score: %f]\n", leftSpecId, rightSpecId, p_x);
            }
        }
        else
        {
            if (isMatch == 0)
            {
                passCounter++;
                printf("%s , %s : PASS [score: %f]\n", leftSpecId, rightSpecId, p_x);
            }
            else
            {
                printf("%s , %s : FAIL [score: %f]\n", leftSpecId, rightSpecId, p_x);
            }
        }
    }
    printf("PERCENTAGE OF SUCCESS: %f%%\n", passCounter / (double)pairscount * 100.0);
}
