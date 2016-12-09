#ifndef BPLEAF_H
#define BPLEAF_H
#include "BPNode.h"

template <typename Key, typename E, int Order> //block的长度
class BPLeaf: public BPNode<Key, E, Order> {
public:

	BPLeaf(BPLeaf* l = NULL, BPLeaf* r = NULL);
	BPLeaf(Key k, E e, BPLeaf* l = NULL, BPLeaf* r = NULL);

	bool isLeaf() const;
	bool isFull() const;
	int numrecs() const;

	void setLeft(BPNode<Key, E, Order>* l);
	void setRight(BPNode<Key, E, Order>* r);

	BPNode* left() const;
	BPNode* right() const;

	BPNode<Key, E, Order>* add(const Key k, const E& e);
	bool del(int currec);

	int location(const Key k) const;

	Key* keys() const;
	E recs(int curr) const;

	Key firstKey() const;

private:
	Key* key;
	E* record;
	int count;

	BPNode* lp;
	BPNode* rp;

	BPNode* split();

};

template <typename Key, typename E, int Order>
BPLeaf<Key, E, Order>::BPLeaf(BPLeaf<Key, E, Order>* l, BPLeaf<Key, E, Order>* r) {
	key = new Key[Order];
	record = new E[Order];
	count = 0;
	lp = l;
	rp = r;
}

template <typename Key, typename E, int Order>
BPLeaf<Key, E, Order>::BPLeaf(Key k, E e, BPLeaf<Key, E, Order>* l, BPLeaf<Key, E, Order>* r) {
	key = new Key[Order];
	record = new E[Order];
	key[0] = k;
	record[0] = e;
	count = 1;
	lp = l;
	rp = r;
}

template <typename Key, typename E, int Order>
bool BPLeaf<Key, E, Order>::isLeaf() const {
	return true;
}

template <typename Key, typename E, int Order>
bool BPLeaf<Key, E, Order>::isFull() const {
	return count == Order; 
}

template <typename Key, typename E, int Order>
int BPLeaf<Key, E, Order>::numrecs() const {
	return count;
}

template <typename Key, typename E, int Order>
BPNode<Key, E, Order>* BPLeaf<Key, E, Order>::left() const {
	return lp;
}

template <typename Key, typename E, int Order>
BPNode<Key, E, Order>* BPLeaf<Key, E, Order>::right() const {
	return rp;
}

template <typename Key, typename E, int Order>
void BPLeaf<Key, E, Order>::setLeft(BPNode<Key, E, Order>* l) {
	lp = l;
}

template <typename Key, typename E, int Order>
void BPLeaf<Key, E, Order>::setRight(BPNode<Key, E, Order>* r) {
	rp = r;
}

template <typename Key, typename E, int Order>
Key* BPLeaf<Key, E, Order>::keys() const {
	return key;
}

template <typename Key, typename E, int Order>
E BPLeaf<Key, E, Order>::recs(int curr) const {
	return record[curr];
}

template <typename Key, typename E, int Order>
Key BPLeaf<Key, E, Order>::firstKey() const {
	return key[0];
}

template <typename Key, typename E, int Order>
BPNode<Key, E, Order>* BPLeaf<Key, E, Order>::add(const Key k, const E& e) {
	int currec = location(k);
	for (int i = count; i > currec; --i) {
		key[i] = key[i-1];
		record[i] = record[i-1];
	}
	key[currec] = k;
	record[currec] = e;
	++count;

	if (isFull())
		return split();
	else
		return this;

}

template <typename Key, typename E, int Order>
bool BPLeaf<Key, E, Order>::del(int currec) {
	for (int i = currec; i < count + 1; ++i) {
		key[i] = key[i+1];
		record[i] = record[i+1];
	}
	--count;
}
template <typename Key, typename E, int Order>
int BPLeaf<Key, E, Order>::location(const Key k) const{
	int l = 0;
	int r = count - 1;
	int mid = 0;
	while(l <= r) {
		mid = (r - l) / 2 + l;
		if (k < keys()[mid])
			r = mid - 1;
		else if (k > keys()[mid])
			l = mid + 1;
		else
			return mid + 1;
	}
	if (r < mid)
		return l;
	if (l > mid)
		return r + 1; 
}

template <typename Key, typename E, int Order>
BPNode<Key, E, Order>* BPLeaf<Key, E, Order>::split() {
	BPLeaf<Key, E, Order>* newLeaf = new BPLeaf<Key, E, Order>(this, NULL);
	setRight(newLeaf);
	for (int i = count/2; i < count; ++i)
		newLeaf->add(key[i], record[i]);
	count = count/ 2;
	BPInternal<Key, E, Order>* newNode = new BPInternal<Key, E, Order>(newLeaf->firstKey(),this, newLeaf);
	return newNode;
}

#endif 
