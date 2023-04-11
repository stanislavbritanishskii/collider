#ifndef COLLIDER_H
#define COLLIDER_H

# include "libs/MLX42/include/MLX42/MLX42.h"
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <sys/time.h>
# include <unistd.h>
# include <math.h>

# define HEIGHT			300
# define WIDTH			300
# define SCREEN_COLOR	0x000000FF

typedef struct s_particle
{
	float x;
	float y;
	float r;
	uint32_t color;
	float x_dir;
	float y_dir;

}	t_particle;

typedef struct s_particle_list
{
	t_particle *p;
	struct s_particle_list *next;
} t_particle_list;

typedef struct s_settings
{
	mlx_t			*mlx;
	mlx_image_t		*image;
	t_particle		**particles;
	t_particle		*center;
	float			center_x;
	float			center_y;
	float 			gravity_force;
	bool			center_physical;
	int 			width;
	int 			height;
	int				sub_steps;
	float move_decrease_percentage;
	float move_decrease_absolute;
	t_particle_list ***grid;

}	t_settings;


void	put_pixel(t_settings *settings, int x, int y, uint32_t color);
t_settings	*init_settings();
unsigned long	create_rgba(unsigned int r, unsigned int g, unsigned int b, unsigned int t);
void draw_circle(t_settings *settings, int centerX, int centerY, int radius, uint32_t color);
void fill_screen(t_settings *settings, uint32_t color);

//
void solve_collision(t_particle *p1, t_particle *p2);

// math_utils.c

float minf(float a, float b);
float maxf(float a, float b);

// times.c
int	get_other_time();

// particle_list.c

t_particle_list *make_list(t_particle *p, t_particle_list *next);
t_particle_list	*remove_particle(t_particle_list **list, t_particle *p);
t_particle_list *add_list_to_list(t_particle_list **big, t_particle_list *to_add);
t_particle_list *find(t_particle_list **list, t_particle *p);
void	for_each_solve_collisions(t_particle_list **list, t_particle *p);
void free_list(t_particle_list *list);
void print_list(t_particle_list *list);

#endif