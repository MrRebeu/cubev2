/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 10:09:42 by tcaccava          #+#    #+#             */
/*   Updated: 2025/05/10 23:42:13 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

t_intersect	v_intersection(int x_player, int y_player, double radiant_angle)
{
	t_intersect	v;

	if (cos(radiant_angle) > 0) // se il player guarda a destra
	{
		v.x = floor(x_player / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
		// arrotonda in difetto alla successiva tiles
		v.step_x = TILE_SIZE;
		// lo step successivo sull'asse orizzontale e' la prossima tilesize
	}
	else
	{
		v.x = floor(x_player / TILE_SIZE) * TILE_SIZE - 0.0001;
		// se guarda a sinistra
		v.step_x = -TILE_SIZE;
	}
	v.y = y_player + (v.x - x_player) * tan(radiant_angle);
	// il . di intersezione dir proporzionale alla tangente dell'angolo
	if (sin(radiant_angle) > 0)
		v.step_y = TILE_SIZE * tan(radiant_angle); // se guarda in alto
	else
		v.step_y = -TILE_SIZE * tan(radiant_angle); // in basso
	return (v);
}

t_intersect	h_intersection(int x_player, int y_player, double radiant_angle)
{
	t_intersect	h;

	if (sin(radiant_angle) > 0) // se guarda in alto
	{
		h.y = floor(y_player / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
		h.step_y = TILE_SIZE;
	}
	else // in basso
	{
		h.y = floor(y_player / TILE_SIZE) * TILE_SIZE - 0.0001;
		h.step_y = -TILE_SIZE;
	}
	h.x = x_player + (h.y - y_player) / tan(radiant_angle);
	if (cos(radiant_angle) > 0) // destra
		h.step_x = TILE_SIZE / tan(radiant_angle);
	else // sinistra
		h.step_x = -TILE_SIZE / tan(radiant_angle);
	return (h);
}

int is_door(t_game *game, double ray_angle, double distance)
{
    // Calcule les coordonnées exactes du point d'impact
    double hit_x = game->player.x + distance * cos(ray_angle);
    double hit_y = game->player.y + distance * sin(ray_angle);
    
    // Convertit en coordonnées de la carte
    int map_x = (int)(hit_x / TILE_SIZE);
    int map_y = (int)(hit_y / TILE_SIZE);
    
    // Debug - imprime uniquement pour quelques rayons spécifiques pour éviter trop de logs
    static int call_count = 0;
    if (call_count % 100 == 0) {
        printf("is_door check: map_x=%d, map_y=%d, cell=%c\n", 
               map_x, map_y, 
               (map_x >= 0 && map_x < game->map.width && 
                map_y >= 0 && map_y < game->map.height) ? 
                game->map.matrix[map_y][map_x] : '?');
    }
    call_count++;
    
    // Vérifie si ces coordonnées correspondent à une porte
    if (map_x >= 0 && map_x < game->map.width && 
        map_y >= 0 && map_y < game->map.height &&
        game->map.matrix[map_y][map_x] == 'D')
        return 1;
    else
        return 0;
}

int is_not_wall(t_map *map, double x, double y)
{
    int map_x;
    int map_y;

    map_x = (int)(x / TILE_SIZE);
    map_y = (int)(y / TILE_SIZE);
    
    if (map_x < 0 || map_x >= map->width || map_y < 0 || map_y >= map->height)
        return (0);
    
    // Vérifie si c'est un mur ou une porte
    char cell = map->matrix[map_y][map_x];
    if (cell == '1' || cell == 'D' || cell == 'P') // Ajoute tous les types d'obstacles que tu souhaites détecter
        return (0);
    else
        return (1);
}

double ray_casting(t_game *game, double radiant_angle)
{
    t_intersect v;
    t_intersect h;
    double      dist_v;
    double      dist_h;
    char        v_hit_type = '0'; // Type d'objet touché par l'intersection verticale
    char        h_hit_type = '0'; // Type d'objet touché par l'intersection horizontale

    v = v_intersection(game->player.x, game->player.y, radiant_angle);
    h = h_intersection(game->player.x, game->player.y, radiant_angle);
    
    // Boucle pour trouver l'intersection verticale
    while (is_not_wall(&game->map, v.x, v.y))
    {
        v.x = v.x + v.step_x;
        v.y = v.y + v.step_y;
    }
    // On a trouvé un mur - identifions son type
    int map_x_v = (int)(v.x / TILE_SIZE);
    int map_y_v = (int)(v.y / TILE_SIZE);
    if (map_x_v >= 0 && map_x_v < game->map.width && 
        map_y_v >= 0 && map_y_v < game->map.height)
    {
        v_hit_type = game->map.matrix[map_y_v][map_x_v];
    }
    
    // Boucle pour trouver l'intersection horizontale
    while (is_not_wall(&game->map, h.x, h.y))
    {
        h.x = h.x + h.step_x;
        h.y = h.y + h.step_y;
    }
    // On a trouvé un mur - identifions son type
    int map_x_h = (int)(h.x / TILE_SIZE);
    int map_y_h = (int)(h.y / TILE_SIZE);
    if (map_x_h >= 0 && map_x_h < game->map.width && 
        map_y_h >= 0 && map_y_h < game->map.height)
    {
        h_hit_type = game->map.matrix[map_y_h][map_x_h];
    }
    
    // Calcul des distances
    dist_v = sqrt((v.x - game->player.x) * (v.x - game->player.x) + 
                  (v.y - game->player.y) * (v.y - game->player.y));
    dist_h = sqrt((h.x - game->player.x) * (h.x - game->player.x) + 
                  (h.y - game->player.y) * (h.y - game->player.y));
    
    // Détermination de l'intersection la plus proche et de son type
    int ray_index = (int)(radiant_angle * DISPLAY_WIDTH / (2 * M_PI)) % DISPLAY_WIDTH;
    if (ray_index < 0) ray_index += DISPLAY_WIDTH;
    
    if (dist_v < dist_h)
    {
        // Stocke le type d'objet touché dans le rayon correspondant
        int i = (int)((radiant_angle - game->player.angle + FOV / 2) / FOV * DISPLAY_WIDTH);
        if (i >= 0 && i < DISPLAY_WIDTH)
            game->rays[i].hit_type = v_hit_type;
        return (dist_v);
    }
    else
    {
        // Stocke le type d'objet touché dans le rayon correspondant
        int i = (int)((radiant_angle - game->player.angle + FOV / 2) / FOV * DISPLAY_WIDTH);
        if (i >= 0 && i < DISPLAY_WIDTH)
            game->rays[i].hit_type = h_hit_type;
        return (dist_h);
    }
}


double	no_fish_eye(double min_distance, double radiant_angle,
		double player_angle)
{
	double	corrected_dist;

	// calcola la distanza perpendicolare
	corrected_dist = min_distance * cos(radiant_angle - player_angle);
	return (corrected_dist);
}
int	calc_wall_height(double corrected_dist)
{
	double	distance_to_projection_plane;
	double	wall_height;

	// calcola l'altezza del muro in funzione della distanza
	distance_to_projection_plane = (DISPLAY_WIDTH / 2) / tan(FOV / 2);
	wall_height = (TILE_SIZE / corrected_dist) * distance_to_projection_plane;
	return ((int)wall_height);
}
