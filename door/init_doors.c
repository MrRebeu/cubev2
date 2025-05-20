#include "cube3d.h"

void init_doors(t_game *game)
{
    int x, y;
    game->door_count = 0;
    
    for (y = 0; y < game->map.height; y++)
    {
        for (x = 0; x < game->map.width; x++)
        {
            if (game->map.matrix[y][x] == 'D')
            {
                // Ajouter cette porte au tableau
                game->doors[game->door_count].x = x;
                game->doors[game->door_count].y = y;
                game->doors[game->door_count].is_open = 0; // Fermée par défaut
                game->door_count++;
                
                if (game->door_count >= MAX_DOORS)
                    break;
            }
        }
    }
}