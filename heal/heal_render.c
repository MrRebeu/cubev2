
#include "../cube3d.h"

void	render_healgun_animation(t_game *game)
{
	t_img	*arm_sprite;
	int		arm_x;
	int		arm_y;

	arm_sprite = &game->map.arm_1;
	arm_x = 50;
	arm_y = DISPLAY_HEIGHT - arm_sprite->height - 50;
	draw_arm_sprite(game, arm_sprite, arm_x, arm_y);
}

void	draw_arm_sprite(t_game *game, t_img *sprite, int x, int y)
{
	int	pixel_y;

	pixel_y = 0;
	while (pixel_y < sprite->height)
	{
		draw_arm_row(game, sprite, x, y, pixel_y);
		pixel_y++;
	}
}

void	draw_arm_row(t_game *game, t_img *sprite, int x, int y, int row)
{
	int	pixel_x;

	pixel_x = 0;
	while (pixel_x < sprite->width)
	{
		draw_arm_pixel(game, sprite, x, y, pixel_x, row);
		pixel_x++;
	}
}

void	draw_arm_pixel(t_game *game, t_img *sprite, int x, int y, int px, int py)
{
	char			*src;
	char			*dst;
	unsigned int	color;
	int				screen_x;
	int				screen_y;

	screen_x = x + px;
	screen_y = y + py;
	if (!is_pixel_in_bounds(screen_x, screen_y))
		return ;
	src = get_sprite_pixel(sprite, px, py);
	color = *(unsigned int *)src;
	if (is_transparent_pixel(color))
		return ;
	dst = get_screen_pixel(game, screen_x, screen_y);
	*(unsigned int *)dst = color;
}

int	is_transparent_pixel(unsigned int color)
{
	int	red;
	int	green;
	int	blue;

	red = (color >> 16) & 0xFF;
	green = (color >> 8) & 0xFF;
	blue = color & 0xFF;
	if (red < 10 && green < 10 && blue < 10)
		return (1);
	return (0);
}