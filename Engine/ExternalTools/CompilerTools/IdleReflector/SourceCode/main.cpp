#include <iostream>
#include <zlib.h>

//#include<tinyxml2.h>


int main(void)
{   
    //tinyxml2::XMLUtil::IsUTF8Continuation('f');
    std::cout << "Hello world" << std::endl;
    std::cout << zlibVersion() << std::endl;
    //std::cout << tinyxml2::XMLUtil::IsUTF8Continuation('a');

    return 0;
}