#include "network.h"
#include "random.h"

void Network::resize(const size_t &n)
{
	RandomNumbers dist_normal(0);
	values.clear(); 
	values.resize(n);
	dist_normal.normal(values, 0, 1); 
}

bool Network::add_link(const size_t& a, const size_t& b)
{
	bool link_a_b(false); //true if there is a link between a and b, false otherwise 
	bool link_b_a(false); //true if there is a link between b and a, false otherwise 
	auto rangeA = links.equal_range(a); 
	auto rangeB = links.equal_range(b);
	
	for (auto i = rangeA.first; i != rangeA.second; ++i)
	{
		if (i->second == b) 
		{
			 link_a_b = true;
			 break;
		}
	}
	for (auto i = rangeB.first; i != rangeB.second; ++i)
	{
		if (i->second == a) 
		{ 
			link_b_a = true; 
			break; 
		}
	}
	
	if (a != b and !link_a_b and !link_b_a and a < values.size() and b < values.size() and a >= 0 and b >= 0) 
	{
		links.insert({a,b});
		links.insert({b,a});
		return true; 
	} else {
		return false; 
	}
}

size_t Network::random_connect(const double& mean)
{
	for (auto i = links.begin(); i != links.end(); ++i)
	{
		links.erase(i);
	}
	
	RandomNumbers dist_poisson(0);
	int degree; 
	std::uniform_int_distribution<> dist(0, values.size() - 1);
	int node; 
	size_t nb_links(0);
	
	for (int n(0); n < values.size(); ++n) 
	{
		degree = dist_poisson.poisson(mean);
		
		int i(0); 
		while (i < degree)
		{
			std::random_device rd;
			std::mt19937 seed(rd());
			node = dist(seed); 
			
			if (add_link(n, node))
			{
				++nb_links; 
				++i;
			}
		}
	}
	
	return nb_links;
}

size_t Network::set_values(const std::vector<double>& new_values)
{
	int reset_values(0); 
	
	if (values.size() >= new_values.size())
	{
		for (int i(0); i < new_values.size(); ++i)
		{
			values[i] = new_values[i]; 
			++reset_values; 
		}
	} else {
		for (int i(0); i < values.size(); ++i)
		{
			values[i] = new_values[i]; 
			++reset_values; 
		}
	}
	
	return reset_values; 
}

size_t Network::size() const
{ 
	return values.size();
}

size_t Network::degree(const size_t &_n) const
{
	return links.count(_n); 
}

double Network::value(const size_t &_n) const
{
	return values[_n];
}

std::vector<double> Network::sorted_values() const
{
	std::vector<double> sorted_values = values;
	std::sort(sorted_values.begin(), sorted_values.end(), std::greater<double>()); //sort the elements of values into descending order
	return sorted_values; 
}

std::vector<size_t> Network::neighbors(const size_t& n) const
{
	std::vector<size_t> neighbors;	
	auto range = links.equal_range(n); 
	for (auto i = range.first; i != range.second; ++i)
	{
		neighbors.push_back(i->second);
	}
	return neighbors;
}
