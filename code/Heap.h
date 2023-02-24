/* By Vamei */

/* 
 * leftist heap
 * bassed on binary tree 
 */


typedef struct node *position;
typedef Node_HLv ElementTP;

struct node {
    ElementTP element;
    int npl;
    position lchild;
    position rchild;
};

typedef struct node *LHEAP;

LHEAP insert(ElementTP, LHEAP);
ElementTP find_min(LHEAP);
LHEAP delete_min(LHEAP);
LHEAP merge(LHEAP, LHEAP);
static LHEAP merge1(LHEAP, LHEAP);
static void swap_children(LHEAP);

LHEAP L[Max_Nodes_Num+5];

LHEAP H[Max_Nodes_Num+5];



/*
 * insert:
 * merge a single-node leftist heap with a leftist heap
 * */
LHEAP insert(ElementTP value, LHEAP h)
{
    LHEAP single;
    single = (position) malloc(sizeof(struct node));

    // initialze
    single->element  = value;
    single->lchild   = NULL;
    single->rchild   = NULL;

    return  merge(single, h);
}

/*
 * find_min:
 * return root value in the tree
 * */
ElementTP find_min(LHEAP h)
{
    ElementTP res;
    if(h != NULL) return h->element;
    else {
        res.fx=-1;
        return res;
    }
}

/*
 * delete_min:
 * remove root, then merge two subheaps
 * */
LHEAP delete_min(LHEAP h)
{
    LHEAP l,r;
    l = h->lchild;
    r = h->rchild;
    free(h);
    return merge(l, r);
}

/*
 * merge two leftist heaps
 * */
LHEAP merge(LHEAP h1, LHEAP h2) 
{

    // if one heap is null, return the other
    if(h1 == NULL) return h2;
    if(h2 == NULL) return h1;

    // if both are not null
    if (h1->element.dis < h2->element.dis) { 
        return merge1(h1, h2);
    }
    else {
        return merge1(h2, h1);
    }
}

// h1->element < h2->element
static LHEAP merge1(LHEAP h1, LHEAP h2)
{
    if (h1->lchild == NULL) { 
        /* h1 is a single node, npl is 0 */
        h1->lchild = h2; 
    /* rchild is NULL, npl of h1 is still 0 */
    }
    else {
        // left is not NULL
    // merge h2 to right
    // swap if necessary
        h1->rchild = merge(h1->rchild, h2);
    if(h1->lchild->npl < h1->rchild->npl) {
        swap_children(h1);
    }
        h1->npl = h1->rchild->npl + 1; // update npl
    }
    return h1;
}

// swap: keep leftist property
static void swap_children(LHEAP h) 
{
    LHEAP tmp;
    tmp       = h->lchild;
    h->lchild = h->rchild;
    h->rchild = tmp;
}



/*
struct TreeNode{
    Node_HLv key;
    int npl;
    TreeNode* left;
    TreeNode* right;
    TreeNode(Node_HLv k){
        key = k;
        npl = 0;
        left = right = NULL;
    }
};
void Swap(TreeNode* node1, TreeNode* node2){
    TreeNode* tmp = node1;
    node1 = node2;
    node2 = tmp;
}
 

TreeNode* Merge(TreeNode* heap1, TreeNode* heap2){

    if (!heap1)
        return heap2;
    if (!heap2)
        return heap1;

    if (heap1->key.dis > heap2->key.dis){
        //Swap(heap1, heap2);
        
        heap2->right = Merge(heap2->right, heap1);
        if (heap2->left == NULL || (heap2->left && heap2->right && heap2->left->npl < heap2->right->npl)){ //如果堆的根节点的左子结点的npl小于右子节点的npl，则交换左右子节点
            Swap(heap2->left, heap2->right);
        }
    
        if (heap2->right == NULL)
            heap2->npl = 1;
        else
            heap2->npl = heap2->right->npl + 1; //新堆的npl设为右子节点的npl +１
        return heap2;
        
    }

    heap1->right = Merge(heap1->right, heap2);
    if (heap1->left == NULL || (heap1->left && heap1->right && heap1->left->npl < heap1->right->npl)){ //如果堆的根节点的左子结点的npl小于右子节点的npl，则交换左右子节点
        Swap(heap1->left, heap1->right);
    }
 
    if (heap1->right == NULL)
        heap1->npl = 1;
    else
        heap1->npl = heap1->right->npl + 1;
    return heap1;
}
 
void Enqueue(TreeNode*& heap, Node_HLv k){
    if (!heap){
        heap = new TreeNode(k);
        return;
    }
    TreeNode* new_heap = new TreeNode(k);
    heap = Merge(heap, new_heap);
}

TreeNode *topl,*topr;
Node_HLv Dequeue(TreeNode*& heap){
    Node_HLv result;
    if(heap==NULL)
    {
        result.fx=-1;
        return result;
    }
    result = heap->key;

    heap = Merge(heap->left, heap->right);

    return result;
}
*/