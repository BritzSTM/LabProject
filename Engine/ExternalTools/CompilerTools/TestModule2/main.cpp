#include <iostream>
#include<tbb/task_scheduler_init.h>

int main(void)
{
    tbb::task_scheduler_init init;
    init.initialize();

    std::cout << "this is module2"<< std::endl;

    return 0;
}