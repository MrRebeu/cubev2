#include "../cube3d.h"

int	load_single_weapon_texture(void *mlx, t_img *tex, char *path)
{
	int	width;
	int	height;

	tex->ptr = mlx_xpm_file_to_image(mlx, path, &width, &height);
	if (!tex->ptr)
	{
		printf("Error loading texture: %s\n", path);
		return (0);
	}
	tex->addr = mlx_get_data_addr(tex->ptr, &tex->bits_per_pixel,
			&tex->line_length, &tex->endian);
	tex->width = width;
	tex->height = height;
	return (1);
}

int	load_weapon_textures(void *mlx, t_img weapon_textures[3], int weapon_type)
{
	const char	*paths[MAX_WEAPONS][3];
	int			i;

	init_weapon_paths(paths);
	i = 0;
	while (i < 3)
	{
		if (!load_single_weapon_texture(mlx, &weapon_textures[i],
				(char *)paths[weapon_type][i]))
			return (0);
		i++;
	}
	return (1);
}

void	init_weapon_paths(const char paths[MAX_WEAPONS][3])
{
	init_hands_paths(paths[HANDS]);
	init_portalgun_paths(paths[PORTALGUN]);
	init_raygun_paths(paths[RAYGUN]);
	init_healgun_paths(paths[HEALGUN]);
}

void	init_hands_paths(const char paths[3])
{
	paths[0] = "./texture/hands.xpm";
	paths[1] = "./texture/hands.xpm";
	paths[2] = "./texture/hands.xpm";
}

void	init_portalgun_paths(const char paths[3])
{
	paths[0] = "./texture/w_portalgun.xpm";
	paths[1] = "./texture/w_portalgun.xpm";
	paths[2] = "./texture/w_portalgun.xpm";
}
void	init_raygun_paths(const char paths[3])
{
	paths[0] = "./texture/w_raygun.xpm";
	paths[1] = "./texture/w_raygun_prefire.xpm";
	paths[2] = "./texture/w_raygun_fire.xpm";
}

void	init_healgun_paths(const char paths[3])
{
	paths[0] = "./texture/w_healgun.xpm";
	paths[1] = "./texture/w_healgun.xpm";
	paths[2] = "./texture/w_healgun.xpm";
}