#include "../cube3d.h"

int	load_healgun(t_game *game)
{
	const char	*healgun_sprites[5];
	int			i;

	init_healgun_sprite_paths(healgun_sprites);
	i = 0;
	while (i < 5)
	{
		if (!load_healgun_frame(game, i, healgun_sprites[i]))
			return (0);
		i++;
	}
	return (1);
}

void	init_healgun_sprite_paths(const char *sprites[5])
{
	sprites[0] = "./texture/healgun.xpm";
	sprites[1] = "./texture/healgun_1.xpm";
	sprites[2] = "./texture/healgun_2.xpm";
	sprites[3] = "./texture/healgun_3.xpm";
	sprites[4] = "./texture/healgun_4.xpm";
}

int	load_healgun_frame(t_game *game, int frame, const char *path)
{
	if (!load_single_weapon_texture(game->mlx, &game->healgun_frames[frame],
			(char *)path))
	{
		printf("Erreur: %s non trouvé\n", path);
		return (0);
	}
	printf("✅ Chargé: %s\n", path);
	return (1);
}

void	check_reload_healgun(t_game *game)
{
	if (game->player.healgun_ammo > 0)
	{
		game->player.healgun_is_loaded = 1;
		printf("🔋 Heal Gun rechargé automatiquement !\n");
	}
}

void	start_healgun_animation(t_game *game)
{
	if (game->player.healgun_animating)
		return ;
	if (!validate_healgun_use(game))
	{
		printf("❌ Heal Gun vide !\n");
		return ;
	}
	init_healgun_animation(game);
}
