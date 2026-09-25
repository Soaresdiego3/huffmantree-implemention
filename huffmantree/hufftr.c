
/* huff.c */

#include <hufftr.h>

void zero(int8 *dst, int16 size)   // dst destination
{
    int16 n; // counter
    int8 *p;

    for(p = dst, n = size; n; p++, n--)
    {
        *p = 0;
    }

    return;
}

leaf *mkleaf(int8 c)
{
    leaf *p;
    int64 size;

    size = sizeof(struct s_leaf);

    p = $el alloc(size);
    assert(p);

    zero($1 p, size);

    p->c = c;
    p->freq = 1;
    p->up = $t 0;
    p->kind = Leaf;

    return p;
}

static int64 tree_freq(tree *t)
{
    assert(t);

    if(t->n.kind == Leaf)
        return t->l.freq;

    if(t->n.kind == Node)
        return t->n.freq;

    assert(0);

    return 0;
}

static void set_up(tree *child, tree *parent)
{
    assert(child);

    if(child->n.kind == Leaf)
    {
        child->l.up = parent;
    }
    else if(child->n.kind == Node)
    {
        child->n.up = parent;
    }
    else
    {
        assert(0);
    }
}

node *mknode(tree *left, tree *right)
{
    node *p;
    int16 size;

    assert(left && right);

    size = sizeof(struct s_node);

    p = $n alloc(size);
    assert(p);

    zero($1 p, size);

    /*
    O kind precisa ser definido antes de chamar conn(),
    pq conn() verifica se o parent é realmente um Node
    */
    p->kind = Node;
    p->up = $t 0;

    // connection
    conn($t p, true, left);
    conn($t p, false, right);

    /*
    freq do node =
    freq left child +
    freq right child
    */
    p->freq = tree_freq(left);
    p->freq += tree_freq(right);

    return p;
}

void conn(tree *parent, bool isleft, tree *child)
{
    tree **connector;

    assert(parent && child);
    assert(parent->n.kind == Node);

    if(isleft)
    {
        connector = (tree **)&parent->n.left;
    }
    else
    {
        connector = (tree **)&parent->n.right;
    }

    *connector = child;

    set_up(child, parent);

    return;
}

void show_(tree *t, int8 *ident)
{
    if(!t)
        return;

    switch(t->n.kind)
    {
        case Leaf:

            printf("(leaf *)%s = {\n", $c ident);

            printf(" char: %c\n",
                (char)t->l.c);

            printf(" freq: %lld\n",
                (unsigned long long int)t->l.freq);

            if(t->l.up)
            {
                printf("  up: [%lld]\n",
                    (unsigned long long int)
                    tree_freq(t->l.up));
            }
            else
            {
                printf("  up: <disconnected>\n");
            }

            printf("}\n");
            break;


        case Node:

            printf("(node *)%s = {\n", $c ident);

            printf(" freq: %lld\n",
                (unsigned long long int)t->n.freq);

            if(t->n.up)
            {
                printf("  up: [%lld]\n",
                    (unsigned long long int)
                    tree_freq(t->n.up));
            }
            else
            {
                printf("  up: <disconnected>\n");
            }

            if(t->n.left)
            {
                printf("  left: [%lld]\n",
                    (unsigned long long int)
                    tree_freq(t->n.left));
            }
            else
            {
                printf("  left: <disconnected>\n");
            }

            if(t->n.right)
            {
                printf("  right: [%lld]\n",
                    (unsigned long long int)
                    tree_freq(t->n.right));
            }
            else
            {
                printf("  right: <disconnected>\n");
            }

            printf("}\n");
            break;


        default:

            return;
    }
}

int main(void)
{
    node *n;
    leaf *l1;
    leaf *l2;

    l1 = mkleaf((int8)'a');
    l1->freq = 4;
    l2 = mkleaf((int8)'b');
    n = mknode($t l1, $t l2);

    show(n);
    printf("\n");

    show(l1);
    printf("\n");

    show(l2);
    printf("\n");

    free(n);
    free(l1);
    free(l2);

    return 0;
}

