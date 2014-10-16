#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <list>
#include <string>
#include <vector>

#include <VBE/utils/NonCopyable.hpp>

class Game;
class ContainerObject;
class GameObject : public NonCopyable { //scenegraph nodes
	public:
		GameObject();
		virtual ~GameObject();

		void addTo(GameObject* parent);
		void removeAndDelete();

		void setName(std::string newName);
		std::string getName() const;
		int getDrawPriority() const;
		int getUpdatePriority() const;
		void setDrawPriority(int newPriority);
		void setUpdatePriority(int newPriority);

		mat4f getFullTransform() const;

		template<class T>
		void getAllObjectsOfType(std::vector<T*> &v) const {
			T* p = const_cast<T*>(dynamic_cast<const T*>(this));
			if(p) v.push_back(p);
			for(std::list<GameObject*>::const_iterator it = children.begin(); it != children.end(); ++it)
				(*it)->getAllObjectsOfType<T>(v);
		}

		template<class T>
		T* getFirstObjectOfType() const {
			T* p = const_cast<T*>(dynamic_cast<const T*>(this));
			if(p) return p;
			for(std::list<GameObject*>::const_iterator it = children.begin(); it != children.end(); ++it) {
				p = (*it)->getFirstObjectOfType<T>();
				if(p) return p;
			}
			return nullptr;
		}

		const std::list<GameObject*>& getChildren() const;

		const int id;
	protected:
		virtual void update(float deltaTime);
		virtual void draw() const ;
		virtual void onObjectAdd(GameObject* object);

		Game* getGame() const;
		const GameObject* getParent() const;

		//Model matrix
		mat4f transform;
		mutable mat4f fullTransform;
	private:
		void removeFromParent();
		void propragateTransforms() const;
		void markForDelete();

		virtual void addToContainer(GameObject* obj);
		virtual void removeFromContainer(GameObject* obj);

		GameObject* parent;
		std::list<GameObject*> children;
		int drawPriority;
		int updatePriority;
		std::string name;
		ContainerObject* container;
		bool isAlive;

		friend class ContainerObject;
};

#endif // GAMEOBJECT_HPP