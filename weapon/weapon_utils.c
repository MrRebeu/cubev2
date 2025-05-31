
#include "../cube3d.h"

void	handle_scroll_wheel(t_game *game, int button)
{
	if (button == 4)
		switch_to_next_weapon(game);
	else
		switch_to_previous_weapon(game);
}

void	switch_to_next_weapon(t_game *game)
{
	int	next_weapon;

	next_weapon = (game->current_weapon + 1) % MAX_WEAPONS;
	while (!game->player.has_weapon[next_weapon] && next_weapon != game->current_weapon)
		next_weapon = (next_weapon + 1) % MAX_WEAPONS;
	if (game->player.has_weapon[next_weapon])
	{
		game->current_weapon = next_weapon;
		game->player.current_weapon = next_weapon;
		printf("Weapon changed (scroll): %d\n", next_weapon);
	}
}

void	switch_to_previous_weapon(t_game *game)
{
	int	prev_weapon;

	prev_weapon = (game->current_weapon - 1 + MAX_WEAPONS) % MAX_WEAPONS;
	while (!game->player.has_weapon[prev_weapon] && prev_weapon != game->current_weapon)
		prev_weapon = (prev_weapon - 1 + MAX_WEAPONS) % MAX_WEAPONS;
	if (game->player.has_weapon[prev_weapon])
	{
		game->current_weapon = prev_weapon;
		game->player.current_weapon = prev_weapon;
		printf("Weapon changed (scroll): %d\n", prev_weapon);
	}
}

void	disable_weapon_pickup_at_position(t_game *game, int map_x, int map_y, int weapon_type)
{
	int	i;

	i = 0;
	while (i < game->num_weapon_pickup)
	{
		if (game->weapon_pickup[i].active)
		{
			if (is_weapon_at_position(&game->weapon_pickup[i], map_x, map_y, weapon_type))
			{
				game->weapon_pickup[i].active = 0;
				printf("Weapon disabled: type=%d at [%d,%d]\n", weapon_type, map_x, map_y);
				return ;
			}
		}
		i++;
	}
	printf("Weapon not found: type=%d at [%d,%d]\n", weapon_type, map_x, map_y);
}

int	is_weapon_at_position(t_weapon_pickup *weapon, int map_x, int map_y, int weapon_type)
{
	int	weapon_map_x;
	int	weapon_map_y;

	weapon_map_x = (int)(weapon->x / TILE_SIZE);
	weapon_map_y = (int)(weapon->y / TILE_SIZE);
	if (weapon_map_x == map_x && weapon_map_y == map_y && weapon->weapon_type == weapon_type)
		return (1);
	return (0);
}
void	setup_weapon_at_position(t_game *game, int x, int y, int *weapon_index)
{
	char	cell;

	game->weapon_pickup[*weapon_index].x = (x * TILE_SIZE) + (TILE_SIZE / 2);
	game->weapon_pickup[*weapon_index].y = (y * TILE_SIZE) + (TILE_SIZE / 2);
	cell = game->map.matrix[y][x];
	if (cell == 'R')
		setup_raygun_pickup(game, *weapon_index);
	else if (cell == 'G')
		setup_portalgun_pickup(game, *weapon_index);
	else if (cell == 'H')
		setup_healgun_pickup(game, *weapon_index);
	game->weapon_pickup[*weapon_index].active = 1;
	(*weapon_index)++;
}

void	setup_raygun_pickup(t_game *game, int index)
{
	game->weapon_pickup[index].weapon_type = RAYGUN;
	if (!load_weapon_pickup_sprite(game, &game->weapon_pickup[index], 
			"./texture/raygun_pickup.xpm"))
		printf("Warning: raygun_pickup.xpm not found\n");
}

void	setup_portalgun_pickup(t_game *game, int index)
{
	game->weapon_pickup[index].weapon_type = PORTALGUN;
	if (!load_weapon_pickup_sprite(game, &game->weapon_pickup[index], 
			"./texture/portalgun_pickup.xpm"))
		printf("Warning: portalgun_pickup.xpm not found\n");
}

void	setup_healgun_pickup_weapon(t_game *game, int index)
{
	game->weapon_pickup[index].weapon_type = HEALGUN;
	if (!load_weapon_pickup_sprite(game, &game->weapon_pickup[index], 
			"./texture/healgun_pickup.xpm"))
		printf("Warning: healgun_pickup.xpm not found\n");
}