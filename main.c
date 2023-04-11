#include "collider.h"

float get_speed(t_particle *p)
{
	return (sqrtf(powf(p->x_dir, 2) + powf(p->y_dir, 2)) + 0.001f);
}

void wall_collide(t_particle *part, t_settings *settings)
{
	if (part->y >= (float)settings->height - part->r - 1)
	{
		part->y = (float)settings->height - part->r - 1;
		part->y_dir *= -1;
	}
	if (part->x >= (float)settings->width - part->r -1)
	{
		part->x = (float)settings->width - part->r - 1;
		part->x_dir *=-1;
	}
	if (part->x <= part->r + 1)
	{
		part->x = part->r + 1;
		part->x_dir *= -1;
	}
	if (part->y <= part->r + 1)
	{
		part->y = part->r + 1;
		part->y_dir *= -1;
	}
}

void	move(t_particle *part, t_settings *settings)
{
	t_particle_list *cur;

//	cur = find(&settings->grid[(int)(part->x)/5][(int)(part->y) / 5], part);
//	settings->grid[(int)(part->x)/5][(int)(part->y) / 5] = remove_particle(&settings->grid[(int)(part->x)/5][(int)(part->y) / 5], part);
	part->y += part->y_dir;
	part->x += part->x_dir;
	wall_collide(part, settings);
//	add_list_to_list(&settings->grid[(int)(part->x) / 5][(int)(part->y) / 5], cur);

	part->x_dir *= (1 - settings->move_decrease_percentage);
	part->y_dir *= (1 - settings->move_decrease_percentage);
	if (part->x_dir > 0)
		part->x_dir = maxf(0, part->x_dir - settings->move_decrease_absolute);
	else
		part->x_dir = minf(0, part->x_dir + settings->move_decrease_absolute);
	if (part->y_dir > 0)
		part->y_dir = maxf(0, part->y_dir - settings->move_decrease_absolute);
	else
		part->y_dir = minf(0, part->y_dir + settings->move_decrease_absolute);

//	printf("%d\n", (int)get_speed(part));
//	part->color = create_rgba((int)get_speed(part) * 30,255, 255, 255 - (300 / get_speed(part)));
//	part->color = create_rgba((int)get_speed(part) * 300,255, 255, 255);
}

float dist(t_particle *p1, t_particle *p2)
{
	return (sqrtf(powf(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2)));
}

//void solve_collision(t_particle *p1, t_particle *p2)
//{
//	float x_d;
//	float y_d;
//	float collision_angle;
//	float sin_angle;
//	float cos_angle;
//	float overlap;
//
//	if (dist(p1, p2) < p1->r + p2->r)
//	{
//		collision_angle = atan2(p1->y - p2->y, p1->x - p2->x);
//		sin_angle = sin(collision_angle);
//		cos_angle = cos(collision_angle);
//		p1->x_dir = p1->x_dir * cos_angle + p1->y_dir * sin_angle;
//		p1->y_dir = -p1->x_dir * sin_angle + p1->y_dir * cos_angle;
//
//		p2->x_dir = p2->x_dir * cos_angle + p2->y_dir * sin_angle;
//		p2->y_dir = -p2->x_dir * sin_angle + p2->y_dir * cos_angle;
//		overlap = p1->r + p2->r - dist(p1, p2);
//		p1->x -= overlap * p1->x_dir;
//		p1->y -= overlap * p1->y_dir;
//		p2->x += overlap * p2->x_dir;
//		p2->y += overlap * p2->y_dir;
//	}
//}



