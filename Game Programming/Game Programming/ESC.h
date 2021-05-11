#ifndef ESC
#define ESC

#include <bitset>
#include <vector>
#include <array>
#include <iostream>

#define MAXCOMPONENTS 32

using namespace std;
class Entity;
class Component;

// inline -> could be faster than normal function
inline size_t getComponentTypeID() {
	static size_t id = 0;
	return id++;
}

// Get Id of specific Component
template<typename T>
inline size_t getComponentTypeID() {
	static size_t componentType = getComponentTypeID();
	return componentType;
}

class Component {
public:
	Entity* entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}
	virtual ~Component() {}

};

class Entity {

private:
	bool active;
	
	vector<Component*> components;
	bitset<MAXCOMPONENTS> componentBitset;
	array<Component*, MAXCOMPONENTS> componentArray;

public:
	
	Entity() {
		active = true;
		componentArray = array<Component*, MAXCOMPONENTS>();
	}

	void update() {
		for (size_t i = 0; i < components.size(); i++) {

			components[i]->update();
		}
	}

	void draw() {
		for (size_t i = 0; i < components.size(); i++) {
			components[i]->draw();
		}
	}

	bool isActive() {
		return active;
	}

	void destroy() {
		active = false;
	}

	template<typename T>
	bool hasComponent() {
		return componentBitset[getComponentTypeID<T>()];
	}

	template<typename T, typename... Targs>
	T& addComponent(Targs&&... params) {
		T* component = new T(forward<Targs>(params)...);
		component->entity = this;

		components.push_back(component);
		componentArray[getComponentTypeID<T>()] = component;
		componentBitset[getComponentTypeID<T>()] = true;

		component->init();
		return *component;
	}

	template<typename T>
	T& getComponent() {
		return *static_cast<T*>(componentArray[getComponentTypeID<T>()]);
	}

};


class Manager {
private:
	vector<Entity*> entities;
public:
	
	void update() {
		for (size_t i = 0; i < entities.size(); i++) {
			entities[i]->update();
		}
	}
	void draw() {
		for (size_t i = 0; i < entities.size(); i++) {
			entities[i]->draw();
		}
	}

	void refresh() {
		for (size_t i = 0; i < entities.size(); i++) {
			if (!(entities[i]->isActive())) {
				entities.erase(entities.begin() + i);
			}
		}
		
	}

	Entity* addEntity() {
		Entity* entity = new Entity();
		entities.push_back(entity);
		return entity;
	}
};


#endif
