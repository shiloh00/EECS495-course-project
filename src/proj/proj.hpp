#pragma once

#include <iostream>
#include <memory>
#include <cstring>
#include <algorithm>
#include <exception>
#include <stdexcept>

namespace proj
{ 
	template<typename K, typename V>
	class skiplist {
		private:
			static const size_t MAX_LEVEL;

			struct node {
				K key;
				V value;
				std::vector<node*> ptr_list;
				node(K k, V v, size_t l) : key(k), value(v), ptr_list(l) {}
			};

			node* find_node_ptr(K key, bool del, bool create, V* value);

			size_t random_level();

			node* level[MAX_LEVEL];

		public:

			void print();

			bool find(K key);

			void insert(K key, V value);

			V& get(K key);

			void remove(K key);

			skiplist();

			~skiplist();
	};

	template<typename K, typename V>
	const size_t skiplist<K, V>::MAX_LEVEL = 20;

	template<typename K, typename V>
	skiplist<K, V>::skiplist() {
		memset(level, 0, sizeof(level));
	}

	template<typename K, typename V>
	skiplist<K, V>::~skiplist() {
		node* p = level[0];
		while(p) {
			node* old = p;
			p = p->ptr_list[0];
			delete old;
		}
	}

	template<typename K, typename V>
	void skiplist<K, V>::print() {
		node* p = level[0];
		while(p) {
			std::cout << p->value << std::endl;
		}
	}

	template<typename K, typename V>
	size_t skiplist<K, V>::random_level() {
		size_t idx = 1;
		while(rand() & 0x1)
			idx++;
		return MAX_LEVEL < idx ? MAX_LEVEL : idx;
	}

	template<typename K, typename V>
	bool skiplist<K, V>::find(K key) {
		node* res = find_node_ptr(key, false, false, nullptr);
		return res != nullptr;
	}

	template<typename K, typename V>
	void skiplist<K, V>::insert(K key, V value) {
		find_node_ptr(key, false, true, &value);
	}
	
	template<typename K, typename V>
	V& skiplist<K, V>::get(K key) {
		node* res = find_node_ptr(key, false, false, nullptr);
		if(res)
			return res->value;
		throw std::invalid_argument("Key not found");
	}

	template<typename K, typename V>
	void skiplist<K, V>::remove(K key) {
		find_node_ptr(key, true, false, nullptr);
	}
	

	template<typename K, typename V>
	auto skiplist<K, V>::find_node_ptr(K key, bool del, bool create, V* value) -> node* {
		node *res = nullptr, *last = nullptr;
		node* ptrs[MAX_LEVEL];

		if(create) {
			memset(ptrs, 0, MAX_LEVEL);
		}

		size_t idx = MAX_LEVEL;
		do {
			idx--;
			node* p = last ? last->ptr_list[idx] : level[idx];
			while(p) {
				if(!(p->key < key) && !(key < p->key))
					res = p;
				if(!(p->key < key))
					break;
				last = p;
				p = p->ptr_list[idx];
			}
			if(create && del)
				ptrs[idx] = last;
		} while(idx > 0);

		if(create && value) {
			if(res) {
				res->value = *value;
			} else {
				size_t l = random_level();
				node* new_node = new node(key, *value, l);
				for(idx = 0; idx < l; idx++) {
					if(ptrs[idx]) {
						new_node->ptr_list[idx] = ptrs[idx]->ptr_list[idx];
						ptrs[idx]->ptr_list[idx] = new_node;
					} else {
						level[idx] = new_node;
					}
				}
			}
		}
		if(res && del) {
			size_t len = res->ptr_list.size();
			for(idx = 0; idx < len; idx++) {
				if(ptrs[idx]) {
					ptrs[idx]->ptr_list[idx] = res->ptr_list[idx];
				} else {
					level[idx] = res->ptr_list[idx];
				}
			}
			delete res;
			res = nullptr;
		}

		return res;
	}

} // namespace proj
