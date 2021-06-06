#ifndef ESC
#define ESC

#include <bitset>
#include <vector>
#include <array>
#include <iostream>

#define MAXCOMPONENTS 32
#define MAXGROUPS 32

using namespace std;
class Entity;
class Component;
class Manager;

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
	bitset<MAXGROUPS> groupBitset;
	array<Component*, MAXCOMPONENTS> componentArray;

public:
	
	Entity() {
		active = true;
		componentArray = array<Component*, MAXCOMPONENTS>();
	}

	~Entity() {
		for (size_t i = 0; i < components.size(); i++) {
			Component* component = components[i];
			components.erase(components.begin() + i);
			delete component;
		}
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
	// forwad needs &&
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

	bool hasGroup(size_t group) {
		return groupBitset[group];
	}

	void addGroup(size_t group);

	void delGroup(size_t group) {
		groupBitset[group] = false;
	}
};


class Manager {
private:
	vector<Entity*> entities;
	array<vector<Entity*>, MAXGROUPS> groupedEntities;
public:
	
	/**
	* Calls update function of entities
	*/
	void update() {
		for (size_t i = 0; i < entities.size(); i++) {
			entities[i]->update();
		}
	}
	/**
	* Calls draw function of entities
	*/
	void draw() {
		for (size_t i = 0; i < entities.size(); i++) {
			entities[i]->draw();
		}
	}
	/**
	* Checks if every entity is still active, if not remove it from list
	*/
	void refresh() {
		// Check Groups
		for (size_t i = 0; i < MAXGROUPS; i++) {
			vector<Entity*> vEntities = groupedEntities[i];
			for (size_t j = 0; j < vEntities.size(); j++) {
				if ( !(vEntities[j]->isActive()) || !(vEntities[j]->hasGroup(i)) )  {
					vEntities.erase(vEntities.begin() + j);
					groupedEntities[i] = vEntities;
				}
			}
		}
		// Check Entities
		for (size_t i = 0; i < entities.size(); i++) {
			if (!(entities[i]->isActive())) {
				Entity* entity = entities[i];
				entities.erase(entities.begin() + i);
				delete entity;
			}
		}
		
	}

	Entity* addEntity() {
		Entity* entity = new Entity();
		entities.push_back(entity);
		return entity;
	}

	void addToGroup(Entity* entity, size_t group) {
		groupedEntities[group].push_back(entity);
	}

	vector<Entity*>& getGroup(size_t group) {
		return groupedEntities[group];
	}
};


#endif
