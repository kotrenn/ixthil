#ifndef __LEVEL_H
#define __LEVEL_H

#include "actor.h"
#include "background.h"
#include "bullet.h"
#include "common.h"
#include "item.h"
#include "surface.h"

class Actor;
class Bullet;
class Item;

class Level
{
public:
	//Level(int w, int h);
	Level(const char *filename, Background *background);
	~Level();

	void pause();
	void unpause();

	void update();
	void move(double dt);
	void draw(Surface *dst) const;

	string get_name() const { return m_name; }
	int get_w() const { return m_w; }
	int get_h() const { return m_h; }
	Rect get_dims() const { return Rect(0, 0, m_w, m_h); }
	void set_level(int level) { m_level = level; }
	int get_level() const { return m_level; }
	Actor *get_player() const { return m_player; }
	Actor *get_boss() const { return m_boss; }
	bool subspace() const { return m_subspace; }
	void set_subspace(bool subspace) { m_subspace = subspace; }
	void set_boss(Actor *actor) { m_boss = actor; }
	void set_size(int w, int h) { m_w = w; m_h = h; }

	void set_option(const string &option, bool value);
	bool get_option(const string &option) const;
	
	string get_next() const { return m_next; }
	void set_next(const string &next) { m_next = next; }

	void enter_subspace() { m_entering_subspace = true; }
	bool entering_subspace() const { return m_entering_subspace; }

	bool done();

	void add_actor(Actor *actor);
	void add_bullet(Bullet *bullet);
	void add_item(Item *item);
	
	void remove_actor(Actor *actor);
	void remove_bullet(Bullet *bullet);
	void remove_item(Item *item);

	void move_bullets(Level *level);
	void clear_bullets();

	bool out_of_bounds(const Bullet *bullet) const;
	bool out_of_bounds(const Item *item) const;
	void get_collisions(list<Actor *> *actors,
	                    const Shape *shape,
	                    int team) const;
	void get_collisions(list<Bullet *> *bullets,
	                    const Shape *shape,
	                    int team) const;
	void get_collisions(list<Item *> *items,
	                    const Shape *shape) const;

	Actor *random_target(int team) const;
private:
	void add_lists();
	void clear_lists();

	string m_name;
	int m_w;
	int m_h;
	Background *m_background;

	int m_level;
	bool m_subspace;
	Actor *m_player;
	Actor *m_boss;

	map<string, bool> m_options;

	string m_next;
	bool m_entering_subspace;
	
	set<Actor *> m_actors;
	set<Actor *> m_actors_add;
	set<Actor *> m_actors_remove;
	
	set<Bullet *> m_bullets;
	set<Bullet *> m_bullets_add;
	set<Bullet *> m_bullets_remove;

	set<Item *> m_items;
	set<Item *> m_items_add;
	set<Item *> m_items_remove;
};

#endif
