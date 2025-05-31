/* ************************************************************************** */
/*                                                                            */
/*   render_walls.c - Rendu des murs et portes                              */
/*                                                                            */
/* ************************************************************************** */

#include "../cube3d.h"




void render_open_door_as_sprite(t_game *game, t_open_door *door)
{
    if (!door->active || !door->sprite.ptr)
        return;
    
    // ✅ CALCULS DE PROJECTION (comme les ennemis)
    double dx = door->x - game->player.x;
    double dy = door->y - game->player.y;
    double distance = sqrt(dx * dx + dy * dy);
    
    if (distance < 10.0 || distance > 1000.0)
        return;
    
    // ✅ TRANSFORMATION 3D
    double inv_det = 1.0f / (game->player.plane_x * game->player.dir_y - 
                              game->player.dir_x * game->player.plane_y);
    double transform_x = inv_det * (game->player.dir_y * dx - game->player.dir_x * dy);
    double transform_y = inv_det * (-game->player.plane_y * dx + game->player.plane_x * dy);
    
    if (transform_y <= 0.2f)
        return;
    
    // ✅ POSITION À L'ÉCRAN
    int screen_x = (int)((DISPLAY_WIDTH / 2) * (1 + transform_x / transform_y));
    
    if (screen_x < 0 || screen_x >= DISPLAY_WIDTH)
        return;
    
    // ✅ TAILLE BASÉE SUR LA DISTANCE
    double distance_to_projection = (DISPLAY_WIDTH / 2.0) / tan(game->player.fov / 2.0);
    double door_height = TILE_SIZE * 1.5;
    int sprite_height = (int)((door_height / transform_y) * distance_to_projection);
    
    if (sprite_height < 1) sprite_height = 1;
    if (sprite_height > DISPLAY_HEIGHT * 2) sprite_height = DISPLAY_HEIGHT * 2;
    
    // ✅ POSITION VERTICALE
    int door_top = (DISPLAY_HEIGHT / 2) - (sprite_height / 2) + game->pitch;
    int door_bottom = door_top + sprite_height;
    
    // ✅ LARGEUR DE LA PORTE (fine)
    int door_width = fmax(1, sprite_height / 12); // Ultra-fine
    int start_col = screen_x - door_width / 2;
    int end_col = screen_x + door_width / 2;
    
    // ✅ CLAMP
    if (start_col < 0) start_col = 0;
    if (end_col >= DISPLAY_WIDTH) end_col = DISPLAY_WIDTH - 1;
    
    // ✅ RENDU COLONNE PAR COLONNE (comme render_open_door)
    for (int col = start_col; col <= end_col; col++)
    {
        // ✅ Vérifier occlusion
        if (transform_y >= game->depth_buffer[col])
            continue;
        
        // ✅ TEXTURE X (centre de la texture)
        int tex_x = door->sprite.width / 2;
        if (door_width > 1)
            tex_x = ((col - start_col) * door->sprite.width) / door_width;
        if (tex_x >= door->sprite.width) tex_x = door->sprite.width - 1;
        
        // ✅ RENDU VERTICAL (même logique que render_open_door)
        for (int y = door_top; y <= door_bottom; y++)
        {
            if (y < 0 || y >= DISPLAY_HEIGHT)
                continue;
            
            // ✅ TEXTURE Y
            int tex_y = ((y - door_top) * door->sprite.height) / (door_bottom - door_top + 1);
            if (tex_y >= door->sprite.height) tex_y = door->sprite.height - 1;
            
            // ✅ RÉCUPÉRER LE PIXEL
            char *src = door->sprite.addr + tex_y * door->sprite.line_length + 
                       tex_x * (door->sprite.bits_per_pixel / 8);
            unsigned int color = *(unsigned int *)src;
            
            int red = (color >> 16) & 0xFF;
            int green = (color >> 8) & 0xFF;
            int blue = color & 0xFF;
            
            // ✅ MÊME TRANSPARENCE ROUGE que render_open_door
            if (!(red >= 200 && green <= 50 && blue <= 50))
            {
                char *dst = game->screen.addr + y * game->screen.line_length + 
                           col * (game->screen.bits_per_pixel / 8);
                *(unsigned int *)dst = color;
            }
        }
    }
}

