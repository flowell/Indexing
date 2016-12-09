#ifndef QUEUE_H
#define QUEUE_H

//可变长环形队列
template <typename E>
class Queue {
public:
	Queue();
	~Queue();

	void enqueue(E e);
	E dequeue();

	int length() const;

private:
	E* queue;
	int head;
	int tail;//tail指向最后一位的后一位空位
	int size; //增加了size变量，使得队列实现更简单
	int maxSize;

	void resize(int newSize);
};

template <typename E>
Queue<E>::Queue() {
	size = 0;
	maxSize = 0;
	queue = NULL;
	head = 0;
	tail = 1;
}

template <typename E>
Queue<E>::~Queue() {
	delete[] queue;
	queue = NULL;
}

template <typename E>
int Queue<E>::length() const {
	return size;
}

template <typename E>
void Queue<E>::enqueue(E e) {
	if (maxSize == 0) {
		queue = new E[2];
		queue[0] = e;
		head = 0;
		tail = 1;
		size = 1;
		maxSize = 2;
	}
	else {
		//使数组变为原来的两倍长
		if (size == maxSize - 1)
			resize(2 * maxSize - 1);

		queue[tail] = e;
		tail = (tail + 1) % maxSize;
		++size;
	}

}

template <typename E>
E Queue<E>::dequeue() {
	//使数组使用率大于50percents
	if (size <= (maxSize - 1) / 4)
		resize((maxSize - 1) / 2);

	E temp = queue[head];
	head = (head + 1) % maxSize;
	--size;
	return temp;

}

//改变底层数组大小
template <typename E>
void Queue<E>::resize(int newSize) {

	if (newSize <= 2)
		return;

	E* temp = queue;
	queue = new E[newSize];

	if (tail < head) {
		int j1, j2, k1, k2;
		for (j1 = head, k1 = 0; j1 < maxSize; ++j1, ++k1)
			queue[k1] = temp[j1];
		for (j2 = 0, k2 = k1; j2 < tail; ++j2, ++k2)
			queue[k2] = temp[j2];
		head = 0;
		tail = size;
	}
	else {
		for (int j = head, k = 0; j < tail; ++j, ++k)
			queue[k] = temp[j];
		head = 0;
		tail = size;
	}

	maxSize = newSize;
}
#endif
