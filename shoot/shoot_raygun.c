
#include "../cube3d.h"

void	handle_raygun_shot(t_game *game, t_ray *center_ray)
{
	if (check_enemy_hit(game, center_ray))
		return ;
	apply_wall_damage(game, center_ray);
}

int	check_enemy_hit(t_game *game, t_ray *center_ray)
{
	double	player_x;
	double	player_y;
	double	ray_dir_x;
	double	ray_dir_y;
	int		i;

	player_x = game->player.x;
	player_y = game->player.y;
	ray_dir_x = cos(game->player.angle);
	ray_dir_y = sin(game->player.angle);
	i = 0;
	while (i < game->num_enemies)
	{
		if (is_enemy_in_line_of_fire(game, &game->enemies[i], player_x, player_y, ray_dir_x, ray_dir_y, center_ray))
			return (damage_enemy(&game->enemies[i], game));
		i++;
	}
	return (0);
}

void	shoot_raygun(t_game *game)
{
	if (!game->player.weapon.is_firing)
	{
		start_raygun_animation(game);
		calculate_shoot(game);
	}
}

void	start_raygun_animation(t_game *game)
{
	game->player.weapon.is_firing = 1;
	game->player.weapon.current_state = WEAPON_PREFIRE;
	game->player.weapon.frame = 1;
	game->player.weapon.frame_delay = 10;
}