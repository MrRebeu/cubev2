#include "cube3d.h"

t_door *find_door(t_game *game, int x, int y)
{
    int i;
    
    for (i = 0; i < game->door_count; i++)
    {
        if (game->doors[i].x == x && game->doors[i].y == y)
            return &game->doors[i];
    }
    
    return NULL; // Aucune porte trouvée
}

int toggle_door(t_game *game, int x, int y)
{
    t_door *door = find_door(game, x, y);
    
    if (door)
    {
        door->is_open = !door->is_open; // Inverser l'état
        return 1; // Succès
    }
    
    return 0; // Échec - porte non trouvée
}

int check_door_interaction(t_game *game)
{
    int map_x, map_y;
    double check_distance = 80.0;
    
    double interact_x = game->player.x + cos(game->player.angle) * check_distance;
    double interact_y = game->player.y + sin(game->player.angle) * check_distance;
    
    map_x = (int)(interact_x / TILE_SIZE);
    map_y = (int)(interact_y / TILE_SIZE);
    
    if (map_x < 0 || map_x >= game->map.width || map_y < 0 || map_y >= game->map.height)
        return 0;
    
    if (game->map.matrix[map_y][map_x] == 'D')
    {
        return toggle_door(game, map_x, map_y);
    }
    
    return 0;
}

