#ifndef SKILL_H
#define SKILL_H

#include "GameObject.h"

class Skill : public GameObject{
	public:
		virtual void update(float delta) = 0;

		virtual void render() = 0;

		bool is_dead(){
			return (elapsed_time.get() > duration);
		}

		virtual void notify_collision(GameObject &object) = 0;
		virtual bool is(string type) = 0;
	
	private:
		int id_player, damage;
		double duration;

		bool collidable, gravity_affected, follow_parent, has_sprite;
		
		Timer elapsed_time;
};

#endif