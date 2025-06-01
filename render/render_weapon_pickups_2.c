#include "../cube3d.h"

static int	is_pixel_transparent(unsigned int color)
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
	if (abs(red - 255) <= tolerance && abs(green - 0) <= tolerance
		&& abs(blue - 0) <= tolerance)
		return (1);
	if (red >= 250 && green >= 160 && green <= 175)
		return (1);
	return (0);
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

void	calculate_weapon_screen_pos(t_game *game, t_render *render)
{
	double	distance_to_projection_plane;
	double	corrected_dist;
	double	weapon_height;

	weapon_height = TILE_SIZE * 0.1;
	render->x = (int)((DISPLAY_WIDTH / 2) * (1 + render->floor_x
				/ render->floor_y));
	distance_to_projection_plane = (DISPLAY_WIDTH / 2.0)
		/ tan(game->player.fov / 2.0);
	corrected_dist = fmax(render->floor_y, 0.1);
	render->sprite_size = (int)((weapon_height / corrected_dist)
			* distance_to_projection_plane);
	if (render->sprite_size > DISPLAY_HEIGHT * 2)
		render->sprite_size = DISPLAY_HEIGHT * 2;
	if (render->sprite_size < 1)
		render->sprite_size = 1;
}

static void	draw_pickup_pixel(t_game *game, t_weapon_pixel_data *data)
{
	int				coords[4];
	char			*src;
	char			*dst;
	unsigned int	color;

	coords[0] = data->i * data->sprite->width / data->size;
	coords[1] = data->j * data->sprite->height / data->size;
	if (coords[0] >= data->sprite->width)
		coords[0] = data->sprite->width - 1;
	if (coords[1] >= data->sprite->height)
		coords[1] = data->sprite->height - 1;
	src = data->sprite->addr + coords[1] * data->sprite->line_length
		+ coords[0] * (data->sprite->bits_per_pixel / 8);
	color = *(unsigned int *)src;
	if (!is_pixel_transparent(color))
	{
		coords[2] = data->pos.x + data->i;
		coords[3] = data->pos.y + data->j;
		if (coords[2] >= 0 && coords[2] < DISPLAY_WIDTH
			&& coords[3] >= 0 && coords[3] < DISPLAY_HEIGHT)
		{
			dst = game->screen.addr + coords[3] * game->screen.line_length
				+ coords[2] * (game->screen.bits_per_pixel / 8);
			*(unsigned int *)dst = color;
		}
	}
}

void	draw_weapon_pickup_sprite(t_game *game, t_img *sprite, t_point pos,
		int size)
{
	t_weapon_pixel_data	data;
	int					i;
	int					j;

	if (!sprite || !sprite->addr || size <= 0)
		return ;
	data.sprite = sprite;
	data.pos = pos;
	data.size = size;
	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < size)
		{
			data.i = i;
			data.j = j;
			draw_pickup_pixel(game, &data);
			j++;
		}
		i++;
	}
}
