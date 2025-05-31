#include "cube3d.h"

void	setup_weapon_pickup_render(t_game *game, t_render *renderer)
{
	setup_enemy_render_params(game, renderer);
	renderer->draw_end += 400;
}

void	draw_weapon_pickup_sprite(t_game *game,
		t_img *sprite, t_point pos, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		draw_weapon_pickup_row(game, sprite, pos, size, i);
		i++;
	}
}

void	draw_weapon_pickup_row(t_game *game, t_img *sprite,
		t_point pos, int size, int i)
{
	int	j;

	j = 0;
	while (j < size)
	{
		draw_weapon_pickup_pixel(game, sprite, pos, size, i, j);
		j++;
	}
}

void	draw_weapon_pickup_pixel(t_game *game,
		t_img *sprite, t_point pos, int size, int i, int j)
{
	int				src_x;
	int				src_y;
	int				x;
	int				y;
	char			*src;
	char			*dst;
	unsigned int	color;
	int				skip_pixel;

	src_x = i * sprite->width / size;
	src_y = j * sprite->height / size;
	if (src_x >= sprite->width)
		src_x = sprite->width - 1;
	if (src_y >= sprite->height)
		src_y = sprite->height - 1;
	src = sprite->addr + src_y * sprite->line_length
			+ src_x * (sprite->bits_per_pixel / 8);
	color = *(unsigned int *)src;
	skip_pixel = is_pickup_pixel_transparent(color);
	if (!skip_pixel)
	{
		x = pos.x + i;
		y = pos.y + j;
		if (x >= 0 && x < DISPLAY_WIDTH && y >= 0 && y < DISPLAY_HEIGHT)
		{
			dst = game->screen.addr + y * game->screen.line_length
				+ x * (game->screen.bits_per_pixel / 8);
			*(unsigned int *)dst = color;
		}
	}
}

int	is_pickup_pixel_transparent(unsigned int color)
{
	int	red;
	int	green;
	int	blue;
	int	tolerance;

	red = (color >> 16) & 0xFF;
	green = (color >> 8) & 0xFF;
	blue = color & 0xFF;
	tolerance = 2;
	if (red < 10 && green < 10 && blue < 10)
		return (1);
	if (abs(red - 255) <= tolerance && abs(green - 0)
		<= tolerance && abs(blue - 0) <= tolerance)
		return (1);
	if (red >= 250 && green >= 160 && green <= 175)
		return (1);
	return (0);
}
