/*
 * File:   main.c
 * Author: bocal
 *
 * Created on July 12, 2018, 3:19 PM
 */

#include <stdlib.h>
#include <math.h>
#include "types.h"

unsigned long int		ft_strlen(char *s)
{
	int count;

	count = 0;
	if (!s)
		return (0);
	while (s[count])
	{
		count++;
	}
	return (count);
}

int             ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char *new_s1;
	unsigned char *new_s2;

	new_s1 = (unsigned char*)s1;
	new_s2 = (unsigned char*)s2;
	while ((*new_s1 || *new_s2) && n > 0)
	{
		if (*new_s1 != *new_s2)
			return (*new_s1 - *new_s2);
		new_s1++;
		new_s2++;
		n--;
	}
	return (0);
}

#define NMEA_GPRMC_LATITUDE 2
#define NMEA_GPRMC_LONGITUDE 4

// NMEA
// Latitude -> DDmm.mmmm
// Longitude -> DDDmm.mmmm
int		parse_nmea_gps(char *data, t_coord *coord)
{
	int size = ft_strlen(data);
	char separatorCount = 0;
	double lat = 0.0;
	double lon = 0.0;
	if (size > 6)
	{
		if (!ft_strncmp(data, "$GPRMC,", 7))
		{
			int i = 0;
			int degrees = 0;
			int min = 0;
			int sec = 0;
			int neg = 1;
			while (i < size && data[i] != '\0')
			{
				if (data[i] == ',') {
					if (separatorCount == NMEA_GPRMC_LATITUDE) {
						if (i + 9 + 1 > size)
							return (0);
						if (data[i+1] == ',') {
							return (999); // Empty value
						} else {
							if (data[i + 1] == '-')
							{
								neg = -1;
								i++;
							}
							degrees = (data[i+1] - '0') * 10 + (data[i+2] - '0');
							min = (data[i+3] - '0') * 10 + (data[i+4] - '0');
							sec = ((data[i+6] - '0') * 1000 + (data[i+7] - '0') * 100 + (data[i+8] - '0') * 10 + (data[i+9] - '0')) * 60.0 / 10000.0; // Get secondes from xx.mmmm
							lat = neg * ((double)degrees + (double)min / 60.0 + (double)sec / 3600.0); // Degrees, Minutes and Secondes to Digital degrees
							i += 9;
						}
					} else if (separatorCount == NMEA_GPRMC_LONGITUDE) {
						if (i + 10 + 1 > size)
							return (0);
						if (data[i+1] == ',') {
							return (999); // Empty value
						} else {
							if (data[i + 1] == '-')
							{
								neg = -1;
								i++;
							}
							degrees = (data[i+1] - '0') * 100 + (data[i+2] - '0') * 10 + (data[i+3] - '0');
							min = (data[i+4] - '0') * 10 + (data[i+5] - '0');
							sec = ((data[i+7] - '0') * 1000 + (data[i+8] - '0') * 100 + (data[i+9] - '0') * 10 + (data[i+10] - '0')) * 60 / 10000; // Get secondes from xx.mmmm
							lon = neg * ((double)degrees + (double)min / 60.0 + (double)sec / 3600.0); // Degrees, Minutes and Secondes to Digital degrees
							i += 10;
						}
					}
					separatorCount++;
				}
				i++;
			}
			coord->lat = lat;
			coord->lon = lon;
			return (1);
		}
	}
	return(0);
}
