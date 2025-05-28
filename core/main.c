/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 20:57:43 by tcaccava          #+#    #+#             */
/*   Updated: 2025/05/26 22:34:07 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

int	main(int argc, char **argv)
{
	t_game	game;

	if (!init_game_mode(&game, argc, argv))
		return (1);
	init_rays(&game);
	loop_game(&game);
	return (0);
}
