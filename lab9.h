#pragma once

#include <stdio.h>
#include <math.h>

void hometask(void) 
{
    const double R = 20, A = (sqrt(5.) + 1) / sqrt(5.) * R, B = sqrt(R * R - pow(A - R, 2));
    printf("%.1lf %.1lf\n", R, A);
    int delta = 0;
    for (int y = 0; y < 2 * R; y++)
    {
        delta = sqrt(pow(R, 2) - pow(R - y, 2));

        for (int x = 0; x < 2 * R; x++)
        {

            if (x == R - delta || x == delta + R)printf("\' ");
            else
            {
                if (x < delta + R) printf("  ");

            }

            //if ((y < A) && x < B) printf("\b\b+ ");
        }

        printf("\n");
    }
}