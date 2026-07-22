#include "nodes.h"
#include "functions.h"
#include "image.h"
#include <stdlib.h>
#include <stdio.h>

struct NodeEntry** node_list = NULL;
int num_nodes = 0;

struct Node empty_node = {.status = EMPTY};

void buildNodeList(){
    //node_list = (struct NodeEntry**)malloc(sizeof(struct NodeEntry) * MAXNODES);
    cvector_vector_type(struct NodeEntry*) node_list = NULL;

}

void killList(){
    clearNodes();
    cvector_free(node_list);
}

void clearNodes(){
    struct NodeEntry* ne;
    struct Node* node;
    for (int i = 0; i < num_nodes; i++){
        ne = node_list[i];
        node = ne->node;
        killNode(node, ne, -1, 1);
    }
}


void setImageInput(
    int nid,
    uint8_t* buffer,
    int width,
    int height,
    int channels
) {
    struct NodeEntry* ne;
    struct Node* node;
    for (int i = 0; i < num_nodes; i++){
        ne = node_list[i];
        if (ne->nid == nid){
            node = ne->node;
            break;
        }
    }

    if(node && ne->type == INPUT){
        struct Image* image = node->output_images[0];
        image->buffer = malloc(width * height * channels);
        memcpy(image->buffer, buffer, width * height * channels);
        image->status = DECOM;
        image->rows = height;
        image->cols = width;
        image->channels = channels;
    }
}




struct Node* createNode(int nid, int type){
    struct Node* node = malloc(sizeof(struct Node));
    struct NodeEntry* ne = malloc(sizeof(struct NodeEntry));
    ne->nid = nid;
    ne->type = type+2;
    ne->node = node;

    int cap = cvector_capacity(node_list);
    if (cap == (cvector_size(node_list) + 2)){
        cvector_reserve(node_list, (cap + 5));
    }
    cvector_push_back(node_list, ne);
    
    num_nodes++;

    node->func = &func_list[ne->type];
    printf("made nid:%d\n", nid);

    if(node->func->n_inputs > 0){
        node->input_nodes = calloc(node->func->n_inputs, sizeof(struct Node*));
        node->input_images = calloc(node->func->n_inputs, sizeof(struct Image*));
    } else {
        node->input_nodes= NULL;
        node->input_images = NULL;
    }

    if(node->func->n_outputs > 0){
        node->output_nodes = calloc(node->func->n_outputs, sizeof(struct Node*));
        node->output_images = calloc(node->func->n_outputs, sizeof(struct Image*));
    } else {
        node->output_nodes= NULL;
        node->output_images = NULL;
    }

    if(node->func->n_params > 0){
        node->params = calloc(node->func->n_params, sizeof(void*));
    } else {
        node->params = NULL;
    }

    /*
        node->input_nodes = malloc(sizeof(struct Node*) * node->func->n_inputs);
        node->output_nodes = malloc(sizeof(struct Node*) * node->func->n_outputs);
        node->input_images = malloc(sizeof(struct Image*) * node->func->n_inputs);
        node->output_images = malloc(sizeof(struct Image) * node->func->n_outputs);
        node->params = malloc(sizeof(void*) * node->func->n_params);
    */
    //node->args = malloc(sizeof(void*) * (node->func->n_params + 2));

    uint8_t i, index;
    for(i = 0; i < node->func->n_inputs; i++){
        node->input_nodes[i] = NULL;
        node->input_images[i] = NULL;
    }
    for(i = 0; i < node->func->n_outputs; i++){
        node->output_nodes[i] = NULL;
        node->output_images[i] = NULL;
    }

    for (i = 0; i < node->func->n_outputs; i++) {
        node->output_images[i] = malloc(sizeof(struct Image));
        node->output_images[i]->buffer = NULL;
        printf("node: %p made image: %p\n", node, node->output_images[i]);
    }
    /*
        for(i = 0; i < node->func->n_args; i++){ node->args[i] = NULL; }
        node->args[1] = node->output_images;
        node->args[2] = node->params;
    */

    node->status = (type == INPUT) ? ACTIVE : INACTIVE;

    return node;
}

// Should have at least defaults for params

void applyFunc(struct Node* node){
    //printf("hi apply\n");
    /*
        //printf("testing:%p\n", node);
        uint8_t i, index;
        index = i = 0;
        for(i = 0; i < node->func->n_inputs; i++){
            //node->args[index] = (void*)node->inputs[i]->args[0];
            printf("checking this input:%p\n", node->inputs[i]);
            printf("and arg:%p\n", (void*)node->inputs[i]->args[0]);
            printf("1- index:%d  i:%d\n", index, i);
            index++;
        }
        
        i = 0;
        for(i = 0; i < node->func->n_outputs; i++){
            node->args[index] = (void*)node->outputs[i]->args;
            printf("2- index:%d  i:%d\n", index, i);
            index++;
        }

        for(i = 0; i < node->func->n_args; i++){
            printf("args%d :%p\n", i, (struct Image*)node->args[i]);
        }
    */

    node->func->op(node->input_images,
                    node->output_images,
                    node->params);
}


void addParam(int nid, void* arg, int param_index){
    struct Node* node;
    struct NodeEntry* ne;
    int i;
    if (nid != -1){
        for (i = 0; i < num_nodes; i++){
            ne = node_list[i];
            if(ne->nid == nid){
                node = ne->node;
                break;
            }
        }
    }

    //node->args[2][param_index] = arg;
    node->params[param_index] = arg;
    printf("pi: %d  arg: %f\n", param_index, *(float*)arg);
}



