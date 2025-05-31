
#include "../cube3d.h"

void	render_weapon(t_game *game)
{
	t_render	renderer;
	t_img		*weapon;

	if (game->map.north.ptr != NULL)
		return ;
	weapon = get_current_weapon_sprite(game);
	if (!weapon)
		return ;
	calculate_weapon_position(game, &renderer, weapon);
	update_weapon_animation(game);
	draw_weapon_sprite(game, &renderer, weapon);
}

t_img	*get_current_weapon_sprite(t_game *game)
{
	if (game->current_weapon == HEALGUN)
		return (get_healgun_sprite(game));
	else
		return (&game->weapons[game->current_weapon][game->player.weapon.frame]);
}

t_img	*get_healgun_sprite(t_game *game)
{
	if (game->player.healgun_animating)
		return (&game->healgun_frames[game->player.healgun_anim_frame]);
	else if (game->player.healgun_ammo > 0)
		return (&game->healgun_frames[0]);
	else
		return (&game->healgun_frames[4]);
}

void	calculate_weapon_position(t_game *game, t_render *renderer, t_img *weapon)
{
	renderer->x = (DISPLAY_WIDTH - weapon->width) + 180;
	if (is_healgun_frame_2(game))
		renderer->x = (DISPLAY_WIDTH - weapon->width) + 100;
	renderer->y = (DISPLAY_HEIGHT - weapon->height) + 250 + game->pitch;
	if (is_healgun_frame_2(game))
		renderer->y += 600;
}

int	is_healgun_frame_2(t_game *game)
{
	if (game->current_weapon == HEALGUN)
		if (game->player.healgun_animating)
			if (game->player.healgun_anim_frame == 2)
				return (1);
	return (0);
}
void	update_weapon_animation(t_game *game)
{
	if (game->current_weapon == HANDS)
		return ;
	if (game->player.weapon.is_firing)
		process_weapon_animation(game);
	if (game->player.healgun_animating && game->player.healgun_anim_timer > 0)
		process_healgun_animation(game);
}

void	draw_weapon_sprite(t_game *game, t_render *renderer, t_img *weapon)
{
	renderer->tex_y = 0;
	while (renderer->tex_y < weapon->height)
	{
		if (renderer->y + renderer->tex_y >= 0 && renderer->y + renderer->tex_y < DISPLAY_HEIGHT)
			draw_weapon_row(game, renderer, weapon);
		renderer->tex_y++;
	}
}

void	draw_weapon_row(t_game *game, t_render *renderer, t_img *weapon)
{
	renderer->tex_x = 0;
	while (renderer->tex_x < weapon->width)
	{
		if (renderer->x + renderer->tex_x >= 0 && renderer->x + renderer->tex_x < DISPLAY_WIDTH)
			draw_weapon_pixel(game, weapon, renderer, renderer->tex_x, renderer->tex_y);
		renderer->tex_x++;
	}
}

void	draw_weapon_pixel(t_game *game, t_img *weapon, t_render *renderer, int tex_x, int tex_y)
{
	char	*dst;

	renderer->tex_addr = weapon->addr + (tex_y * weapon->line_length +
			tex_x * (weapon->bits_per_pixel / 8));
	renderer->color = *(unsigned int*)renderer->tex_addr;
	renderer->red = (renderer->color >> 16) & 0xFF;
	renderer->green = (renderer->color >> 8) & 0xFF;
	renderer->blue = renderer->color & 0xFF;
	if (!(renderer->red < 10 && renderer->green < 10 && renderer->blue < 10))
	{
		dst = game->screen.addr + ((renderer->y + tex_y) * game->screen.line_length +
				(renderer->x + tex_x) * (game->screen.bits_per_pixel / 8));
		*(unsigned int*)dst = renderer->color;
	}
}