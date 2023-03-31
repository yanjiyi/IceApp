#ifndef IObject_TREE_HEADER
#define IObject_TREE_HEADER
#ifndef NDEBUG
#include <iostream>
#endif
#include <type_traits>

template<typename T>
class Node {
	public:
		Node() : _parent(nullptr),_children(0) {}
		Node(Node<T>* parent,T data) : _children(0),_status(true) {
			_parent = parent;
			_data = data;
		}

		virtual ~Node()
		{
			
		}
	
		Node<T>* Insert(T data) {
			Node<T>** _new_next = new Node<T>*[_children+1];
			/*if((_new_next*) == nullptr)
				throw std::runtime_error("IObject Tree Cann't Alloc Memory!");*/			
			for(int i=0;i<_children;i++)
			{
				_new_next[i] = _next[i];	
			}

			delete[] _next;
			_next = _new_next;

			_next[_children] = new Node<T>(this,data);
			_children++;

			return _next[_children-1];
		}

		static void SetStatus(Node<T>* node,bool status)
		{
			node->_status = status;
		}
		static Node<T>* Find(Node<T>* node,T data)
		{
			if(node!=nullptr)
			{
				if(node->_data == data)
					return node;
			}

			for(int i=0;i<node->_children;i++)
			{
				Node<T>* result = Find(node->_next[i],data);
				if(result!=nullptr)
					return result;
			}

			return nullptr;
		}

		Node<T>* Find(T data) { return Find(this,data); }
		
		static void Unlink(Node<T>* node)
		{
			if(node->_parent!=nullptr)
			{
				if(node->_children == 0)
				{
					int _realloc_children = node->_parent->_children-1;
					int cur = 0;

					Node<T>** _new_parent_next = new Node<T>*[_realloc_children];

					for(int i=0;i<node->_parent->_children;i++)
					{
						if(node->_parent->_next[i] != node)
							_new_parent_next[cur] = node->_parent->_next[i];
					}

					delete[] node->_parent->_next;

					node->_parent->_next = _new_parent_next;

					delete node;

					return;
				}

				int _new_children = node->_parent->_children + node->_children - 1;
				Node<T>** _new_next = new Node<T>*[_new_children];

				/*if(nullptr == _new_next*)
					throw std::runtime_error("IObject Tree Cann't Alloc Memory!");*/

				int i=0;
				int cur = 0;
				for(i;i<node->_parent->_children;i++)
				{
					if(node->_parent->_next[i] != node) {
						_new_next[cur] = node->_parent->_next[i];
						cur++;
					}
				}

				int cur_combind_index = 0;
				for(cur;cur<_new_children;i++)
				{
					_new_next[cur] = node->_next[cur_combind_index];
					if(node->_parent->_parent!=nullptr)
					{
						_new_next[cur]->_parent = node->_parent->_parent; 
					}else{
						_new_next[cur]->_parent = nullptr;
					}
					cur_combind_index++;
					cur++;
				}

				if(node->_children>0)
					delete[] node->_next;
				
				delete[] node->_parent->_next;

				node->_parent->_next = _new_next;
				node->_parent->_children = _new_children;
				delete node;
			}
		}

		static void Release(Node<T>* node) {
			if(node->_parent!=nullptr)
			{
				Node<T>** _new_next = new Node<T>*[node->_parent->_children-1];

				int cur = 0;
				for(int i=0;i<node->_parent->_children;i++)
				{
					if(node->_parent->_next[i]!=node)
					{
						_new_next[cur] = node->_parent->_next[i];
						cur++;
					}	
				}

				node->_parent->_children--;

				delete[] node->_parent->_next;
				node->_parent->_next = _new_next;
			}

			for(int i=0;i<node->_children;i++)
			{
				Release(node->_next[i]);
			}

			/*for(int i=0;i<node->_children;i++)
				delete node->_next[i];*/

			if(node->_children>0)
				delete[] node->_next;

			delete node;
		}	

		T _data;
		bool _status;

		Node<T>* _parent;
		Node<T>** _next;
		int _children = 0;
};

template<typename T>
class Tree{
	public:
		Tree() {
			root = new Node<T>();
		}

		virtual ~Tree() { 
#ifndef NDEBUG
			std::cout <<"Tree : " << this << " Destroy!" << std::endl;
#endif
			if(std::is_pointer<T>::value)
				ReleaseAllData();
#ifndef NDEBUG
			Print(root);
#endif
			if(root!=nullptr)
				Node<T>::Release(root);
		}

		Node<T>* Insert(T data) {
			if(root!=nullptr) {
				return root->Insert(data);
			}

			return nullptr;
		}

		Node<T>* Find(T data) {
			return root->Find(data);
		}

		static void ReleaseAllData(Node<T>* node){
			if(node->_status)
			{
#ifndef NDEBUG 
				std::cout <<"Release Node Data : " << node->_data << std::endl;
#endif
				delete node->_data;
			}

			for(int i=0;i<node->_children;i++)
			{
				ReleaseAllData(node->_next[i]);
			}
		}

 		void ReleaseAllData()
		{
			ReleaseAllData(root);
		}

#ifndef NDEBUG
		void Print(Node<T>* node)
		{
			std::cout << "Node : " << node << " , Storage Data : " << node->_data << " , Status : " << (node->_status ? "True" : "False") << std::endl;
			for(int i=0;i<node->_children;i++)
			{
				Print(node->_next[i]);
			}			
		}
#endif

		static void DreakNode(Node<T>* node)
		{
			if(node->_parent!=nullptr)
			{
				if(node->_parent->_children>1)
				{
					Node<T>** _new_next = new Node<T>*[node->_parent->_children-1];

					int cur = 0;
					for(int i=0;i<node->_parent->_children;i++)
					{
						if(node->_parent->_next[i]!=node)
						{
							_new_next[cur] = node->_parent->_next[i];
							cur++;
						}
					}

					delete[] node->_parent->_next;
					node->_parent->_next = _new_next;
				}else{
					delete[] node->_parent->_next;
				}

				node->_parent->_children--;
			}
		}
	protected:
		Node<T>* root = nullptr;
};

#endif
