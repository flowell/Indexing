#ifndef BPTREE_H
#define BPTREE_H

#include "BPInternal.h"
#include "BPLeaf.h"
#include "Queue.h"

template <typename Key, typename E, int Order>
class BPTree {
public:

	BPTree();
	void insert(Key k, E e);
	void print();

private:
	BPNode<Key, E, Order>* root;

	E findHelp(BPNode<Key, E, Order>* rt, const Key& k) const;
	BPNode<Key, E, Order>* insertHelp(BPNode<Key, E, Order>* rt, const Key& k, const E& e);
	bool removeHelp(BPNode<Key, E, Order>* rt, const Key& k);
	void printHelp(BPNode<Key, E, Order>* rt, Queue<BPNode<Key, E, Order>*>& nq, Queue<E>& eq) const;

};


template <typename Key, typename E, int Order>
BPTree<Key, E, Order>::BPTree() {
	root = NULL;
}


template <typename Key, typename E, int Order>
E BPTree<Key, E, Order>::findHelp(BPNode<Key, E, Order>* rt, const Key& k) const {
	int currec = rt->location(k);
	if (rt->isLeaf())
		if ((((BPLeaf<Key, E, Order>*)rt)->keys())[currec] == k)
			return ((BPLeaf<Key, E, Order>*)rt)->recs(currec);
		else
			return NULL;
	else
		return findHelp(((BPInternal<Key, E, Order>*)rt)->pointers(currec), k);
}


template <typename Key, typename E, int Order>
void BPTree<Key, E, Order>::insert(Key k, E e) {
	root = insertHelp(root, k, e);
}


template <typename Key, typename E, int Order>
BPNode<Key, E, Order>* BPTree<Key, E, Order>::insertHelp(BPNode<Key, E, Order>* rt, const Key& k, const E& e) {
	if (rt == NULL)
		return new BPLeaf<Key, E, Order>(k, e, NULL, NULL);

	if (rt->isLeaf())
		return ((BPLeaf<Key, E, Order>*)rt)->add(k, e);

	int	currec = rt->location(k);
	BPNode<Key, E, Order>* temp = insertHelp(((BPInternal<Key, E, Order>*)rt)->pointers(currec), k, e);

	if (temp != ((BPInternal<Key, E, Order>*)rt)->pointers(currec))
		return ((BPInternal<Key, E, Order>*)rt)->add((BPInternal<Key, E, Order>*)temp);
	else
		return rt;
}

template <typename Key, typename E, int Order>
bool BPTree<Key, E, Order>::removeHelp(BPNode<Key, E, Order>* rt, const Key& k) {
	int currec = rt->location(k);

	if (rt->isLeaf())
		if(((BPLeaf<Key, E, Order>*)rt)->keys()[currec] == k)
			return ((BPLeaf<Key, E, Order>*)rt)->del(currec);
		else
			return false;
	else
		if (removeHelp(((BPInternal<Key, E, Order>*)rt)->pointers(currec), k))
			return ((BPInternal<Key, E, Order>*)rt)->underflow(currec);
		else
			return false;
}


template <typename Key, typename E, int Order>
void BPTree<Key, E, Order>::print() {
	Queue<BPNode<Key, E, Order>*> nq;
	Queue<E> eq;
	printHelp(root, nq, eq);
	int num = eq.length();
	for (int i = 0; i < num; ++i)
		std::cout << eq.dequeue() << ' ';
	std::cout << '\n';
}

template <typename Key, typename E, int Order>
void BPTree<Key, E, Order>::printHelp(BPNode<Key, E, Order>* rt, Queue<BPNode<Key, E, Order>*>& nq, Queue<E>& eq) const {
	if (rt->isLeaf()) { 
		for (int i = 0; i < ((BPLeaf<Key, E, Order>*)rt)->numrecs(); ++i)
			eq.enqueue(((BPLeaf<Key, E, Order>*)rt)->recs(i));
	}
	else 
	{
		int i;
		for (i = 0; i < rt->numrecs(); ++i) {
			eq.enqueue(((BPInternal<Key, E, Order>*)rt)->keys()[i]);
			nq.enqueue(((BPInternal<Key, E, Order>*)rt)->pointers(i));
		}
		nq.enqueue(((BPInternal<Key, E, Order>*)rt)->pointers(i));
	}
	if (nq.length() <= 0)
		return;
	else
		printHelp(nq.dequeue(), nq, eq);

}
#endif
