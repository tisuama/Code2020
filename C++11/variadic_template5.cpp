#include <iostream> 

//还不是很理解里面的机制
template<typename type>
type print(type param)
{
    std::cout<<param<<' ';
    return param;
}
 
template<typename... Args> 
inline void pass(Args&&...) 
{}
 
template<typename... Args> 
inline void expand(Args&&... args) 
{
    //std::cout<<sizeof...(args)<<std::endl;
    pass( print(args)... );
}
 
int main()
{
    expand(42, "answer", true);
}