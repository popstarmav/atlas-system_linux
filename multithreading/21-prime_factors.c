#include "multithreading.h"
#include <stdlib.h>

/**
 * prime_factors - Factorizes a number into a list of prime factors
 * @s: String representation of the number to factorize
 * Return: List of prime factors
 */
list_t *prime_factors(char const *s)
{
    list_t *factors;
    unsigned long num, i;
    unsigned long *factor;

    factors = malloc(sizeof(list_t));
    if (!factors)
        return NULL;

    list_init(factors);
    num = strtoul(s, NULL, 10);

    while (num % 2 == 0)
    {
        factor = malloc(sizeof(unsigned long));
        if (!factor)
        {
            list_destroy(factors, free);
            free(factors);
            return NULL;
        }
        *factor = 2;
        list_add(factors, factor);
        num = num / 2;
    }

    for (i = 3; i * i <= num; i += 2)
    {
        while (num % i == 0)
        {
            factor = malloc(sizeof(unsigned long));
            if (!factor)
            {
                list_destroy(factors, free);
                free(factors);
                return NULL;
            }
            *factor = i;
            list_add(factors, factor);
            num = num / i;
        }
    }

    if (num > 2)
    {
        factor = malloc(sizeof(unsigned long));
        if (!factor)
        {
            list_destroy(factors, free);
            free(factors);
            return NULL;
        }
        *factor = num;
        list_add(factors, factor);
    }

    return factors;
}

