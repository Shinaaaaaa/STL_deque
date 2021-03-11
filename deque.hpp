#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>

namespace sjtu { 

template<class T>
class deque {

private:
    class Block{
    private:
        T **nodeArray;
        int count = 0;
        Block *next = nullptr , *previous = nullptr;
    public:
        Block(){
            nodeArray = new T* [200];
        }

        ~Block(){
            for (int i = 0 ; i < count ; ++i){
                delete nodeArray[i];
                nodeArray[i] = nullptr;
            }
            delete []nodeArray;
            nodeArray = nullptr;
            count = 0;
        }

        Block(const Block &other){
            count = other.count;
            nodeArray = new T* [200];
            for (int i = 0 ; i < count ; ++i){
                nodeArray[i] = new T (other.nodeArray[i][0]);
            }
        }

        void Split(){
            Block *tmp;
            tmp = new Block;
            for (int i = 100 ; i < count ; ++i){
                tmp->nodeArray[i - 100] = new T (nodeArray[i][0]);
                delete nodeArray[i];
                nodeArray[i] = nullptr;
            }
            tmp->next = next;
            tmp->previous = this;
            next->previous = tmp;
            next = tmp;
            tmp->count = count - 100;
            count = 100;
            /*Block *tmp;
            tmp = new Block;
            for (int i = 50 ; i < o->count ; ++i){
                tmp->nodeArray[i - 50] = new T (o->nodeArray[i][0]);
                delete o->nodeArray[i];
                o->nodeArray[i] = nullptr;
            }
            tmp->next = o->next;
            tmp->previous = o;
            o->next->previous = tmp;
            o->next = tmp;*/
        }

//        void Merge(){
//            Block *tmp = previous;
//            for (int i = 0 ; i < count ; ++i){
//                tmp->nodeArray[i + tmp->count] = new T (nodeArray[i][0]);
//                delete nodeArray[i];
//                nodeArray[i] = nullptr;
//            }
//            tmp->next = next;
//            next->previous = tmp;
//            delete nodeArray;
//            delete this;
//            /*
//             Block *tmp = o->previous;
//            for (int i = 0 ; i < o->count ; ++i){
//                tmp->nodeArray[i + tmp->count] = new T (o->nodeArray[i][0]);
//                delete o->nodeArray[i];
//                o->nodeArray[i] = nullptr;
//            }
//            tmp->next = o->next;
//            o->next->previous = tmp;
//            delete o;*/
//        }

        void addNode(int p , const T &value){
            for (int i = count - 1 ; i >= p ; --i){
                nodeArray[i + 1] = nodeArray[i];
            }
            nodeArray[p] = new T (value);
            count++;
//            if (count >= 95) Split();
        }

        void deleteNode(int p){
            delete nodeArray[p];
            for (int i = p ; i < count - 1; ++i){
                nodeArray[i] = nodeArray[i + 1];
            }
            nodeArray[count - 1] = nullptr;
            --count;
        }

