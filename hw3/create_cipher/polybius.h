#ifndef PB_NODE_H
#define PB_NODE_H

// Define the pb_node structure
typedef struct pb_node {
    char letter;
    char row;
    char col;
} pb_node;

#endif // PB_NODE_H
struct pb_node* populate_pb_table(int);
const struct pb_node* isCharInStructArray(const struct pb_node*, char);
const struct pb_node* isRowColInStructArray(const struct pb_node*, char, char);
char* pbEncode(const char *, struct pb_node*);
char* pbDecode(const char *, struct pb_node*);
