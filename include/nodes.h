#pragma once
#include "types-c.h"

#ifdef __cplusplus
extern "C" {
#endif

void buildNodeList();
void killList();
struct Node* createNode(uint16_t nid, int type);
void killNode(struct Node* node, struct NodeEntry* ne, int index, int save_num);
void connectNodes(uint16_t input_nid, uint16_t output_nid, int index_start, int index_end);
void disconnectNodes(uint16_t input_nid, uint16_t output_nid, int index_start, int index_end);
extern cvector_vector_type(struct NodeEntry*) node_list;
extern int num_nodes;


#ifdef __cplusplus
}
#endif


void disconnectFromKill(struct Node* input_node, struct Node* output_node, uint16_t nid, int dead);

void applyFunc(struct Node* node);
void addParam(uint16_t nid, void* arg, int param_index);//, int type);
void updateForward(struct Node* node, uint16_t nid);
void activateForward(struct Node* node, uint16_t nid);
void deactivateForward(struct Node* node, uint16_t nid);

void setImageInput(uint16_t nid, uint8_t* buffer, int width, int height, int channels);

void clearNodes();
void killList();
