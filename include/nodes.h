#pragma once
#include "types-c.h"

#ifdef __cplusplus
extern "C" {
#endif

void buildNodeList();
void killList();


#ifdef __cplusplus
}
#endif

extern cvector_vector_type(struct NodeEntry*) node_list;
extern int num_nodes;

void buildNodeList();

struct Node* createNode(int nid, int type);
void connectNodes(int input_nid, int output_nid, int index_start, int index_end);
void disconnectNodes(int input_nid, int output_nid, int index_start, int index_end);
void disconnectFromKill(struct Node* input_node, struct Node* output_node, int nid, int dead);

void applyFunc(struct Node* node);
void addParam(int nid, void* arg, int param_index);//, int type);
void updateForward(struct Node* node, int nid);
void activateForward(struct Node* node, int nid);
void deactivateForward(struct Node* node, int nid);

void setImageInput(int nid, uint8_t* buffer, int width, int height, int channels);

void killNode(struct Node* node, struct NodeEntry* ne, int index, int save_num);
void clearNodes();
void killList();
