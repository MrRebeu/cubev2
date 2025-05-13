#include "../cube3d.h"

void update_rays(t_game *game)
{
    int i;
    double ray_offset;
    double radiant_angle;
    
    i = 0;
    while (i < DISPLAY_WIDTH)
    {
        // Calculate ray angle (same as in main)
        ray_offset = game->player.fov * ((double)i / DISPLAY_WIDTH - 0.5);
        radiant_angle = game->player.angle + ray_offset;
        
        // Normalize angle
        radiant_angle = normalize_angle(radiant_angle);
        
        // Update ray properties
        game->rays[i].radiant_angle = radiant_angle;
        game->rays[i].player_angle = game->player.angle;
        
        // Cast ray with hit_type
        game->rays[i].distance = ray_casting(game, radiant_angle);
        
        i++;
    }
}
