/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_checker.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 18:40:07 by mbonnet           #+#    #+#             */
/*   Updated: 2021/11/25 21:44:04 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	my_write_2(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->info->check_write);
	printf("%lld\t%d\t%s\n", get_time()
		- philo->info->time_starte, philo->id + 1, str);
	pthread_mutex_unlock(&philo->info->check_write);
	return (1);
}

void	my_died_shot(t_philo *philo, int alive)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&philo->check_alive);
	philo->alive = alive;
	pthread_mutex_unlock(&philo->check_alive);
	pthread_mutex_lock(&philo->info->check_alive);
	philo->info->alive = -1;
	pthread_mutex_unlock(&philo->info->check_alive);
	while (i < philo->info->nb_philo)
	{
		pthread_mutex_lock(&philo->info->philos[i].check_alive);
		if (philo->info->philos[i].alive > 0)
			philo->info->philos[i].alive = -3;
		pthread_mutex_unlock(&philo->info->philos[i].check_alive);
		i++;
	}
}

int	check_time_last_eat(t_info *info)
{
	long long int	time_last_eat;
	int				i;
	long long int	big_time;
	long long int	time;

	i = 0;
	big_time = 0;
	while (i < info->nb_philo)
	{
		pthread_mutex_lock(&(info->philos[i].check_last_eat));
		time_last_eat = info->philos[i].time_last_eat;
		pthread_mutex_unlock(&(info->philos[i].check_last_eat));
		time = get_time() - time_last_eat;
		if (time > info->time_die)
		{
			my_died_shot(&info->philos[i], -1);
			return (-1);
		}
		if (time > big_time)
			big_time = time;
		i++;
	}
	return (time);
}

int	check_philo_alive(t_philo *philo)
{
	int	philo_alive;

	pthread_mutex_lock(&philo->check_alive);
	philo_alive = philo->alive;
	pthread_mutex_unlock(&philo->check_alive);
	if (philo_alive < 0)
		return (-1);
	else
		return (1);
}
