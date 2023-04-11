#include "collider.h"

t_particle_list *make_list(t_particle *p, t_particle_list *next)
{
	t_particle_list *res;

	res = malloc(sizeof(t_particle_list));
	res->p = p;
	res->next = next;
	return (res);
}

t_particle_list	*remove_particle(t_particle_list **list, t_particle *p)
{
	t_particle_list *cur;
	t_particle_list *prev;

	if (!list)
		return (NULL);
	prev = *list;
	if (!prev)
		return NULL;
	cur = prev->next;
	if (prev != NULL && prev->p == p)
	{
		*list = cur;
		prev->next = NULL;
		return (*list);
	}
	while (cur)
	{
		if (cur->p == p)
		{
			prev->next = cur->next;
			cur->next = NULL;
			return (*list);
		}
		prev = cur;
		cur = prev->next;
	}
	return (NULL);
}

t_particle_list *add_list_to_list(t_particle_list **big, t_particle_list *to_add)
{
	t_particle_list *cur;
	if (!big || !*big)
	{
		*big = to_add;
		return (*big);
	}

	cur = *big;
	while (cur->next)
		cur = cur->next;
	cur->next = to_add;
	return (*big);
}

t_particle_list *find(t_particle_list **list, t_particle *p)
{
	t_particle_list *cur;
	if (!list || !*list)
		return (NULL);
	cur = *list;
	while (cur)
	{
		if (cur->p == p)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

void	for_each_solve_collisions(t_particle_list **list, t_particle *p)
{
	t_particle_list *cur;
	if (!list || !*list)
		return ;
	cur = *list;
	while (cur)
	{
		solve_collision(p, cur->p);
		cur = cur->next;
	}
}

void free_list(t_particle_list *list)
{
	if (list)
	{
		free_list(list->next);
		free(list);
	}
}

void print_list(t_particle_list *list)
{
	t_particle_list *cur;
	cur = list;
	while (cur)
	{
		printf("%p ->", cur->p);
		cur = cur->next;
	}
	printf("NULL\n");
}