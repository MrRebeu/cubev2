#include "../cube3d.h"

int	should_skip_weapon_render(t_game *game, t_weapon_pickup *weapon)
{
	if (weapon->weapon_type == HEALGUN)
	{
		if (game->player.has_weapon[HEALGUN])
		{
			if (!load_weapon_pickup_sprite(game, weapon,
					"./texture/w_heal_pickup.xpm"))
				printf("Warning: syringe_pickup.xpm not found\n");
		}
		else if (!weapon->sprite.ptr)
			load_weapon_pickup_sprite(game, weapon,
				"./texture/w_healgun_pickup.xpm");
		return (0);
	}
	else if (game->player.has_weapon[weapon->weapon_type])
		return (1);
	return (0);
}

static int	check_weapon_visibility(t_game *game, t_weapon_pickup *weapon,
		t_render *renderer)
{
	calculate_weapon_transform(game, weapon, renderer);
	if (renderer->floor_y <= 0.2f)
		return (0);
	calculate_weapon_screen_pos(game, renderer);
	if (renderer->x < 0 || renderer->x >= DISPLAY_WIDTH)
		return (0);
	return (check_enemy_occlusion(game, renderer));
}

static void	setup_weapon_rendering(t_game *game, t_render *renderer)
{
	setup_enemy_render_params(game, renderer);
	renderer->draw_end += 400;
}

void	render_weapon_pickup(t_game *game, t_weapon_pickup *weapon)
{
	t_render	renderer;
	int			is_visible;

	if (!weapon->active)
		return ;
	if (should_skip_weapon_render(game, weapon))
		return ;
	is_visible = check_weapon_visibility(game, weapon, &renderer);
	if (!is_visible)
		return ;
	setup_weapon_rendering(game, &renderer);
	draw_weapon_pickup_sprite(game, &weapon->sprite,
		(t_point){renderer.draw_start, renderer.draw_end},
		renderer.sprite_size);
}


void	calculate_weapon_transform(t_game *game, t_weapon_pickup *weapon,
		t_render *render)
{
	double	dx;
	double	dy;
	double	inv_det;

	dx = weapon->x - game->player.x;
	dy = weapon->y - game->player.y;
	inv_det = 1.0f / (game->player.plane_x * game->player.dir_y
			- game->player.dir_x * game->player.plane_y);
	render->floor_x = inv_det * (game->player.dir_y * dx
			- game->player.dir_x * dy);
	render->floor_y = inv_det * (-game->player.plane_y * dx
			+ game->player.plane_x * dy);
}