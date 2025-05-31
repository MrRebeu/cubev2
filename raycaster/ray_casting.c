#include "../cube3d.h"

static void	cast_vertical_ray(t_intersect *v, t_map *map)
{
	int	iter;
	int	max_iterations;

	iter = 0;
	max_iterations = 1000;
	while (iter < max_iterations)
	{
		if (!is_not_wall(map, v->x, v->y))
			break ;
		v->x += v->step_x;
		v->y += v->step_y;
		iter++;
	}
}

static void	cast_horizontal_ray(t_intersect *h, t_map *map)
{
	int	iter;
	int	max_iterations;

	iter = 0;
	max_iterations = 1000;
	while (iter < max_iterations)
	{
		if (!is_not_wall(map, h->x, h->y))
			break ;
		h->x += h->step_x;
		h->y += h->step_y;
		iter++;
	}
}

static double	calculate_distances_and_store(t_game *game, t_intersect *v, 
				t_intersect *h, int column_x)
{
	double	dist_v;
	double	dist_h;
	double	epsilon;
	char	hit_type_v;
	char	hit_type_h;

	epsilon = 0.00001;
	hit_type_v = get_hit_type(&game->map, v->x, v->y);
	hit_type_h = get_hit_type(&game->map, h->x, h->y);
	dist_v = sqrt(pow(v->x - game->player.x, 2) + pow(v->y - game->player.y, 2));
	dist_h = sqrt(pow(h->x - game->player.x, 2) + pow(h->y - game->player.y, 2));
	if (fabs(dist_v - dist_h) < epsilon)
	{
		store_ray_info(game, column_x, dist_h, h->x, h->y, 0, hit_type_h);
		return (dist_h);
	}
	if (dist_v < dist_h)
	{
		store_ray_info(game, column_x, dist_v, v->x, v->y, 1, hit_type_v);
		return (dist_v);
	}
	else
	{
		store_ray_info(game, column_x, dist_h, h->x, h->y, 0, hit_type_h);
		return (dist_h);
	}
}

double	ray_casting(t_game *game, double radiant_angle, int column_x)
{
	t_intersect	v;
	t_intersect	h;

	radiant_angle = normalize_angle(radiant_angle);
	v = v_intersection(game->player.x, game->player.y, radiant_angle);
	h = h_intersection(game->player.x, game->player.y, radiant_angle);
	cast_vertical_ray(&v, &game->map);
	cast_horizontal_ray(&h, &game->map);
	return (calculate_distances_and_store(game, &v, &h, column_x));
}