void connectNodes(int input_nid, int output_nid, int index_start, int index_end){
    struct Node* input_node = NULL;
    struct Node* output_node = NULL;
    struct NodeEntry* ne;
    int i;

    for (i = 0; i < num_nodes; i++) {
        ne = node_list[i];
        if (ne->nid == input_nid) input_node = ne->node;
        if (ne->nid == output_nid) output_node = ne->node;
        if (input_node && output_node) break;
    }

    printf("number of inputs %d\n", output_node->func->n_inputs);

    input_node->output_nodes[index_start] = output_node;
    output_node->input_nodes[index_end] = input_node;
    output_node->input_images[index_end] = input_node->output_images[index_start];

    printf("connect %p at node %d to %p with image %p\n", output_node, index_end, input_node, input_node->output_images[index_start]);

    if(input_node->status == ACTIVE){
        for(i = 0; i < output_node->func->n_inputs; i++){
            if (output_node->input_images[i] == NULL){
                printf("missing %p is missing input %d\n", input_node, i);
                return;
            }
        }
        activateForward(output_node, -1);
    }
}

void activateForward(struct Node* node, int nid){
    int i;
    struct NodeEntry* ne;
    if (nid != -1){
        for (i = 0; i < num_nodes; i++){
            ne = node_list[i];
            if(ne->nid == nid){
                node = ne->node;
                break;
            }
        }
    }

    node->status = ACTIVE;
    applyFunc(node);

    uint8_t j;
    struct Node* next;
    for(i = 0; i < node->func->n_outputs; i++){
        if(!node->output_nodes[i]) continue;
        

        next = node->output_nodes[i];

        for(j = 0; j < next->func->n_inputs; j++){
            if (next->input_images[i] == NULL){
                printf("missing input %d\n", i);
                break;
            }
        }

        if (j == (next->func->n_inputs - 1)){
            activateForward(next, -1);
        }
    }
}


void disconnectNodes(int input_nid, int output_nid, int index_start, int index_end){
    struct Node* input_node = NULL;
    struct Node* output_node = NULL;
    struct NodeEntry* ne;
    for (int i = 0; i < num_nodes; i++){
        ne = node_list[i];
        if(ne->nid == input_nid){
            input_node = ne->node;
        } else if (ne->nid == output_nid){
            output_node = ne->node;
        }

        if(input_node && output_node){
            break;
        }
    }
    
    input_node->output_nodes[index_start] = NULL;
    output_node->input_nodes[index_end] = NULL;

    output_node->input_images[index_end] = NULL;
    //output_node->args[0][index_end] = NULL;

    if(input_node->status == ACTIVE && output_node->status == ACTIVE){
        deactivateForward(output_node, -1);
    }
}


void deactivateForward(struct Node* node, int nid){
    int i;
    struct NodeEntry* ne;
    if (nid != -1){
        for (i = 0; i < num_nodes; i++){
            ne = node_list[i];
            if(ne->nid == nid){
                node = ne->node;
                break;
            }
        }
    }

    node->status = INACTIVE;
    for(i = 0; i < node->func->n_outputs; i++){
        if(!node->output_nodes[i]) continue;
        

        deactivateForward(node->output_nodes[i], -1);
    }
}

// 0 means the input is dead, 1 means the output is dead
void disconnectFromKill(struct Node* input_node, struct Node* output_node, int index, int dead){
    if (dead == 0){
        output_node->input_nodes[index] = NULL;
        output_node->input_images[index] = NULL;   
    } else {
        input_node->output_nodes[index] = NULL;
    }

    deactivateForward(output_node, -1);
}


void killNode(struct Node* node, struct NodeEntry* ne, int nid, int save_num){
    //printf("killing: %p\n", node);
    struct Node* node_prime;
    int i, j;

    if (nid != -1){
        for (i = 0; i < num_nodes; i++){
            ne = node_list[i];
            if(ne->nid == nid){
                node = ne->node;
                break;
            }
        }
    }

    
    // for all nodes that the dying node outputs to
    for (i = 0; i < node->func->n_outputs; i++){
        node_prime = node->output_nodes[i];
        if(node_prime){
            for (j = 0; j < node_prime->func->n_inputs; j++){
                disconnectFromKill(node, node_prime, j, 0);    
            }
        }
        if(node->output_images){
            if(node->output_images[i]){
                destroyImage(node->output_images[i]);
            }
        }
    }

    // for all nodes that output to the dying node
    for (i = 0; i < node->func->n_inputs; i++){
        node_prime = node->input_nodes[i];
        if (node_prime){
            disconnectFromKill(node_prime, node, i, 1);
        }
    }

    
    if(node->func->n_inputs > 0){
        if(node->input_nodes){
            free(node->input_nodes);
            node->input_nodes = NULL;
        }
        if(node->input_images){
            free(node->input_images);
            node->input_images = NULL;
        }
    }

    if(node->func->n_outputs > 0){
        if(node->output_nodes){
            free(node->output_nodes);
            node->output_nodes = NULL;
        }
        if(node->output_images){
            free(node->output_images);
            node->output_images = NULL;
        }
    }
    
    
    if(node->func->n_params > 0){
        if(node->params){
            free(node->params);
            node->params = NULL;
        }
    }
    
    free(node);
    free(ne);
    node = NULL;
    ne = NULL;
    //free(node->args);

    if (save_num == 0){
        num_nodes--;
    }
}
