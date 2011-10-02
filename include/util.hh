#include <ostream>
template<class T> inline std::ostream& operator << (std::ostream &os, const std::vector<T>& vec)
{
    os << "[ ";
	for(unsigned int i=0;i<vec.size();i++){
    	  os << vec.at(i);
          if(i!=vec.size()-1){
        	  os << " , " << std::endl;
          }
	}
	os << " ]";
    return os;
}
