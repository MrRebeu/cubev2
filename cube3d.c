/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 10:09:42 by tcaccava          #+#    #+#             */
/*   Updated: 2025/05/12 17:12:57 by tcaccava         ###   ########.fr       */
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

int is_not_wall(t_map *map, double x, double y)
{
    int map_x;
    int map_y;

    map_x = (int)(x / TILE_SIZE);
    map_y = (int)(y / TILE_SIZE);
    
    if (map_x < 0 || map_x >= map->width || map_y < 0 || map_y >= map->height)
        return (0);
    
    // Check if the cell is a wall ('1')
    if (map->matrix[map_y][map_x] == '1')
        return (0);  // Not passable (it's a wall)
    else
        return (1);  // Passable (not a wall)
}

double	normalize_angle(double angle)
{
	while (angle < 0)
		angle = angle + (2 * M_PI);
	while (angle >= 2 * M_PI)
		angle = angle - (2 * M_PI);
	return (angle);
}

double ray_casting(t_game *game, double radiant_angle)
{
    t_intersect v;
    t_intersect h;
    double dist_v;
    double dist_h;
    char v_hit_type = '0'; // Type d'objet touché par l'intersection verticale
    char h_hit_type = '0'; // Type d'objet touché par l'intersection horizontale

    radiant_angle = normalize_angle(radiant_angle);
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
    dist_v = sqrt(pow(v.x - game->player.x, 2) + pow(v.y - game->player.y, 2));
    dist_h = sqrt(pow(h.x - game->player.x, 2) + pow(h.y - game->player.y, 2));
    
    // Détermination de l'intersection la plus proche et de son type
    int ray_index = (int)((radiant_angle - game->player.angle + FOV / 2) / FOV * DISPLAY_WIDTH);
    if (ray_index >= 0 && ray_index < DISPLAY_WIDTH)
    {
        if (dist_v < dist_h)
        {
            game->rays[ray_index].hit_type = v_hit_type;
        }
        else
        {
            game->rays[ray_index].hit_type = h_hit_type;
        }
    }
    
    return (fmin(dist_v, dist_h));
}

double	no_fish_eye(double min_distance, double radiant_angle,
		double player_angle)
{
	double	corrected_dist;
	double	angle_diff;

	angle_diff = radiant_angle - player_angle;
	angle_diff = normalize_angle(angle_diff);
	// calcola la distanza perpendicolare
	corrected_dist = min_distance * cos(angle_diff);
	return (corrected_dist);
}
int	calc_wall_height(double corrected_dist)
{
	double	distance_to_projection_plane;
	double	wall_height;

	distance_to_projection_plane = (DISPLAY_WIDTH / 2.0) / tan(FOV / 2.0);
	wall_height = (TILE_SIZE / fmax(corrected_dist, 1))
		* distance_to_projection_plane;
	return ((int)wall_height);
}
