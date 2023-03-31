#include "IObject.h"
namespace  {
	Tree<IObject*> _g_obj_tree;
}

IObject::IObject(IObject* parent) 
{
	if(parent!=nullptr)
	{
		auto parent_node =_g_obj_tree.Find(parent);
		if(parent_node!=nullptr)
		{
			_objNode_holder = parent_node->Insert(this);
		}
	}else{
		_objNode_holder = _g_obj_tree.Insert(this);
	}
}

IObject::~IObject()
{
	Node<IObject*>::SetStatus(_objNode_holder, false);
	Tree<IObject*>::DreakNode(_objNode_holder);
	Tree<IObject*>::ReleaseAllData(_objNode_holder);
}


