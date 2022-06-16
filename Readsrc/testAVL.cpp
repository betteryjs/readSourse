#include <iostream>
#include "AVL.h"


int main(int argc, char* argv[])
{
    AVL* root = NULL;
    int vector[] = {12,15,25,63,23};
//	int vector[] = {15};

    const int length = sizeof(vector)/sizeof(int);
    for(int i = 0; i< length;i++)
        root = Insert(root, vector[i]);

    printf("\nInOrder: \n");
    InOrder(root);


    printf("\n");
    return 0;
}