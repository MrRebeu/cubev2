#include "../cube3d.h"

void	apply_random_rotation(t_enemy *e)
{
	e->angle += ((rand() % 60) - 30) * M_PI / 180;
	e->angle = normalize_angle(e->angle);
}

void	set_search_state(t_enemy *e)
{
	e->state = SEARCH;
	e->sees_player = 1;
}

void	set_melee_state(t_enemy *e)
{
	e->state = MELEE;
	e->cooldown = 0;
}

void	set_shoot_state(t_enemy *e)
{
	e->state = SHOOT;
	e->cooldown = 0;
}

void	move_towards_player(t_enemy *e, t_player *p, t_map *m, double dx, double dy)
{
    (void)p;
	double	angle_to_player;
	double	move_x;
	double	move_y;
	double	speed;
	int		next_x;
	int		next_y;

	angle_to_player = atan2(dy, dx);
	e->angle = angle_to_player;
	speed = 3.0;
	move_x = cos(e->angle) * speed;
	move_y = sin(e->angle) * speed;
	next_x = (int)((e->x + move_x) / TILE_SIZE);
	next_y = (int)((e->y + move_y) / TILE_SIZE);
	if (next_x >= 0 && next_x < m->width && next_y >= 0 && next_y < m->height
		&& m->matrix[next_y][next_x] != '1')
	{
		e->x += move_x;
		e->y += move_y;
	}
}