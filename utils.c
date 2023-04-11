#include "collider.h"

void	put_pixel(t_settings *settings, int x, int y, uint32_t color)
{
	if (x > 0 && y > 0 && x < settings->width - 1 && y < settings->height - 1)
	mlx_put_pixel(settings->image, x, y, color);
}

t_particle_list ***init_grid()
{
	t_particle_list ***res;
	int i;
	int j;

	res = malloc(sizeof(t_particle_list **) * 601);
	i = 0;
	while (i < 600)
	{
		res[i] = malloc(sizeof(t_particle_list) * 601);
		j = 0;
		while (j < 601)
		{
			res[i][j] = NULL;
			j++;
		}
		i++;
	}
	res[i] = NULL;
	return (res);
}

t_settings	*init_settings()
{
	t_settings	*res;

	res = malloc(sizeof(t_settings));

	res->mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true);
	res->height = HEIGHT;
	res->width = WIDTH;
	res->image = mlx_new_image(res->mlx, WIDTH, HEIGHT);
	res->sub_steps = 4;
	res->grid = init_grid();
	mlx_image_to_window(res->mlx, res->image, 0, 0);
	res->move_decrease_absolute = 0.001f;
	res->move_decrease_percentage = 0.001f;
	return (res);
}

unsigned long	create_rgba(unsigned int r, unsigned int g, unsigned int b, unsigned int t)
{

	if (r > 255)
		r = 255;
	if (t < 0)
		t = 0;
	if (r > 255 || g > 255 || b > 255)
	{
		r = 0;
		g = 0;
		b = 0;
	}

	return (((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8)
			+ (t & 0xff));
}

void draw_circle(t_settings *settings, int centerX, int centerY, int radius, uint32_t color) {
	int x, y, r2;
	r2 = radius * radius;

	for (y = centerY - radius; y <= centerY + radius; y++) {
		for (x = centerX - radius; x <= centerX + radius; x++) {
			int dx = x - centerX;
			int dy = y - centerY;

			if ((dx*dx + dy*dy) <= r2) {
				put_pixel(settings, x, y, color);
			}
		}
	}
}

void fill_screen(t_settings *settings, uint32_t color)
{
	int i = 1;
	int j;

	while (i < settings->height - 1)
	{
		j = 1;
		while (j < settings->width - 1)
		{
			put_pixel(settings, j, i, color);
			j++;
		}
		i++;
	}
}
