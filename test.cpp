#include <iostream>
#include <string>



int main(){
	int end_x,end_y,end_z;
	std::string tmp;
	std::string source="12,-34,180";
	std::string x_s,y_s,z_s;
			end_x = source.find(",");
			x_s = source.substr(0,end_x);
			tmp = source.substr(end_x+1,source.size());
			end_y = tmp.find(",");
			y_s = source.substr(end_x+1,end_y);
			z_s = tmp.substr(end_y+1,tmp.size());
		std::cout<<x_s<<" "<<y_s<<" "<<z_s<<std::endl;

}