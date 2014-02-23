#include "ContainerObject.hpp"

ContainerObject::ContainerObject() {
}

void ContainerObject::update(float deltaTime) {
	while(!objectTasksToRemove.empty()) {
		GameObject* obj = objectTasksToRemove.front();
		updateTasks.erase(obj);
		drawTasks.erase(obj);
		objectTasksToRemove.pop();
		if(!obj->isAlive) delete obj;
	}

	while(!objectTasksToAdd.empty()) {
		GameObject* obj = objectTasksToAdd.front();
		updateTasks.insert(obj);
		drawTasks.insert(obj);
		objectTasksToAdd.pop();
	}

	for(std::set<GameObject*, FunctorCompareUpdate>::iterator it = updateTasks.begin(); it != updateTasks.end(); ++it)
		(*it)->update(deltaTime);
}

void ContainerObject::draw() const {
	propragateTransforms();
	for(std::set<GameObject*, FunctorCompareDraw>::iterator it = drawTasks.begin(); it != drawTasks.end(); ++it)
		(*it)->draw();
}

void ContainerObject::addToContainer(GameObject* obj) {
	VBE_ASSERT(obj->container == nullptr, "Adding an object to container that is already in a container.");
	obj->container = this;
	objectTasksToAdd.push(obj);
	if(dynamic_cast<ContainerObject*> (obj) == nullptr)
		for(std::list<GameObject*>::iterator it = obj->children.begin(); it != obj->children.end(); ++it)
			addToContainer(*it);
}

void ContainerObject::removeFromContainer(GameObject* obj) {
	VBE_ASSERT(obj->container != nullptr, "Removing an object from a container that is not in a container.");
	obj->container = nullptr;
	objectTasksToRemove.push(obj);
	if(dynamic_cast<ContainerObject*> (obj) == nullptr)
		for(std::list<GameObject*>::iterator it = obj->children.begin(); it != obj->children.end(); ++it)
			removeFromContainer(*it);
}