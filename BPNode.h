#ifndef BPNODE_H
#define BPNODE_H

template <typename Key, typename E, int Order>
class BPNode {
public:

	virtual ~BPNode() {}
	virtual bool isLeaf() const = 0;
	virtual bool isFull() const = 0;
	virtual int numrecs() const = 0;
	virtual Key* keys() const = 0;
	virtual BPNode* left() const = 0;
	virtual BPNode* right() const = 0;

	virtual int location(const Key k) const = 0;

};

#endif
