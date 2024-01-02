#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void main(){
    #pragma omp target teams num_teams(3)
    {
        int team = omp_get_team_num();
        int nteams = omp_get_num_teams();
        printf("Hello from team %d out of %d teams.\n", team, nteams);
    }
}
