
#include "../cube3d.h"

void render_column(t_game *game, int column_x, t_ray *ray)
{
    t_render renderer;

    renderer.corrected_dist = no_fish_eye(ray->distance, ray->radiant_angle, ray->player_angle);
    renderer.wall_height = calc_wall_height(renderer.corrected_dist);
    renderer.door_height = (int)(renderer.wall_height * 1.3);
    
    renderer.draw_start = (DISPLAY_HEIGHT / 2) - (renderer.wall_height / 2) + game->pitch;
    renderer.draw_end = (DISPLAY_HEIGHT / 2) + (renderer.wall_height / 2) + game->pitch;
    
    if (renderer.draw_start < 0)
        renderer.draw_start = 0;
    if (renderer.draw_end >= DISPLAY_HEIGHT)
        renderer.draw_end = DISPLAY_HEIGHT - 1;
    
    static int debug_count = 0;
    if (ray->has_transparent_door && debug_count < 5) {
        printf("🔍 Column %d: has_transparent_door=1, hit_type='%c'\n", 
               column_x, ray->hit_type);
        debug_count++;
    }
    // if (column_x >= 1280 && column_x <= 1285) {
    //     printf("🎨 RENDER Column %d: has_transparent_door=%d, hit_type='%c'\n", 
    //            column_x, ray->has_transparent_door, ray->hit_type);
    // }
    // ✅ NOUVEAU : Gérer les portes transparentes
    if (ray->has_transparent_door)
    {
        //printf("✅ TRANSPARENT DOOR PATH!\n");
        // 1. Dessiner le mur du fond d'abord
        if (ray->hit_type == 'P')
            render_wall_portal(game, column_x, &renderer, ray);
        else if (ray->hit_type == 'D')
            render_door(game, column_x, &renderer, ray);
        else if (ray->hit_type == 'i')
            render_wall_shooted(game, column_x, &renderer, ray);
        else if (ray->hit_type == 'd')
            render_door_shooted(game, column_x, &renderer, ray);
        else
            render_wall(game, column_x, &renderer, ray);
        
        // 2. Dessiner la porte transparente par-dessus
        render_transparent_door(game, column_x, ray);
    }
    else
    {
        
        //printf("❌ NORMAL PATH - Column %d\n", column_x);
        // ✅ Rendu normal (pas de porte transparente)
        if (ray->hit_type == 'P')
            render_wall_portal(game, column_x, &renderer, ray);
        else if (ray->hit_type == 'D')
            render_door(game, column_x, &renderer, ray);
        else if (ray->hit_type == 'i')
            render_wall_shooted(game, column_x, &renderer, ray);
        else if (ray->hit_type == 'd')
            render_door_shooted(game, column_x, &renderer, ray);
        else if (ray->hit_type == 'O')
            render_open_door(game, column_x, &renderer, ray);
        else
            render_wall(game, column_x, &renderer, ray);
    }
        
    render_floor_and_ceiling(game, column_x, &renderer);
}

void render_scene(t_game *game)
{
    int col = 0;

    while (col < DISPLAY_WIDTH)
    {
        render_column(game, col, &game->rays[col]);
        col++;
    }
    //render_all_open_doors(game);
}

void render_frame(t_game *game)
{
    int col = 0;

    while (col < DISPLAY_WIDTH)
    {
        render_column(game, col, &game->rays[col]);
        col++;
    }
    mlx_put_image_to_window(game->mlx, game->win, game->screen.ptr, 0, 0);
    render_weapon(game);
}

void render_ui(t_game *game)
{
    render_weapon(game);
    draw_health_bar(game);
    minimap(game);
}

void render_next_frame_enemies(t_game *game)
{
    int j = 0;
    
    while (j < game->num_enemies)
    {
        if (game->enemies[j].active)
            render_enemy(game, &game->enemies[j]);
        j++;
    }
}