void render_fixed_door_columns(t_game *game, t_open_door *door, int center_column, 
                              int door_width, int door_top, int door_bottom, double distance)
{
    (void)distance;
    
    if (!door->sprite.ptr)
        return;
    
    // ✅ Calculer les colonnes de début et fin
    int start_col = center_column - door_width / 2;
    int end_col = center_column + door_width / 2;
    
    // ✅ Clamp aux limites d'écran
    if (start_col < 0) start_col = 0;
    if (end_col >= DISPLAY_WIDTH) end_col = DISPLAY_WIDTH - 1;
    
    int sprite_height = door_bottom - door_top;
    if (sprite_height <= 0) return;
    
    // ✅ Pour chaque colonne de la porte
    for (int col = start_col; col <= end_col; col++)
    {
        if (col < 0 || col >= DISPLAY_WIDTH)
            continue;
        
        // ✅ Position X dans la texture (centrée)
        int tex_x = (door->sprite.width / 2); // Centre de la texture
        if (door_width > 1)
        {
            // Si la porte fait plusieurs pixels, étaler sur la texture
            tex_x = ((col - start_col) * door->sprite.width) / door_width;
        }
        if (tex_x >= door->sprite.width) tex_x = door->sprite.width - 1;
        
        // ✅ Pour chaque pixel vertical
        for (int y = door_top; y <= door_bottom; y++)
        {
            if (y < 0 || y >= DISPLAY_HEIGHT)
                continue;
            
            // ✅ Position Y dans la texture
            int tex_y = ((y - door_top) * door->sprite.height) / sprite_height;
            if (tex_y >= door->sprite.height) tex_y = door->sprite.height - 1;
            
            // ✅ Récupérer le pixel de texture
            char *src = door->sprite.addr + tex_y * door->sprite.line_length + 
                       tex_x * (door->sprite.bits_per_pixel / 8);
            unsigned int color = *(unsigned int *)src;
            
            int red = (color >> 16) & 0xFF;
            int green = (color >> 8) & 0xFF;
            int blue = color & 0xFF;
            
            // ✅ Transparence pour ne garder que les montants
            int skip_pixel = 0;
            
            if (red < 15 && green < 15 && blue < 15) // Noir
                skip_pixel = 1;
            if (abs(red - 255) <= 3 && abs(green - 0) <= 3 && abs(blue - 0) <= 3) // Rouge pur
                skip_pixel = 1;
            if (red > 240 && green > 240 && blue > 240) // Blanc
                skip_pixel = 1;
            
            // ✅ Dessiner le pixel FIXE
            if (!skip_pixel)
            {
                char *dst = game->screen.addr + y * game->screen.line_length + 
                           col * (game->screen.bits_per_pixel / 8);
                *(unsigned int *)dst = color;
            }
        }
    }
}

void draw_ultra_thin_door_sprite(t_game *game, t_img *sprite, t_point pos, int size)
{
    int i = 0;
    int j;
    int src_x, src_y, x, y;
    char *src, *dst;
    unsigned int color;
    int red, green, blue;
    
    if (!sprite || !sprite->addr || size <= 0)
        return;
    
    while (i < size)
    {
        j = 0;
        while (j < size)
        {
            // ✅ Échantillonnage de la texture
            src_x = i * sprite->width / size;
            src_y = j * sprite->height / size;
            
            if (src_x < 0) src_x = 0;
            else if (src_x >= sprite->width) src_x = sprite->width - 1;
            if (src_y < 0) src_y = 0;
            else if (src_y >= sprite->height) src_y = sprite->height - 1;
            
            src = sprite->addr + src_y * sprite->line_length + src_x * (sprite->bits_per_pixel / 8);
            color = *(unsigned int *)src;
            
            red = (color >> 16) & 0xFF;
            green = (color >> 8) & 0xFF;
            blue = color & 0xFF;
            
            // ✅ TRANSPARENCE ULTRA-PRÉCISE pour porte fine
            int skip_pixel = 0;
            
            // Skip pixels noirs (fond transparent)
            if (red < 15 && green < 15 && blue < 15)
                skip_pixel = 1;
            
            // Skip pixels rouge pur (marqueur de transparence)
            if (abs(red - 255) <= 3 && abs(green - 0) <= 3 && abs(blue - 0) <= 3)
                skip_pixel = 1;
            
            // Skip pixels blancs très clairs (souvent transparents)
            if (red > 240 && green > 240 && blue > 240)
                skip_pixel = 1;
            
            // ✅ NE DESSINER QUE LES PIXELS OPAQUES (montants de porte)
            if (!skip_pixel)
            {
                x = pos.x + i;
                y = pos.y + j;
                if (x >= 0 && x < DISPLAY_WIDTH && y >= 0 && y < DISPLAY_HEIGHT)
                {
                    dst = game->screen.addr + y * game->screen.line_length + x * (game->screen.bits_per_pixel / 8);
                    *(unsigned int *)dst = color;
                }
            }
            j++;
        }
        i++;
    }
}


