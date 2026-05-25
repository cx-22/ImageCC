#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include "nodes.h"

void start(){
    buildNodeList();
    buildFuncs();
}

void end(){
    killList();
    killFuncs();
}

int main(){
    clock_t t; 
    t = clock();

    buildNodeList();
    buildFuncs();

    char name1[] = "test1.png";
    char name2a[] = "test2a.png";
    char name2b[] = "test2b.png";
    char name3[] = "test3.png";
    char name4[] = "test4.png";

    /*
        // Test 1
        /*
            struct Node* input = createNode(0, INPUT);
            struct Node* output = createNode(1, OUTPUT);

            printf("input: %p\n", input);
            printf("output: %p\n", output);

            struct Image* image = loadPng("r3d.png");
            printImage(image);
            deepCopyImage(input->output_images[0], image);
            destroyImage(image);

            addParam(1, &name2a, 0);
            connectNodes(0, 1, 0, 0);
            disconnectNodes(0, 1, 0, 0);
            addParam(1, &name2b, 0);
            connectNodes(0, 1, 0, 0);
        /*/

        // Test 2
        /*
            struct Node* input = createNode(0, INPUT);
            struct Node* output = createNode(1, OUTPUT);

            printf("input: %p\n", input);
            printf("output: %p\n", output);

            struct Image* image = loadPng("r3d.png");
            printImage(image);
            deepCopyImage(input->output_images[0], image);
            destroyImage(image);
            
            addParam(1, name3, 0);
            struct Node* mid = createNode(2, 0);
            printf("mid: %p\n", mid);
            int val = 50;
            addParam(2, &val, 0);
            connectNodes(0, 2, 0, 0);
            connectNodes(2, 1, 0, 0);
        //*/

        // Test 3
        /*
            struct Node* input = createNode(0, INPUT);
            struct Node* output = createNode(1, OUTPUT);

            printf("input: %p\n", input);
            printf("output: %p\n", output);

            struct Image* image = loadPng("r3d.png");
            printImage(image);
            deepCopyImage(input->output_images[0], image);
            destroyImage(image);
            
            addParam(1, name3, 0);
            struct Node* mid1 = createNode(2, 0);
            struct Node* mid2 = createNode(3, 1);
            //printf("mid: %p\n", mid);
            uint8_t val1 = 100;
            uint8_t val2 = 10;
            addParam(2, &val1, 0);
            addParam(3, &val2, 0);
            connectNodes(0, 2, 0, 0);
            connectNodes(2, 3, 0, 0);
            connectNodes(3, 1, 0, 0);
        //*/

        // Test 4
        /*
            struct Image* image1 = loadPng("500_1.png");
            struct Image* image2 = loadPng("500_2.png");

            struct Node* input1 = createNode(0, INPUT);
            struct Node* input2 = createNode(1, INPUT);

            deepCopyImage(input1->output_images[0], image1);
            deepCopyImage(input2->output_images[0], image2);
            destroyImage(image1);
            destroyImage(image2);

            struct Node* mid = createNode(2, 2);

            float val = 0.5;
            addParam(2, &val, 0);

            struct Node* output = createNode(3, OUTPUT);
            addParam(3, name4, 0);

            connectNodes(0, 2, 0, 0);
            connectNodes(1, 2, 0, 1);
            connectNodes(2, 3, 0, 0);
        /*/
    //*/

    t = clock() - t; 
    
        double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf(" took %f seconds to execute \n", time_taken); 

    //destroyImage(image);
    killList();
    killFuncs();
    return 0;
}