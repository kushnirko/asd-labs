typedef struct Node
{
    int data;
    struct Node *next;
} node;

/********** Функції для роботи з чергою **********/

typedef struct Queue
{
    node *head;
    node *tail;
} queue;

queue* InitQueue()
{
    queue* q = (queue *) malloc(sizeof(queue));
    q->tail = q->head = NULL;
    return q;
}

int IsQueueEmpty(queue *q)
{
    return (q->tail == NULL);
}

void Enqueue(queue *q, int data)
{
    node *newNode;
    newNode = (node *) malloc(sizeof(node));
    newNode->data = data;
    newNode->next = NULL;
    if (IsQueueEmpty(q))
        q->head = q->tail = newNode;
    else
    {
        q->head->next = newNode;
        q->head = newNode;
    }
}

int Dequeue(queue *q)
{
    if (!IsQueueEmpty(q))
    {
        node *temp = q->tail;
        q->tail = q->tail->next;
        free(temp);
        return 0;
    }
    q->head = NULL;
    return -1;
}

void FreeQueue(queue *q)
{
    int status_of_freeing = 0;
    while (status_of_freeing != -1)
        status_of_freeing = Dequeue(q);
}

/********** Функції для роботи зі стеком **********/

typedef struct Stack
{
    node *head;
} stack;

stack *InitStack()
{
    stack *s = (stack *) malloc(sizeof(stack));
    s->head = NULL;
    return s;
}

int IsStackEmpty(stack *s)
{
    return (s->head == NULL);
}

void Push(stack *s, int data)
{
    node *newNode;
    newNode = (node *) malloc(sizeof(node));
    newNode->data = data;
    newNode->next = s->head;
    s->head = newNode;
}

int Pop(stack *s)
{
    if (!IsStackEmpty(s))
    {
        node *temp = s->head;
        s->head = s->head->next;
        free(temp);
        return 0;
    }
    return -1;
}

void FreeStack(stack *s)
{
    int status_of_freeing = 0;
    while (status_of_freeing != -1)
        status_of_freeing = Pop(s);
}
