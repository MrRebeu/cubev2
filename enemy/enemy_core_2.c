#include "cube3d.h"

int	line_of_sight(double ex, double ey, double px, double py, t_map *map)
{
	double	dx;
	double	dy;
	double	distance;
	double	step_x;
	double	step_y;
	double	x;
	double	y;
	double	traveled;
	int		map_x;
	int		map_y;

	dx = px - ex;
	dy = py - ey;
	distance = sqrt(dx * dx + dy * dy);
	step_x = (dx / distance) * 5.0;
	step_y = (dy / distance) * 5.0;
	x = ex;
	y = ey;
	traveled = 0.0;
	while (traveled < distance)
	{
		map_x = (int)(x / TILE_SIZE);
		map_y = (int)(y / TILE_SIZE);
		if (map_x < 0 || map_x >= map->width ||
			map_y < 0 || map_y >= map->height)
			return (0);
		if (map->matrix[map_y][map_x] == '1')
			return (0);
		x += step_x;
		y += step_y;
		traveled += 5.0;
	}
	return (1);
}

int	damage_enemy_at_position(t_game *game, int tile_x, int tile_y, int damage)
{
	int			i;
	t_enemy		*enemy;
	int			enemy_tile_x;
	int			enemy_tile_y;

	i = 0;
	while (i < game->num_enemies)
	{
		enemy = &game->enemies[i];
		enemy_tile_x = (int)(enemy->x / TILE_SIZE);
		enemy_tile_y = (int)(enemy->y / TILE_SIZE);
		if (enemy_tile_x == tile_x && enemy_tile_y == tile_y && enemy->active)
		{
			enemy->health -= damage;
			if (enemy->health <= 0)
			{
				enemy->death_timer = 300;
				enemy->animation.current_frame = 0;
				enemy->animation.frame_counter = 0;
				enemy->state = DEAD;
				return (1);
			}
			else
				return (0);
		}
		i++;
	}
	return (0);
}