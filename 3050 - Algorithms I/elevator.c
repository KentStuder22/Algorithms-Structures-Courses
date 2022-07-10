#include <stdio.h>
#include <stdlib.h>

// Solomon DellaPenna
// SLDN3F
// 12550386
// CS_3050
// Programming Assignment #3

int main() {
    // Read input
    int f, s, g, u, d;
    scanf("%d %d %d %d %d", &f, &s, &g, &u, &d);

    int max = abs(s-g); //maximum amount of steps to take before giving up
    int step_list[100]; //arr to hold steps for printing
    int idx = 0; // Index within step tracking array
    int lvl = s; // Current level
    int max_step_flag = 0; // Flag to mark if the max amount of steps has bene reached (so print use the stairs)

    // While the current level isn't the goal
    while(lvl != g){
        // Assign current level to array for printing
        step_list[idx++] = lvl;

        // If the level provided or traversed to is negative or zero, exit
        if (lvl < 1){
            max_step_flag = 1;
            break;
        }// Traverse down if the goal is below
        else if (lvl > g){
            lvl -= d;
        }// Traverse up if the goal is above
        else if (lvl < g){
            lvl += u;
        }

        // If the maximum amount of steps are reached, break the loop and mark the flag
        if (idx > max){
            max_step_flag = 1;
            break;
        }
    }

    // If flag is marked, print the error message and break
    if (max_step_flag == 1){
        printf("use the stairs\n");
    } else { // Else, iterate through the array and print values
        for (int i = 1; i < idx; i++){
            printf("%d -> ", step_list[i]);
        }
        printf("%d\n", g);
    }

    return max_step_flag;
}