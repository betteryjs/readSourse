//
// Created by yjs on 2022/5/3.
//
#include "list.h"
#include <bits/stdc++.h>
#include <stddef.h>




using namespace std;

struct  Task{
    int val;

    struct list_head entry_list;
};


//bool list_add_test(){
//    struct Task task;
//    INIT_LIST_HEAD(&task.entry_list);
//    struct list_head *pos;
//    for (int i = 0; i < 4; ++i) {
//        struct Task *t = (struct Task *)malloc(sizeof(struct Task));
//        t->val=i;
//        list_add(&t->entry_list,&task.entry_list);
//    }
//    list_for_each(pos, &task.entry_list) {
//        printf("val = %d\n", ((struct Task*)pos)->val);
//    }
//    return true;
//}

bool list_tail_test(){
    struct Task task;
    INIT_LIST_HEAD(&task.entry_list);
    struct list_head *pos;
    for (int i = 0; i < 4; ++i) {
        struct Task *t = (struct Task *)malloc(sizeof(struct Task));
        t->val=i;
        list_add_tail(&t->entry_list,&task.entry_list);
    }
    list_for_each(pos, &task.entry_list) {
        printf("val = %d\n", ((struct Task*)pos)->val);
    }
    return true;
}


//bool list_tail_test2(){
//    struct Task task;
//    struct Task * task_pos;
//    INIT_LIST_HEAD(&task.entry_list);
//    struct list_head *pos;
//    for (int i = 0; i < 4; ++i) {
//        struct Task *t = (struct Task *)malloc(sizeof(struct Task));
//        t->val=i;
//        list_add_tail(&t->entry_list,&task.entry_list);
//    }
//    list_for_each_entry(task_pos, &task.entry_list,entry_list) {
//        printf("val = %d\n", task_pos->val);
//    }
//    return true;
//}







int main(){

//    struct Task task;
//    INIT_LIST_HEAD(&task.entry_list);
//    task.val=2;
//    cout << task.entry_list.next <<endl;
//    cout << task.entry_list.prev <<endl;
//    printf("list_add_test : \n");
//    list_add_test();
//
//    printf("list_tail_test : \n");
//    list_tail_test2();
    struct Task task;
    struct Task* taskPos;
    INIT_LIST_HEAD(&task.entry_list);
    struct list_head *pos;
    for (int i = 0; i < 4; ++i) {
        struct Task *t = (struct Task *)malloc(sizeof(struct Task));
        t->val=i;
        list_add_tail(&t->entry_list,&task.entry_list);
    }
    list_for_each(pos, &task.entry_list) {
        taskPos= list_entry(pos,struct Task,entry_list);
        cout << taskPos->val << " ";
//        cout << ((struct Task*)pos)->val<<" "; 偏移地址不对


    }
    cout <<endl;
    struct Task *task_pos;
    list_for_each_entry(task_pos, &task.entry_list, entry_list)
    {
        cout << task_pos->val <<" ";

    }
//    cout <<endl;
//    list_for_each_entry(task_pos, &task.entry_list, entry_list)
//    {
//        if (task_pos->val == 2)
//        {
//            list_del(&task_pos->entry_list);
//            break;
//        }
//
//    }
//    list_for_each_entry(task_pos, &task.entry_list, entry_list)
//    {
//        cout << task_pos->val <<" ";
//
//    }
    cout <<endl;






    return 0;
}
