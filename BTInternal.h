#ifndef BPINTERNAL_H
#define BPINTERNAL_H

#include "BPNode.h"

template <typename Key, typename E, int Order>
class BPInternal: public BPNode<Key, E, Order> {
public:
	
	BPInternal(BPInternal* l = NULL, BPInternal* r = NULL);
	BPInternal(Key k, BPNode* pl, BPNode* pr, BPInternal* l = NULL, BPInternal* r = NULL);
	bool isLeaf() const;
	bool isFull() const;

	int numrecs() const;
	Key* keys() const;
	BPNode<Key, E, Order>* pointers(int curr) const;

	BPNode* add(BPInternal<Key, E, Order>* p);

	int location(const Key k) const;

	BPNode* left() const;
	BPNode* right()const;

	void setLeft(BPNode<Key, E, Order>* l);
	void setRight(BPNode<Key, E, Order>* r);

	Key firstKey() const;

private:
	Key* key;
	(BPNode<Key, E, Order>*)* pointer;
	int count;


	BPNode* lp;
	BPNode* rp;

	BPNode* split();

};

template <typename Key, typename E, int Order>
BPInternal<Key, E, Order>::BPInternal(BPInternal<Key, E, Order>* l, BPInternal<Key, E, Order>* r) {
	key = new Key[Order];
	pointer = new BPNode<Key, E, Order>*[Order + 1];
	count = 0;
	lp = l;
	rp = r;
}

template <typename Key, typename E, int Order>
BPInternal<Key, E, Order>::BPInternal(Key k, BPNode<Key, E, Order>* pl, BPNode<Key, E, Order>* pr, BPInternal<Key, E, Order>* l, BPInternal<Key, E, Order>* r) {
	key = new Key[Order];
	pointer = new BPNode<Key, E, Order>*[Order + 1];
	key[0] = k;
	pointer[0] = pl;
	pointer[1] = pr;
	count = 1;
	lp = l;
	rp = r;
}

template <typename Key, typename E, int Order>
bool BPInternal<Key, E, Order>::isLeaf() const {
	return false;
}

template <typename Key, typename E, int Order>
bool BPInternal<Key, E, Order>::isFull() const {
	return count == Order;
}

template <typename Key, typename E, int Order>
int BPInternal<Key, E, Order>::numrecs() const {
	return count;
}

template <typename Key, typename E, int Order>
BPNode<Key, E, Order>* BPInternal<Key, E, Order>::left() const {
	return lp;
}

template <typename Key, typename E, int Order>
BPNode<Key, E, Order>* BPInternal<Key, E, Order>::right() const {
	return rp;
}

template <typename Key, typename E, int Order>
void BPInternal<Key, E, Order>::setLeft(BPNode<Key, E, Order>* l) {
	lp = l;
}

template <typename Key, typename E, int Order>
void BPInternal<Key, E, Order>::setRight(BPNode<Key, E, Order>* r) {
	rp = r;
}
//为了方便将新建的节点加入到旧节点中
template <typename Key, typename E, int Order>
Key BPInternal<Key, E, Order>::firstKey() const {
	return key[0];
}

template <typename Key, typename E, int Order>
Key* BPInternal<Key, E, Order>::keys() const {
	return key;
}

template <typename Key, typename E, int Order>
BPNode<Key, E, Order>* BPInternal<Key, E, Order>::pointers(int curr) const {
	return pointer[curr];
}
//返回k在keys中应该在的位置
template <typename Key, typename E, int Order>
int BPInternal<Key, E, Order>::location(const Key k) const{
	int l = 0;
	int r = count - 1;
	int mid;
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
//将split()中返回的新节点加入到this当前节点中
template <typename Key, typename E, int Order>
BPNode<Key, E, Order>* BPInternal<Key, E, Order>::add(BPInternal<Key, E, Order>* p) {
	int currec = location(p->firstKey());
	for (int i = count; i > currec; --i) {
		key[i] = key[i - 1];
		pointer[i + 1] = pointer[i]; 
	}
	key[currec] = ((BPInternal<Key, E, Order>*)p)->firstKey();
	pointer[currec + 1] = ((BPInternal<Key, E, Order>*)p)->pointers(1);
	++count;
	delete p;

	if (isFull())
		return split();
	else
		return this;
}
//对满了的internal节点分裂，返回新建的母节点，该母节点的pointer[0],pointer[1]，分别指向旧的节点和新建的节点
template <typename Key, typename E, int Order>
BPNode<Key, E, Order>* BPInternal<Key, E, Order>::split() {
	BPInternal<Key, E, Order>* rightNode = new BPInternal<Key, E, Order>(this, NULL);
	setRight(rightNode);
	//可以把这个循环封装成add()函数
	int i,j;
	for (i = count/2 + 1, j = 0; i < count; ++i, ++j) {
		rightNode->key[j] = key[i];
		rightNode->pointer[j] = pointer[i];
		++rightNode->count;
	}
	rightNode->pointer[j] = pointer[i];

	BPInternal<Key, E, Order>* upNode = new BPInternal<Key, E, Order>(key[count/2], this, rightNode, NULL, NULL);
	count = count/2;
	return upNode;


}
#endif
