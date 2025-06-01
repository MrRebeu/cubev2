#include "cube3d.h"

void open_door(t_game *game)
{
    char cell = game->map.matrix[map_y][map_x];
    double player_x = game->player.x;
    double player_y = game->player.y;
    double player_angle = game->player.angle;
    double check_distance = TILE_SIZE * 1.5;
    double check_x = player_x + cos(player_angle) * check_distance;
    double check_y = player_y + sin(player_angle) * check_distance;
    
    int map_x = (int)(check_x / TILE_SIZE);
    int map_y = (int)(check_y / TILE_SIZE);
    
    if (map_x < 0 || map_x >= game->map.width || 
        map_y < 0 || map_y >= game->map.height)
        return;
    char cell = game->map.matrix[map_y][map_x];
    if (cell == 'D')
        game->map.matrix[map_y][map_x] = 'O';
    else if (cell == 'O')
        game->map.matrix[map_y][map_x] = 'D';
}