#include <iostream>
#include <workflow/WFTaskFactory.h>
#include <errno.h>
#include <iostream>


using namespace std;

// 直接定义thread_task三要素
// 一个典型的后端程序由三个部分组成，并且完全独立开发。即：程序=协议+算法+任务流。

// 定义INPUT
struct AddInput
{
    int x;
    int y;
};

// 定义OUTPUT
struct AddOutput
{
    int res;
};

// 加法流程
void add_routine(const AddInput *input, AddOutput *output)
{
    output->res = input->x + input->y;
}

using AddTask = WFThreadTask<AddInput, AddOutput>;

void callback(AddTask *task)
{
    auto *input = task->get_input();
    auto *output = task->get_output();

    assert(task->get_state() == WFT_STATE_SUCCESS);
//cout << task->get_state()<<endl;

    fprintf(stderr, "%d + %d = %d\n", input->x, input->y, output->res);
}

int main()
{
    using AddFactory = WFThreadTaskFactory<AddInput, AddOutput>;
    AddTask *task = AddFactory::create_thread_task("add_task",
                                                   add_routine,
                                                   callback);
    AddInput *input = task->get_input();

    input->x = 1;
    input->y = 2;

    task->start();
//    cout << task->get_output()->res<<endl;

    return 0;
}