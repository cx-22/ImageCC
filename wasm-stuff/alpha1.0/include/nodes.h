#pragma once
#include "cvector.h"
#include "functions.h"

#define MAXNODES	20
#define MAX_OUT		10

#define INPUT		-2
#define OUTPUT		-1

#define ACTIVE		0
#define INACTIVE	1
#define EMPTY		2

// A node will own its own output images, but reference its input
struct Node{
	struct Node** input_nodes;
	struct Node** output_nodes;
	struct Image** input_images;
	struct Image** output_images;
	void** params;
	struct function* func;
	//void** args; // order: input images list, output images list, parameters
	uint8_t status;
};

struct NodeEntry{
	int nid;
	int type;
	struct Node* node;
};

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