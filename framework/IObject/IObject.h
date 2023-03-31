#ifndef IOBJECT_HEADER
#define IOBJECT_HEADER
#include "internal/tree.h"

class IObject {
	public:
		IObject(IObject* parent = nullptr);
		virtual ~IObject();

	private:
		Node<IObject*>* _objNode_holder = nullptr;
};

#endif
