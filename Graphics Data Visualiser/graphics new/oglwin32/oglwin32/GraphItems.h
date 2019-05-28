#pragma once

struct RGB
{
	float red;
	float green;
	float blue;
};
struct graphItem
{
	std::string		data;
	int				occurency;
	RGB				colour;
};

struct graphItem2E
{
	std::string		data1;
	std::string		data2;
	int				occurency;
	RGB				colour;
};

struct graphItem3E
{
	std::string		data1;
	std::string		data2;
	std::string		data3;
	int				occurency;
	RGB				colour;
};