//This program will read the files and output their fight, Kent Studer, CS 1050 Section E, 21 April 2020, Lab 9
#include <stdio.h>
#include <fight.h>

void Fight( char *combatant1, int ac1, int hp1, int hitbonus1, int damagebonus1, char *combatant2, int ac2, int hp2, int hitbonus2, int damagebonus2 );

int main( void )
{
    char character1[ 30 ];
    char character2[ 30 ];
    int armor1;
    int armor2;
    int hit_points1;
    int hit_points2;
    int hit_bonus1;
    int hit_bonus2;
    int dmg_bonus1;
    int dmg_bonus2;

    FILE *AhotePtr;
    FILE *GaryPtr;

    if(( AhotePtr = fopen("ahote.dat", "r")) == NULL)
    {
        printf("The file could not be opened\n");
    }
    else
    {
        fscanf( AhotePtr, "%s%d%d%d%d", character1, &armor1, &hit_points1, &hit_bonus1, &dmg_bonus1 );
        while( !feof( AhotePtr ))
        {
            printf("Name = %s\tAC = %d\tHP = %d\tHit Bonus = %d\tDamage Bonus = %d\n", character1, armor1, hit_points1, hit_bonus1, dmg_bonus1);
            fscanf( AhotePtr, "%s%d%d%d%d", character1, &armor1, &hit_points1, &hit_bonus1, &dmg_bonus1 );
        }
        //fclose( AhotePtr );
    }
    if(( GaryPtr = fopen("gary.dat", "r")) == NULL)
    {
        printf("The file could not be opened\n");
    }
    else
    {
        fscanf( GaryPtr, "%s%d%d%d%d", character2, &armor2, &hit_points2, &hit_bonus2, &dmg_bonus2 );
        while( !feof( GaryPtr ))
        {
            printf("Name = %s\tAC = %d\tHP = %d\tHit Bonus = %d\tDamage Bonus = %d\n", character2, armor2, hit_points2, hit_bonus2, dmg_bonus2);
            fscanf( GaryPtr, "%s%d%d%d%d", character2, &armor2, &hit_points2, &hit_bonus2, &dmg_bonus2);
        }
        //fclose( GaryPtr );

    }
    Fight( character1, armor1, hit_points1, hit_bonus1, dmg_bonus1, character2, armor2, hit_points2, hit_bonus2, dmg_bonus2);
    fclose( AhotePtr );
    fclose( GaryPtr );
    return 0;
}

    