void solve_collision(t_particle *p1, t_particle *p2) {
	float dx = p1->x - p2->x;
	float dy = p1->y - p2->y;
	float distance = sqrtf(dx*dx + dy*dy);
	float overlap = 0.5f * (distance - p1->r - p2->r);

	if (distance < p1->r + p2->r) {
		p1->x -= overlap * dx / distance;
		p1->y -= overlap * dy / distance;
		p2->x += overlap * dx / distance;
		p2->y += overlap * dy / distance;

		float angle = atan2f(dy, dx);
		float sin_a = sinf(angle);
		float cos_a = cosf(angle);

		float vx1 = p1->x_dir * cos_a + p1->y_dir * sin_a;
		float vy1 = -p1->x_dir * sin_a + p1->y_dir * cos_a;
		float vx2 = p2->x_dir * cos_a + p2->y_dir * sin_a;
		float vy2 = -p2->x_dir * sin_a + p2->y_dir * cos_a;

		float m1 = p1->r;
		float m2 = p2->r;
		float M = m1 + m2;
		float diff_vx = vx1 - vx2;
		float new_vx1 = (vx1 * (m1 - m2) + 2.0f * m2 * vx2) / M;
		float new_vx2 = (vx2 * (m2 - m1) + 2.0f * m1 * vx1) / M;

		p1->x_dir = new_vx1 * cos_a - vy1 * sin_a;
		p1->y_dir = new_vx1 * sin_a + vy1 * cos_a;
		p2->x_dir = new_vx2 * cos_a - vy2 * sin_a;
		p2->y_dir = new_vx2 * sin_a + vy2 * cos_a;
	}
}


void add_gravity(t_particle *p)
{
	p->y_dir += 0.1f;
}

void add_center_gravity(t_particle *p, t_settings *settings)
{
	float d = -dist(p, settings->center);
	p->y_dir += ((p->y - settings->center->y) / (d * d * d)) * settings->gravity_force;
	p->x_dir += ((p->x - settings->center->x) / (d * d * d)) * settings->gravity_force;
}

void move_draw(t_settings *settings,t_particle *p)
{
	draw_circle(settings, (int) p->x, (int) p->y, (int)p->r + 2, SCREEN_COLOR);
	move(p, settings);
	draw_circle(settings, (int)p->x, (int)p->y, (int)p->r, p->color);
}

void	solve_all_collisions(t_settings *settings)
{
	int i;
	int j;
	int steps = settings->sub_steps;

//	printf("%d\n", steps);
	while(steps-- > 0)
	{
		i = 0;
		while (settings->particles[i])
		{
			j = i + 1;
			while (settings->particles[j])
			{
				solve_collision(settings->particles[i], settings->particles[j]);
				j++;

			}
			i++;
		}
	}
}

void solve_all_collisions_grid(t_settings *settings)
{
	t_particle_list *cur;
	t_particle_list *neigh;
	t_particle_list *next_neigh;
	t_particle *p;
	int i;
	int x;
	int y;
	int prev_x;
	int prev_y;

	i = 0;
//	printf("starting all_solution\n");
	while (settings->particles[i])
	{
		p = settings->particles[i];
		prev_x = p->x;
		prev_y = p->y;
		cur = find(&settings->grid[(int)p->x / 2][(int)p->y / 2], p);
		x = (int)maxf(((int)p->x / 5 )-1, 0);

		while (x < ((int)p->x / 5 ) + 2)
		{
//			printf("x = %d\n", x);
			y = (int)maxf(((int)p->y / 5 )-1, 0);
			while (y < ((int)p->y / 5 ) + 2)
			{
//				printf("y = %d\n", y);
				if (y >= 0 && x >= 0)
				{
				neigh = settings->grid[x][y];
				if (neigh && neigh->p != p)
					{
						while(neigh)
						{
							next_neigh = neigh->next;
							if (neigh->p != p)
							{
//								printf("neigh P = %p\n", neigh->p);
								solve_collision(p, neigh->p);
								wall_collide(neigh->p, settings);
								if (((int) (neigh->p->x)) / 5 != x || ((int)( neigh->p->y)) / 5 != y)
								{
//									printf("here0 %d, %d, %d, %d\n", (int)neigh->p->x / 5, ((int)( neigh->p->y)) / 5, x, y);
//									printf("here\n");
									settings->grid[x][y] = remove_particle(&settings->grid[x][y], neigh->p);
									neigh->next = NULL;
//									print_list(settings->grid[x][y]);
//									print_list(settings->grid[(int) neigh->p->x / 5][(int) (neigh->p->y) / 5]);
//									printf("here2 %f, %f\n", neigh->p->x, neigh->p->y);
//									printf("%d %d\n", (int) (neigh->p->x) / 5, (int) (neigh->p->y) / 5);
									settings->grid[(int) neigh->p->x / 5][(int) (neigh->p->y) / 5] = add_list_to_list(&settings->grid[(int) neigh->p->x / 5][(int) neigh->p->y / 5], neigh);
//									print_list(settings->grid[(int) neigh->p->x / 5][(int) (neigh->p->y) / 5]);
//									printf("cur is %p\n", p);
//									printf("here2 %f, %f, %d, %d\n", neigh->p->x / 5, neigh->p->y / 5, x, y);
//									printf("here3\n");
								}
							}
							neigh = next_neigh;
						}
					}
				}

				y++;
			}
			x++;
		}
		if ((int) p->x / 5 != prev_x / 5 || (int)p->y / 5 != prev_y / 5)
		{
			cur = remove_particle(&settings->grid[prev_x][prev_y], p);
			settings->grid[(int)p->x / 5][(int)p->y / 5] = add_list_to_list(&settings->grid[(int)p->x / 5][(int)p->y / 5], neigh);
		}
		i++;
	}
//	printf("ending all_solution\n");
}