        friend class deque<T>;
    };
    Block *head , *tail;
    int number = 0;
public:
	class const_iterator;
	class iterator {
        friend class deque<T>;
	private:
	    deque<T> *d;
	    Block *b;
	    int l = -1;
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
	public:
	    iterator() = default;
	    iterator(deque<T> *dd , Block *bb , int ll):d(dd) , b(bb) , l(ll){}
		/**
		 * return a new iterator which pointer n-next elements
		 *   if there are not enough elements, iterator becomes invalid
		 * as well as operator-
		 */
		iterator operator+(const int &n) const {
            if (n < 0) return *this - -n;
		    int position = l + n;
		    Block *tmp = b;
		    while(position >= tmp->count && tmp->next->next != nullptr){
		        position -= tmp->count;
                tmp = tmp->next;
		    }
            return iterator(d , tmp , position);
			//TODO
		}
		iterator operator-(const int &n) const {
            if (n < 0) return *this + -n;
            int position = l - n;
            Block *tmp = b;
            while(position < 0){
                tmp = tmp->previous;
                position = tmp->count + position;
            }
            return iterator(d , tmp , position);
			//TODO
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const {
		    if (d != rhs.d) throw "error";
            if (b == rhs.b) return l - rhs.l;
		    Block *tmp = rhs.b;
		    int position = rhs.l;
		    int len = 0;
            while(tmp != b){
                len += (tmp->count - position);
                position = 0;
                tmp = tmp->next;
                if (tmp == nullptr) return -(rhs - *this);
            }
            len += l;
            return len;
			//TODO
		}
		iterator& operator+=(const int &n) {
            if (n < 0) return *this -= -n;
            int position = l + n;
            if (b->next == d->tail && position == b->count) {
                l = position;
                return *this;
            }
            while(position >= b->count  && b->next->next != nullptr){
                position -= b->count;
                b = b->next;
            }
            l = position;
            return *this;
			//TODO
		}
		iterator& operator-=(const int &n) {
            if (n < 0) return *this += -n;
            int position = l - n;
            while(position < 0){
                b = b->previous;
                position = b->count + position;
            }
            l = position;
            return *this;
			//TODO
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
		    iterator tmp = *this;
            int position = l + 1;
            if (b->next == d->tail && position == b->count) {
                l = position;
                return tmp;
            }
            while(position >= b->count && b->next->next != nullptr){
                position -= b->count;
                b = b->next;
            }
            l = position;
            return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
            int position = l + 1;
            if (b->next == d->tail && position == b->count) {
                l = position;
                return *this;
            }
            while(position >= b->count && b->next->next != nullptr){
                position -= b->count;
                b = b->next;
            }
            l = position;
            return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
            iterator tmp = *this;
            int position = l - 1;
            while(position < 0){
                b = b->previous;
                if (b == nullptr) throw "error";
                position = b->count + position;
            }
            l = position;
            return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
            int position = l - 1;
            while(position < 0){
                b = b->previous;
                if (b == nullptr) throw "error";
                position = b->count + position;
            }
            l = position;
            return *this;
		}
		/**
		 * TODO *it
		 * 		throw if iterator is invalid
		 */
		T& operator*() const {
		    if (l >= b->count || l < 0) throw "error";
            return b->nodeArray[l][0];
		}
		/**
		 * TODO it->field
		 * 		throw if iterator is invalid
		 */
		T* operator->() const noexcept {
            if (l >= b->count || l < 0) throw "error";
            return b->nodeArray[l];
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {
            if (d != rhs.d || b != rhs.b || l != rhs.l) return false;
            else return true;
		}
		bool operator==(const const_iterator &rhs) const {
            if (d != rhs.d || b != rhs.b || l != rhs.l) return false;
            else return true;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
            if (d != rhs.d || b != rhs.b || l != rhs.l) return true;
            else return false;
		}
		bool operator!=(const const_iterator &rhs) const {
            if (d != rhs.d || b != rhs.b || l != rhs.l) return true;
            else return false;
		}
	};
    class const_iterator {
        friend class deque<T>;
        // it should has similar member method as iterator.
        //  and it should be able to construct from an iterator.
    private:
        const deque<T> *d;
        Block *b;
        int l = -1;
        // data members.
    public:
        const_iterator() = default;
        const_iterator(const deque<T> *dd , Block *bb , int ll):d(dd) , b(bb) , l(ll){}
        const_iterator operator+(const int &n) const {
            if (n < 0) return *this - -n;
            int position = l + n;
            Block *tmp = b;
            while(position >= tmp->count && tmp->next->next != nullptr){
                position -= tmp->count;
                tmp = tmp->next;
            }
            return const_iterator(d , tmp , position);
            //TODO
        }
        const_iterator operator-(const int &n) const {
            if (n < 0) return *this + -n;
            int position = l - n;
            Block *tmp = b;
            while(position < 0){
                tmp = tmp->previous;
                position = tmp->count + position;
            }
            return const_iterator(d , tmp , position);
            //TODO
        }
        // return th distance between two iterator,
        // if these two iterators points to different vectors, throw invaild_iterator.
        int operator-(const const_iterator &rhs) const {
            if (d != rhs.d) throw "error";
            if (b == rhs.b) return l - rhs.l;
            Block *tmp = rhs.b;
            int position = rhs.l;
            int len = 0;
            while(tmp != b){
                len += (tmp->count - position);
                position = 0;
                tmp = tmp->next;
                if (tmp == nullptr) return -(rhs - *this);
            }
            len += l;
            return len;
            //TODO
        }
        const_iterator& operator+=(const int &n) {
            if (n < 0) return *this -= -n;
            int position = l + n;
            if (b->next == d->tail && position == b->count) {
                l = position;
                return *this;
            }
            while(position >= b->count  && b->next->next != nullptr){
                position -= b->count;
                b = b->next;
            }
            l = position;
            return *this;
            //TODO
        }
        const_iterator& operator-=(const int &n) {
            if (n < 0) return *this += -n;
            int position = l - n;
            while(position < 0){
                b = b->previous;
                position = b->count + position;
            }
            l = position;
            return *this;
            //TODO
        }
        /**
         * TODO iter++
         */
        const_iterator operator++(int) {
            iterator tmp = *this;
            int position = l + 1;
            if (b->next == d->tail && position == b->count) {
                l = position;
                return tmp;
            }
            while(position >= b->count && b->next->next != nullptr){
                position -= b->count;
                b = b->next;
            }
            l = position;
            return tmp;
        }
        /**
         * TODO ++iter
         */
        const_iterator& operator++() {
            int position = l + 1;
            if (b->next == d->tail && position == b->count) {
                l = position;
                return *this;
            }
            while(position >= b->count && b->next->next != nullptr){
                position -= b->count;
                b = b->next;
            }
            l = position;
            return *this;
        }
        /**
         * TODO iter--
         */
        const_iterator operator--(int) {
            iterator tmp = *this;
            int position = l - 1;
            while(position < 0){
                b = b->previous;
                if (b == nullptr) throw "error";
                position = b->count + position;
            }
            l = position;
            return tmp;
        }
        /**
         * TODO --iter
         */
        const_iterator& operator--() {
            int position = l - 1;
            while(position < 0){
                b = b->previous;
                if (b == nullptr) throw "error";
                position = b->count + position;
            }
            l = position;
            return *this;
        }
        /**
         * TODO *it
         * 		throw if iterator is invalid
         */
        const T& operator*() const {
            if (l >= b->count || l < 0) throw "error";
            return b->nodeArray[l][0];
        }
        /**
         * TODO it->field
         * 		throw if iterator is invalid
         */
        const T* operator->() const noexcept {
            if (l >= b->count || l < 0) throw "error";
            return b->nodeArray[l];
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            if (d != rhs.d || b != rhs.b || l != rhs.l) return false;
            else return true;
        }
        bool operator==(const const_iterator &rhs) const {
            if (d != rhs.d || b != rhs.b || l != rhs.l) return false;
            else return true;
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            if (d != rhs.d || b != rhs.b || l != rhs.l) return true;
            else return false;
        }
        bool operator!=(const const_iterator &rhs) const {
            if (d != rhs.d || b != rhs.b || l != rhs.l) return true;
            else return false;
        }

        const_iterator(const const_iterator &other) {
            b = other.b;
            d = other.d;
            l = other.l;
            // TODO
        }
        const_iterator(const iterator &other) {
            b = other.b;
            d = other.d;
            l = other.l;
            // TODO
        }
        // And other methods in iterator.
        // And other methods in iterator.
        // And other methods in iterator.
    };
	/**
	 * TODO Constructors
	 */
	deque() {
	    head = new Block;
	    tail = new Block;
	    head->next = tail;
	    tail->previous = head;
	    number = 0;
	}
	deque(const deque &other) {
	    number = other.number;
	    head = new Block (*(other.head));
	    Block *q = head;
	    Block *p = other.head;
	    while (p->next){
            q->next = new Block (*(p->next));
	        q->next->previous = q;
	        p = p->next;
	        q = q->next;
	    }
	    tail = q;
	}
	/**
	 * TODO Deconstructor
	 */
	~deque() {
	    Block *q = head;
	    Block *p = q->next;
	    while(q->next){
	        delete q;
	        q = p;
	        p = p->next;
	    }
	    delete q;
	    number = 0;
	}
	/**
	 * TODO assignment operator
	 */
	deque &operator=(const deque &other) {
        if (this == &other) return *this;
        Block *q = head;
        Block *p = q->next;
        while(q->next){
            delete q;
            q = p;
            p = p->next;
        }
        delete q;
        head = tail = nullptr;//delete
        number = other.number;
        head = new Block (*(other.head));
        Block *q2 = head;
        Block *p2 = other.head;
        while (p2->next){
            q2->next = new Block (*(p2->next));
            q2->next->previous = q2;
            p2 = p2->next;
            q2 = q2->next;
        }
        tail = q2;
        return *this;
	}
	/**
	 * access specified element with bounds checking
	 * throw index_out_of_bound if out of bound.
	 */
	T & at(const size_t &pos) {
	    if (pos >= number || pos < 0) throw index_out_of_bound();
	    Block *tmp = head;
	    int position = pos;
	    while(position >= tmp->count){
	        position -= tmp->count;
	        tmp = tmp->next;
	    }
        return tmp->nodeArray[position][0];
	}
	const T & at(const size_t &pos) const {
        if (pos >= number || pos < 0) throw index_out_of_bound();
        Block *tmp = head;
        int position = pos;
        while(position >= tmp->count){
            position -= tmp->count;
            tmp = tmp->next;
        }
        return tmp->nodeArray[position][0];
	}
	T & operator[](const size_t &pos) {
        if (pos >= number || pos < 0) throw index_out_of_bound();
        Block *tmp = head;
        int position = pos;
        while(position >= tmp->count){
            position -= tmp->count;
            tmp = tmp->next;
        }
        return tmp->nodeArray[position][0];
	}
	const T & operator[](const size_t &pos) const {
        if (pos >= number || pos < 0) throw index_out_of_bound();
        Block *tmp = head;
        int position = pos;
        while(position >= tmp->count){
            position -= tmp->count;
            tmp = tmp->next;
        }
        return tmp->nodeArray[position][0];
	}
	/**
	 * access the first element
	 * throw container_is_empty when the container is empty.
	 */
	const T & front() const {
	    if (number == 0) throw container_is_empty();
        return head->nodeArray[0][0];
	}
	/**
	 * access the last element
	 * throw container_is_empty when the container is empty.
	 */
	const T & back() const {
        if (number == 0) throw container_is_empty();
        return tail->previous->nodeArray[tail->previous->count - 1][0];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
        return iterator(this , head , 0);
	}
	const_iterator cbegin() const {
        return const_iterator(this , head , 0);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
        return iterator(this , tail->previous , tail->previous->count);
	}
	const_iterator cend() const {
        return const_iterator(this , tail->previous , tail->previous->count);
	}
	/**
	 * checks whether the container is empty.
	 */
	bool empty() const {
        if (number == 0) return true;
        else return false;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
        return number;
	}
	/**
	 * clears the contents
	 */
	void clear() {
        Block *q = head;
        Block *p = q->next;
        while(q->next){
            delete q;
            q = p;
            p = p->next;
        }
        delete q;
        head = tail = nullptr;
        head = new Block;
        tail = new Block;
        head->next = tail;
        tail->previous = head;
        number = 0;
	}
	/**
	 * inserts elements at the specified locat on in the container.
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value
	 *     throw if the iterator is invalid or it point to a wrong place.
	 */
	iterator insert(iterator pos, const T &value) {
        if (pos.d != this) throw "error";
        int position = pos.l;
        Block *tmp = pos.b;
        tmp->addNode(position , value);
        if (tmp->count >= 195) {
            tmp->Split();
            if (position >= 100) {
                tmp = tmp->next;
                position -= 100;
            }
        }
//        for (int i = tmp->count - 1 ; i >= position ; --i){
//            tmp->nodeArray[i + 1] = tmp->nodeArray[i];
//        }
//        tmp->nodeArray[position] = new T(value);
//        tmp->count++;
        number++;
        return iterator(this , tmp , position);
	}
	/**
	 * removes specified element at pos.
	 * removes the element at pos.
	 * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
	 * throw if the container is empty, the iterator is invalid or it points to a wrong place.
	 */
	iterator erase(iterator pos) {
	    if (pos.d != this) throw "error";
	    if (pos.l >= pos.b->count || pos.l < 0) throw "error";
        int position = pos.l;
        Block *tmp = pos.b;
        tmp->deleteNode(position);
//        delete tmp->nodeArray[position];
//        for (int i = position ; i < tmp->count - 1; ++i){
//            tmp->nodeArray[i] = tmp->nodeArray[i + 1];
//        }
//        tmp->nodeArray[tmp->count - 1] = nullptr;
//        --tmp->count;
        --number;
        if (tmp->count == 0) {
            if (tmp != head){
                tmp->previous->next = tmp->next;
                tmp->next->previous = tmp->previous;
                if (tmp->next != tail){
                    Block *tmp2 = tmp->next;
                    delete tmp;
                    return iterator(this , tmp2 , 0);
                }
                else {
                    delete tmp;
                    return pos.d->end();
                }
            }
            else if (tmp->next != tail){
                head = tmp->next;
                delete tmp;
                return iterator(this , head , 0);
            }
            else pos.d->end();
        }
        if (position == tmp->count) {
            if (tmp->next != tail){
                tmp = tmp->next;
                position = 0;
            }
            else return pos.d->end();
        }
        return iterator(this , tmp , position);
	}
	/**
	 * adds an element to the end
	 */
	void push_back(const T &value) {
	    Block *tmp = tail->previous;
	    tmp->addNode(tmp->count , value);
	    if (tmp->count >= 195) tmp->Split();
//	    tmp->nodeArray[tmp->count] = new T (value);
//	    tmp->count++;
	    number++;
	}
	/**
	 * removes the last element
	 *     throw when the container is empty.
	 */
	void pop_back() {
        Block *tmp = tail->previous;
        tmp->deleteNode(tmp->count - 1);
//        delete tmp->nodeArray[tmp->count];
//        --tmp->count;
        --number;
        if (tmp->count == 0) {
            if (tmp != head){
                tmp->previous->next = tmp->next;
                tmp->next->previous = tmp->previous;
                delete tmp;
            }
            else if (tmp->next != tail){
                delete head;
                head = tmp->next;
            }
        }
	}
	/**
	 * inserts an element to the beginning.
	 */
	void push_front(const T &value) {
	    Block *tmp = head;
	    tmp->addNode(0 , value);
        if (tmp->count >= 195) tmp->Split();
//	    for (int i = tmp->count - 1 ; i >= 0 ; --i){
//	        tmp->nodeArray[i + 1] = tmp->nodeArray[i];
//	    }
//	    tmp->nodeArray[0] = new T (value);
//        tmp->count++;
        number++;
	}
	/**
	 * removes the first element.
	 *     throw when the container is empty.
	 */
	void pop_front() {
        Block *tmp = head;
        tmp->deleteNode(0);
//        delete tmp->nodeArray[0];
//        for (int i = 0 ; i < tmp->count - 1 ; ++i){
//            tmp->nodeArray[i] = tmp->nodeArray[i + 1];
//        }
//        --tmp->count;
        --number;
        if (tmp->count == 0) {
            if (tmp != head){
                tmp->previous->next = tmp->next;
                tmp->next->previous = tmp->previous;
                delete tmp;
            }
            else if (tmp->next != tail){
                head = tmp->next;
                delete tmp;
            }
        }
	}
};

}

#endif
