
typedef struct Node Node;

typedef struct Node {
	int type;
	void *data;
	Node *next;
} Node;

Node * nodeinit(void *data, int type);
Node * nodeappend(Node *head, void *d, int t);
void nodepush(Node **h, void *data, int type);
int  nodecount(Node *n);

typedef void (*objprinter) (Node *);
void nodeprintall(Node *h, objprinter p);

typedef void (*objdestroy) (void *);
void nodefreelist(Node **n, objdestroy destructor);