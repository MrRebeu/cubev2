#include "../cube3d.h"

void	idle(t_enemy *e, t_player *p, t_map *m, double dx, double dy, double d)
{
	double	move_x;
	double	move_y;
	double	speed;
	int		next_x;
	int		next_y;

	(void)dx;
	(void)dy;
	(void)d;
	speed = 1.5;
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
	else
		apply_random_rotation(e);
	if (enemy_sees_you(e, p, m))
		set_search_state(e);
}

void	search(t_enemy *e, t_player *p, t_map *m, double dx, double dy, double d)
{
	double	distance_in_tiles;

	distance_in_tiles = d / TILE_SIZE;
	if (!enemy_sees_you(e, p, m))
	{
		e->state = IDLE;
		e->sees_player = 0;
		return ;
	}
	if (distance_in_tiles < 1.5)
		set_melee_state(e);
	else if (distance_in_tiles < 6)
		set_shoot_state(e);
	else
		move_towards_player(e, p, m, dx, dy);
}

void	shoot(t_enemy *e, t_player *p, t_map *m, double dx, double dy, double d)
{
	double	angle_to_player;
	double	distance_in_tiles;

	distance_in_tiles = d / TILE_SIZE;
	if (!enemy_sees_you(e, p, m))
	{
		e->state = SEARCH;
		e->sees_player = 0;
		return ;
	}
	if (distance_in_tiles >= 6.0)
	{
		e->state = SEARCH;
		return ;
	}
	if (distance_in_tiles < 1.5)
		set_melee_state(e);
	else
	{
		angle_to_player = atan2(dy, dx);
		e->angle = angle_to_player;
		handle_shoot_cooldown(e, p);
	}
}

void	melee(t_enemy *e, t_player *p, t_map *m, double dx, double dy, double d)
{
	double	angle_to_player;
	double	distance_in_tiles;

	distance_in_tiles = d / TILE_SIZE;
	if (!enemy_sees_you(e, p, m) || distance_in_tiles >= 1.5)
	{
		e->state = SEARCH;
		return ;
	}
	angle_to_player = atan2(dy, dx);
	e->angle = angle_to_player;
	handle_melee_cooldown(e, p);
}