void render_door_columns(t_game *game, t_open_door *door, int col_start, int col_end, 
                        int door_top, int door_bottom, double distance)
{
    (void)distance;
    int col, y;
    int tex_x, tex_y;
    char *src, *dst;
    unsigned int color;
    int red, green, blue;
    int door_width_screen = col_end - col_start;
    
    if (door_width_screen <= 0 || !door->sprite.ptr)
        return;
    
    // ✅ Calculer le facteur d'étirement de la texture
    double texture_scale = (double)door->sprite.width / door_width_screen;
    
    col = col_start;
    while (col <= col_end)
    {
        if (col >= 0 && col < DISPLAY_WIDTH)
        {
            // ✅ Étirer la texture sur toute la largeur
            double tex_x_float = (col - col_start) * texture_scale;
            tex_x = (int)tex_x_float;
            if (tex_x >= door->sprite.width) tex_x = door->sprite.width - 1;
            
            y = door_top;
            while (y <= door_bottom)
            {
                if (y >= 0 && y < DISPLAY_HEIGHT)
                {
                    tex_y = ((y - door_top) * door->sprite.height) / (door_bottom - door_top + 1);
                    if (tex_y >= door->sprite.height) tex_y = door->sprite.height - 1;
                    
                    src = door->sprite.addr + tex_y * door->sprite.line_length + 
                          tex_x * (door->sprite.bits_per_pixel / 8);
                    color = *(unsigned int *)src;
                    
                    red = (color >> 16) & 0xFF;
                    green = (color >> 8) & 0xFF;
                    blue = color & 0xFF;
                    
                    // ✅ Skip transparence mais garder les montants de porte
                    if (!(red < 10 && green < 10 && blue < 10))
                    {
                        dst = game->screen.addr + y * game->screen.line_length + 
                              col * (game->screen.bits_per_pixel / 8);
                        *(unsigned int *)dst = color;
                    }
                }
                y++;
            }
        }
        col++;
    }
}

void draw_open_door_sprite(t_game *game, t_img *sprite, t_point pos, int size)
{
    int i = 0;
    int j;
    int src_x, src_y, x, y;
    char *src, *dst;
    unsigned int color;
    int red, green, blue;
    int skip_pixel = 0;
    
    if (!sprite || !sprite->addr || size <= 0)
        return;
        
    while (i < size)
    {
        j = 0;
        while (j < size)
        {
            src_x = i * sprite->width / size;
            src_y = j * sprite->height / size;
            
            if (src_x < 0) src_x = 0;
            else if (src_x >= sprite->width) src_x = sprite->width - 1;
            if (src_y < 0) src_y = 0;
            else if (src_y >= sprite->height) src_y = sprite->height - 1;
            
            src = sprite->addr + src_y * sprite->line_length + src_x * (sprite->bits_per_pixel / 8);
            color = *(unsigned int *)src;
            
            red = (color >> 16) & 0xFF;
            green = (color >> 8) & 0xFF;
            blue = color & 0xFF;

            // ✅ Transparence (pixels noirs ou couleur spécifique)
            skip_pixel = 0;
            
            // Skip pixels noirs
            if (red < 10 && green < 10 && blue < 10)
                skip_pixel = 1;
            
            // Skip pixels rouge pur (comme les ennemis)
            if (abs(red - 255) <= 2 && abs(green - 0) <= 2 && abs(blue - 0) <= 2)
                skip_pixel = 1;

            if (!skip_pixel)
            {
                x = pos.x + i;
                y = pos.y + j;
                if (x >= 0 && x < DISPLAY_WIDTH && y >= 0 && y < DISPLAY_HEIGHT)
                {
                    dst = game->screen.addr + y * game->screen.line_length + x * (game->screen.bits_per_pixel / 8);
                    *(unsigned int *)dst = color;
                }
            }
            j++;
        }
        i++;
    }
}

void render_all_open_doors(t_game *game)
{
    int i = 0;
    
    while (i < game->num_open_doors)
    {
        if (game->open_doors[i].active)
            render_open_door_as_sprite(game, &game->open_doors[i]); // ← Nouvelle fonction
        i++;
    }
}
