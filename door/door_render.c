#include "../cube3d.h"

void	render_door(t_game *game, int column_x, t_render *renderer, t_ray *ray)
{
	calculate_door_texture_coordinates(renderer, ray);
	render_door_column(game, column_x, renderer);
}

void	render_open_door(t_game *game, int column_x, t_render *renderer, t_ray *ray)
{
	double	pos_in_cell;

	calculate_door_texture_coordinates(renderer, ray);
	pos_in_cell = get_position_in_cell(ray);
	if (is_door_frame_position(pos_in_cell))
		render_door_column(game, column_x, renderer);
}

void	calculate_door_texture_coordinates(t_render *renderer, t_ray *ray)
{
	if (ray->hit_vertical)
		renderer->tex_x = (int)(ray->wall_hit_y) % TILE_SIZE;
	else
		renderer->tex_x = (int)(ray->wall_hit_x) % TILE_SIZE;
}

double	get_position_in_cell(t_ray *ray)
{
	double	pos_in_cell;

	if (ray->hit_vertical)
		pos_in_cell = fmod(ray->wall_hit_y, TILE_SIZE);
	else
		pos_in_cell = fmod(ray->wall_hit_x, TILE_SIZE);
	if (pos_in_cell < 0)
		pos_in_cell += TILE_SIZE;
	return (pos_in_cell);
}

int	is_door_frame_position(double pos_in_cell)
{
	double	edge_size;

	edge_size = TILE_SIZE * 0.2;
	if (pos_in_cell <= edge_size)
		return (1);
	if (pos_in_cell >= TILE_SIZE - edge_size)
		return (1);
	return (0);
}



static void	render_shooted_door_column(t_game *game, t_render *renderer, 
				int column_x, int cy, double h)
{
	int				texture_y;
	float			rel;
	unsigned int	damaged_color;

	renderer->y = renderer->draw_start;
	while (renderer->y <= renderer->draw_end)
	{
		if (renderer->y >= 0 && renderer->y < DISPLAY_HEIGHT)
		{
			rel = ((renderer->y - cy) / h) + 0.5f;
			texture_y = (int)(rel * TILE_SIZE);
			if (texture_y < 0)
				texture_y = 0;
			else if (texture_y >= TILE_SIZE)
				texture_y = TILE_SIZE - 1;
			renderer->tex_addr = game->map.door_shooted_texture.addr
				+ (texture_y * game->map.door_shooted_texture.line_length
				+ renderer->tex_x * (game->map.door_shooted_texture.bits_per_pixel / 8));
			renderer->color = *(unsigned int *)renderer->tex_addr;
			apply_damage_effect(renderer->color, &damaged_color);
			renderer->screen_pixel = game->screen.addr
				+ (renderer->y * game->screen.line_length
				+ column_x * (game->screen.bits_per_pixel / 8));
			*(unsigned int *)renderer->screen_pixel = damaged_color;
		}
		renderer->y++;
	}
}

void	render_shooted_open_door(t_game *game, int column_x, 
			t_render *renderer, t_ray *ray)
{
	int		cy;
	double	h;
	double	cell_pos;
	double	frame_thickness;
	int		is_on_frame;

	cy = (DISPLAY_HEIGHT / 2) + game->pitch;
	h = renderer->wall_height;
	if (ray->hit_vertical)
	{
		renderer->tex_x = (int)(ray->wall_hit_y) % TILE_SIZE;
		if (cos(ray->radiant_angle) > 0)
			renderer->tex_x = TILE_SIZE - renderer->tex_x - 1;
	}
	else
	{
		renderer->tex_x = (int)(ray->wall_hit_x) % TILE_SIZE;
		if (sin(ray->radiant_angle) < 0)
			renderer->tex_x = TILE_SIZE - renderer->tex_x - 1;
	}
	cell_pos = (double)renderer->tex_x / TILE_SIZE;
	frame_thickness = 0.2;
	is_on_frame = (cell_pos < frame_thickness) || (cell_pos > 1.0 - frame_thickness);
	if (!is_on_frame)
		return ;
	render_shooted_door_column(game, renderer, column_x, cy, h);
}