void move_all_particles(t_settings * settings)
{
	int i;
	int j;

	i = 0;
//	printf("started move\n");
	solve_all_collisions(settings);
	while(settings->particles[i])
	{
//		printf("started while\n");
		if (settings->center_physical)
		{
//			printf("started if\n");
			solve_collision(settings->particles[i], settings->center);
			settings->center->x = settings->center_x;
			settings->center->y = settings->center_y;
//			printf("finished if\n");
		}
//		printf("started after if\n");
//		add_center_gravity(settings->particles[i], settings);
		add_gravity(settings->particles[i]);
		move_draw(settings, settings->particles[i]);
		i++;
//		printf("finished after if\n");
//		printf("finished while\n");
	}
//	printf("finished move\n");
}

void print_values(t_settings *settings)
{
	printf("substeps = \t\t\t%d\n", settings->sub_steps);
	printf("absolute speed decrease = \t%f\n", settings->move_decrease_absolute);
	printf("percentage speed decrease = \t%f\n", settings->move_decrease_percentage);
	printf("gravity force = \t\t%f\n", settings->gravity_force);
	printf("center is physical = \t\t%d\n", settings->center_physical);
	printf("center size = \t\t\t%f\n", settings->center->r);
	printf("\n");

}

void	ft_hook(void *param)
{
	t_settings		*settings;
	settings = param;

	if (settings->mlx->width != settings->width || settings->mlx->height != settings->height)
	{
		settings->width = settings->mlx->width;
		settings->height = settings->mlx->height;

//		free(settings->image);
		settings->image = mlx_new_image(settings->mlx, settings->width, settings->height);
		mlx_image_to_window(settings->mlx, settings->image, 0, 0);
		fill_screen(settings, SCREEN_COLOR);
	}
	if (mlx_is_key_down(settings->mlx, MLX_KEY_UP))
		settings->center_y -= 1;
	if (mlx_is_key_down(settings->mlx, MLX_KEY_DOWN))
		settings->center_y += 1;
	if (mlx_is_key_down(settings->mlx, MLX_KEY_LEFT))
		settings->center_x -= 1;
	if (mlx_is_key_down(settings->mlx, MLX_KEY_RIGHT))
		settings->center_x += 1;
	settings->center->x = settings->center_x;
	settings->center->y = settings->center_y;


	move_all_particles(settings);
	draw_circle(settings, settings->center_x, settings->center_y, settings->center->r, settings->center->color);
	usleep(1000);
}

