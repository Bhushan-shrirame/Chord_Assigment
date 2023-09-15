#include <iostream>
#define M 4
#define N 16
class node
{
    node *predecessor = nullptr;
    node *fingerTable[M];

    bool xInRange(int x, int start, int end)
    {
        if (start > end)
        {

            if (x < start && x <= end)
                return true;
            if (x > start && x >= end)
                return true;
        }
        else
        {
            if (x > start && x <= end)
                return true;
        }
        return false;
    }

    void updateFingers()
    {
        int pow2 = 1;
        for (int i = 0; i < M; i++)
        {
            int next = identifier + pow2;
            next = next % N;
            fingerTable[i] = findSuccesor(next);
            pow2 <<= 1;
        }
    }

    node *getClosestPrecidingNode(int id)
    {
        node *maxNode = fingerTable[0];
        for (int i = 0; i < M; i++)
        {
            if (fingerTable[i] && id > fingerTable[i]->identifier && fingerTable[i]->identifier > maxNode->identifier)
            {
                // 1 2 3 4 0 10 6 5
                maxNode = fingerTable[i];
            }
        }
        return maxNode;
    }

    node *findSuccesor(int id)
    {
        if (xInRange(id, identifier, successor->identifier))
        {
            return successor;
        }
        else
        {
            node *t = getClosestPrecidingNode(id);
            if (t->identifier == this->identifier)
                return t;
            return t->findSuccesor(id);
        }
    }

    void notify(node *n)
    {
        if (!predecessor || (xInRange(n->identifier, predecessor->identifier, identifier)))
        {
            predecessor = n;
            if (n == this)
                predecessor = nullptr;
        }
    }

    void stablize()
    {
        node *x = successor->predecessor;
        if (!x)
        {
            successor->notify(this);
        }
        else if (xInRange(x->identifier, this->identifier, successor->identifier))
        {
            successor = x;
            successor->notify(this);
        }
        else
        {
            successor->notify(this);
        }
    }

public:
    int identifier;
    node *successor = nullptr;
    node(){};
    node(int x) : identifier(x){};
    void createNetwork()
    {
        predecessor = nullptr;
        successor = this;
        for (int i = 0; i < M; i++)
        {
            fingerTable[i] = this;
        }
    }

    void showFingerTable()
    {
        std::cout << "NodeID :" << identifier << std::endl;
        std::cout << "Successor :" << (successor ? successor->identifier : -1) << std::endl;
        std::cout << "Predecessor :" << (predecessor ? predecessor->identifier : -1) << std::endl;
        int i = 0, pow2 = 1;
        for (auto &fingers : fingerTable)
        {
            int next = (identifier + pow2) % N;
            std::cout << i << " " << next << "  " << fingers->identifier << std::endl;
            pow2 <<= 1;
            i++;
        }
        std::cout << "-----------" << std::endl;
    }

    void join(node *n)
    {
        predecessor = nullptr;
        successor = n->findSuccesor(identifier);
        if (successor->successor == successor)
            successor->successor = this;
        updateFingers();
        stablize();
    }

    void stablizeNode()
    {
        this->stablize();
        this->updateFingers();
    }

    void nodeLeave()
    {
        successor->predecessor = predecessor;
        predecessor->successor = successor;
        free(this);
    }

    node *findKey(int id)
    {
        return this->findSuccesor(id);
    }
};
