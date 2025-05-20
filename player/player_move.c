/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 17:44:16 by tcaccava          #+#    #+#             */
/*   Updated: 2025/05/18 18:33:20 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cube3d.h"

int	key_release(int keycode, t_player *player)
{
	if (keycode == W)
		player->key_up = false;
	if (keycode == S)
		player->key_down = false;
	if (keycode == A)
		player->key_left = false;
	if (keycode == D)
		player->key_right = false;
	if (keycode == Q)
		player->left_rotate = false;
	if (keycode == E)
		player->right_rotate = false;
	if (keycode == R)
		player->turn_back = false;
	if (keycode == LEFT)
		player->left = false;
	if (keycode == RIGHT)
		player->right = false;
	return (0);
}

int	key_press(int keycode, t_player *player)
{
	printf("Keycode pressed: %d\n", keycode);
	if (keycode == ESC)
		close_window(NULL);
	if (keycode == W)
		player->key_up = true;
	if (keycode == S)
		player->key_down = true;
	if (keycode == A)
		player->key_left = true;
	if (keycode == D)
		player->key_right = true;
	if (keycode == Q)
		player->left_rotate = true;
	if (keycode == E)
		player->right_rotate = true;
	if (keycode == R)
		player->turn_back = true;
	if (keycode == LEFT)
		player->left = true;
	if (keycode == RIGHT)
		player->right = true;
	if (keycode == 49)
	{
		player->current_weapon = PORTALGUN;
		if (player->game)
			player->game->current_weapon = PORTALGUN;
	}
	if (keycode == 50)
	{
		player->current_weapon = RAYGUN;
		if (player->game)
			player->game->current_weapon = RAYGUN;
	}
	if (keycode == 32)
	{
		if (player->game)
			check_door_interaction(player->game);
	}
	// if (player->current_weapon >= MAX_WEAPONS)
	//     player->current_weapon = 0;
	return (0);
}
int	mouse_move(int x, int y, t_game *game)
{
	int		center_x;
	int		center_y;
	int		delta_x;
	int		delta_y;
	double	sensibility;

	center_x = DISPLAY_WIDTH / 2;
	center_y = DISPLAY_HEIGHT / 2;
	if (x == center_x && y == center_y)
		return (0);
	delta_x = x - center_x;
	delta_y = y - center_y;
	sensibility = 0.003;
	game->player.angle += delta_x * sensibility;
	game->player.angle = normalize_angle(game->player.angle);
	game->pitch -= delta_y;
	if (game->pitch > 300)
		game->pitch = 300;
	if (game->pitch < -300)
		game->pitch = -300;
	mlx_mouse_move(game->mlx, game->win, center_x, center_y);
	return (0);
}


int is_traversable(t_map *map, double x, double y)
{
    int map_x = (int)(x / TILE_SIZE);
    int map_y = (int)(y / TILE_SIZE);
    
    if (map_x < 0 || map_x >= map->width || map_y < 0 || map_y >= map->height)
        return (0);
    
    char cell = map->matrix[map_y][map_x];
    
    // portes ouvertes 'O' sont traversables
    if (cell == '0' || cell == 'O')
        return (1);
    
    return (0);
}

void	move_player(t_player *player)
{
	float	angle_speed;
	float	speed;
	float	cos_angle;
	float	sin_angle;
	float	new_x;
	float	new_y;
	float	strafe_angle;


	angle_speed = 0.05;
	speed = 10.0;
	cos_angle = cos(player->angle);
	sin_angle = sin(player->angle);
	if (player->left_rotate)
		player->angle -= angle_speed;
	if (player->right_rotate)
		player->angle += angle_speed;
	if (player->turn_back)
		player->angle += M_PI;
	while (player->angle < 0)
		player->angle += 2 * M_PI;
	while (player->angle >= 2 * M_PI)
		player->angle -= 2 * M_PI;
	if (player->key_up)
	{
		new_x = player->x + cos_angle * speed;
		new_y = player->y + sin_angle * speed;
		if (player->game && is_traversable(&player->game->map, new_x, player->y))
			player->x = new_x;
		if (player->game && is_traversable(&player->game->map, player->x, new_y))
			player->y = new_y;
	}
	if (player->key_down)
	{
		new_x = player->x - cos_angle * speed;
		new_y = player->y - sin_angle * speed;
		if (player->game && is_traversable(&player->game->map, new_x, player->y))
			player->x = new_x;
		if (player->game && is_traversable(&player->game->map, player->x, new_y))
			player->y = new_y;
	}
	if (player->key_left)
	{
		strafe_angle = player->angle - M_PI / 2;
		new_x = player->x + cos(strafe_angle) * speed;
		new_y = player->y + sin(strafe_angle) * speed;
		if (player->game && is_traversable(&player->game->map, new_x, player->y))
			player->x = new_x;
		if (player->game && is_traversable(&player->game->map, player->x, new_y))
			player->y = new_y;
	}
	if (player->key_right)
	{
		strafe_angle = player->angle + M_PI / 2;
		new_x = player->x + cos(strafe_angle) * speed;
		new_y = player->y + sin(strafe_angle) * speed;
		if (player->game && is_traversable(&player->game->map, new_x, player->y))
			player->x = new_x;
		if (player->game && is_traversable(&player->game->map, player->x, new_y))
			player->y = new_y;
	}
}