void	ft_key_hook(mlx_key_data_t key, void *param)
{
	t_settings *settings;

	settings = param;
	static bool press;
	static long long pressed_time;

	if (pressed_time == 0)
		pressed_time = get_other_time();

	if ( pressed_time < get_other_time() - 100 && (key.action == MLX_PRESS || key.action == MLX_REPEAT)) {
		if (mlx_is_key_down(settings->mlx, MLX_KEY_ESCAPE))
			mlx_close_window(settings->mlx);


		if (mlx_is_key_down(settings->mlx, MLX_KEY_T))
			settings->sub_steps += 1;
		if (mlx_is_key_down(settings->mlx, MLX_KEY_G))
			settings->sub_steps -= 1;

		if (mlx_is_key_down(settings->mlx, MLX_KEY_Q))
			settings->move_decrease_absolute += 0.001f;
		if (mlx_is_key_down(settings->mlx, MLX_KEY_A))
			settings->move_decrease_absolute -= 0.001f;
		if (mlx_is_key_down(settings->mlx, MLX_KEY_W))
			settings->move_decrease_percentage += 0.001f;
		if (mlx_is_key_down(settings->mlx, MLX_KEY_S))
			settings->move_decrease_percentage -= 0.001f;
		if (mlx_is_key_down(settings->mlx, MLX_KEY_E))
			settings->gravity_force += 10;
		if (mlx_is_key_down(settings->mlx, MLX_KEY_D))
			settings->gravity_force -= 10;
		if (mlx_is_key_down(settings->mlx, MLX_KEY_SPACE))
			settings->center_physical ^= true;
		if (mlx_is_key_down(settings->mlx, MLX_KEY_R))
			settings->center->r += 1;
		if (mlx_is_key_down(settings->mlx, MLX_KEY_F))
			settings->center->r -= 1;
//		if (mlx_is_key_down(settings->mlx, MLX_KEY_SPACE))
			print_values(settings);
		pressed_time = get_other_time();
	}
}


t_particle *make_particle(float x, float y, float r, float x_dir, float y_dir, uint32_t color)
{
	t_particle *p;

	p = malloc(sizeof(t_particle));
	p->x = x;
	p->y = y;
	p->r = r;
	p->x_dir = x_dir;
	p->y_dir = y_dir;
	p->color = color;
	return (p);
}

int	main()
{
	t_settings	*settings;
	t_particle *p;
	t_particle *p2;

	settings = init_settings();

	settings->particles = malloc(sizeof(t_particle) * 1001);
	for (int k = 0; k < 10; k++)
	{
		for (int j = 0; j < 10; j++)
		{
			for (int i = 0; i < 10; i++)
			{
				settings->particles[i + (j * 10) + k * 100] = make_particle(4 + i, 4 + 3 * (j + k * 10), 2 + (i % 3), 1, 0,
																  create_rgba((30 * j) % 256, (30 * i) % 256, (k * 30) % 256, 255));
//				add_list_to_list(&settings->grid[(4 + i) / 5][(4 + 3 * (j + k * 10)) / 5], make_list(settings->particles[i + (j * 10) + k * 100], NULL));

			}
		}
	}
	int i = 0;
	while (settings->particles[i])
	{
		settings->grid[(int)(settings->particles[i]->x) / 5][(int)(settings->particles[i]->y) / 5] =
				add_list_to_list(&settings->grid[(int)(settings->particles[i]->x) / 5][(int)(settings->particles[i]->y) / 5], make_list(settings->particles[i], NULL));
		i++;
	}
	settings->particles[1001] = NULL;

	settings->center_x = settings->height / 2;
	settings->center_y = settings->width / 2;
	settings->center = make_particle(settings->height / 2, settings->width / 2, 0, 0, 0, create_rgba(255, 255, 255, 255));
	settings->gravity_force = 0.1f;
	settings->center_physical = false;

	fill_screen(settings, SCREEN_COLOR);
	if (settings)
	{

		mlx_key_hook(settings->mlx, ft_key_hook, settings);
		mlx_loop_hook(settings->mlx, ft_hook, settings);
		mlx_loop(settings->mlx);
//		clean_settings(settings);
		return (0);
	}
	return (1);
}