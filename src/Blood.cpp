#include "Blood.h"

#define CROUCH_COOLDOWN 100.0

Blood::Blood(string name, float x, float y, int cjoystick_id) : Fighter(cjoystick_id){
	sprite[IDLE] = Sprite(name + "/idle.png", 12, 10);
	sprite[RUNNING] = Sprite(name + "/running.png", 8, 10);
	sprite[JUMPING] = Sprite(name + "/jumping.png", 6, 10);
	sprite[FALLING] = Sprite(name + "/falling.png", 2, 10);
	sprite[CROUCH] = Sprite(name + "/crouch.png", 3, 20);
	sprite[IDLE_ATK_NEUTRAL_1] = Sprite(name + "/idle_atk_neutral_1.png", 4, 10);
	sprite[IDLE_ATK_NEUTRAL_2] = Sprite(name + "/idle_atk_neutral_2.png", 4, 10);
	sprite[IDLE_ATK_NEUTRAL_3] = Sprite(name + "/idle_atk_neutral_3.png", 3, 10);
	sprite[IDLE_ATK_FRONT] = Sprite(name + "/idle_atk_front.png", 5, 10);
	sprite[IDLE_ATK_UP] = Sprite(name + "/idle_atk_up.png", 5, 10);
	sprite[IDLE_ATK_DOWN] = Sprite(name + "/idle_atk_down.png", 6, 10);
	sprite[CROUCH_ATK] = Sprite(name + "/crouch_atk.png", 3, 10);
	sprite[JUMP_ATK_DOWN] = Sprite(name + "/jump_atk_down.png", 4, 10);
	sprite[JUMP_ATK_UP] = Sprite(name + "/jump_atk_up.png", 4, 10);
	sprite[DEFENDING] = Sprite(name + "/defending.png", 2, 10);
	sprite[STUNT] = Sprite(name + "/stunt.png", 2, 10);

	tags["blood"] = true;
	box = Rectangle(x, y, sprite[state].get_width(), sprite[state].get_height());
}

void Blood::update_machine_state(){
	switch(state){
		case FighterState::IDLE_ATK_NEUTRAL_1:
			attack_damage = 3 * (sprite[state].get_current_frame() == 1);
			attack_mask = (orientation == Orientation::LEFT ? AttackDirection::ATK_LEFT : AttackDirection::ATK_RIGHT);
			if(sprite[state].is_finished()){
				check_idle();
				check_crouch();
				check_idle_atk_neutral_2();
			}else if(pressed[ATTACK_BUTTON]){
				combo++;
			}
		break;

		case FighterState::IDLE_ATK_NEUTRAL_2:
			attack_damage = 5 * (sprite[state].get_current_frame() == 1);
			attack_mask = (orientation == Orientation::LEFT ? AttackDirection::ATK_LEFT : AttackDirection::ATK_RIGHT);
			if(sprite[state].is_finished()){
				check_idle();
				check_crouch();
				check_idle_atk_neutral_3();
			}else if(pressed[ATTACK_BUTTON]){
				combo++;
			}
		break;

		case FighterState::IDLE_ATK_FRONT: //2
			attack_damage = 10 * (sprite[state].get_current_frame() == 2);
			attack_mask = (orientation == Orientation::LEFT ? AttackDirection::ATK_LEFT : AttackDirection::ATK_RIGHT);
			if(sprite[state].is_finished()){
				check_idle();
				check_crouch();
			}
		break;

		case FighterState::IDLE_ATK_DOWN: //3
			attack_damage = 10 * (sprite[state].get_current_frame() == 3);
			attack_mask = AttackDirection::ATK_DOWN;
			if(sprite[state].is_finished()){
				check_idle();
				check_crouch();
			}
		break;
		case FighterState::CROUCH_ATK: //1
		attack_damage = 3 * (sprite[state].get_current_frame() == 1);
		attack_mask = ((orientation == Orientation::LEFT ? AttackDirection::ATK_LEFT : AttackDirection::ATK_RIGHT) | AttackDirection::ATK_DOWN);
		if(sprite[state].is_finished()){
			check_idle();
			check_crouch();
		}

		case FighterState::IDLE_ATK_NEUTRAL_3: //1
		case FighterState::IDLE_ATK_UP: //1
			attack_damage = 3 * (sprite[state].get_current_frame() == 1);
			attack_mask = (orientation == Orientation::LEFT ? AttackDirection::ATK_LEFT : AttackDirection::ATK_RIGHT);
			if(sprite[state].is_finished()){
				check_idle();
				check_crouch();
			}
		break;

		case FighterState::JUMP_ATK_DOWN:
			attack_damage = 2;
			attack_mask = AttackDirection::ATK_DOWN;
			if(on_floor){
				n_sprite_start = 2;
				check_idle_atk_down(true, true);
			}
		break;

		case FighterState::JUMP_ATK_UP:
			attack_damage = 7 * (sprite[state].get_current_frame() == 1);
			attack_mask = AttackDirection::ATK_UP;
			if(sprite[state].is_finished()){
				speed.y = 0.1;
				check_fall();
				check_idle();
				check_left(false);
				check_right(false);
			}
		break;

		case FighterState::STUNT:
			attack_damage = 0;
			attack_mask = 0;
			if(sprite[state].is_finished()){
				check_fall();
				check_idle();
			}
		break;

		case FighterState::IDLE:
			attack_damage = 0;
			attack_mask = 0;
			combo = 0;
			check_jump();
			check_left(on_floor);
			check_right(on_floor);
			check_crouch();
			check_defense();
			check_idle_atk_neutral_1();
			check_idle_atk_front();
			check_idle_atk_up();
			check_idle_atk_down();
			check_pass_through_platform();
			check_fall();
		break;

		case FighterState::JUMPING:
			attack_damage = 0;
			attack_mask = 0;
			check_left(on_floor);
			check_right(on_floor);
			check_jump_atk_down();
			check_fall();
			check_jump_atk_up();
		break;

		case FighterState::FALLING:
			attack_damage = 0;
			attack_mask = 0;
			check_idle();
			check_left(on_floor);
			check_right(on_floor);
			check_fall();
			check_crouch();
			check_jump_atk_up();
			check_jump_atk_down();
		break;


		case FighterState::RUNNING:
			attack_damage = 0;
			attack_mask = 0;
			check_jump();
			check_left(false);
			check_right(false);
			check_idle();
			check_crouch();
			check_defense();
			check_idle_atk_neutral_1();
			check_idle_atk_front();
			check_pass_through_platform();
			check_fall();
		break;

		case FighterState::DEFENDING:
			attack_damage = 0;
			attack_mask = 0;
			check_idle();
			check_fall();
		break;

		case FighterState::CROUCH:
			attack_damage = 0;
			attack_mask = 0;
			check_idle();
			check_crouch_atk();
			check_fall();
		break;
	}
}

void Blood::check_jump(bool change){
	if(pressed[JUMP_BUTTON]){
		if(change) temporary_state = FighterState::JUMPING;
		speed.y = -5;
		on_floor = false;
	}
}

void Blood::check_fall(bool change){
	if(speed.y > 0){
		if(change) temporary_state = FighterState::FALLING;
	}
}

void Blood::check_left(bool change){
	if(is_holding[LEFT_BUTTON]){
		if(change) temporary_state = FighterState::RUNNING;
		speed.x = -2;
		orientation = Orientation::LEFT;
	}
}

void Blood::check_right(bool change){
	if(is_holding[RIGHT_BUTTON]){
		if(change) temporary_state = FighterState::RUNNING;
		speed.x = 2;
		orientation = Orientation::RIGHT;
	}
}

void Blood::check_idle(bool change){
	if(speed.x == 0 and on_floor and not is_holding[DOWN_BUTTON] and not is_holding[BLOCK_BUTTON]){
		if(change) temporary_state = FighterState::IDLE;
	}
}

void Blood::check_crouch(bool change){
	if(is_holding[DOWN_BUTTON] and on_floor and not is_holding[ATTACK_BUTTON]){
   		if(change) temporary_state = FighterState::CROUCH;
    }
}

void Blood::check_idle_atk_neutral_1(bool change){
	if(pressed[ATTACK_BUTTON]){
		speed.y = 0;
		if(change) temporary_state = FighterState::IDLE_ATK_NEUTRAL_1;
	}
}

void Blood::check_idle_atk_neutral_2(bool change){
	if(combo){
		combo--;
		if(change) temporary_state = FighterState::IDLE_ATK_NEUTRAL_2;
	}
}

void Blood::check_idle_atk_neutral_3(bool change){
	if(combo){
		combo--;
		if(change) temporary_state = FighterState::IDLE_ATK_NEUTRAL_3;
	}
}

void Blood::check_idle_atk_front(bool change){
	if(pressed[ATTACK_BUTTON] and (is_holding[LEFT_BUTTON] or is_holding[RIGHT_BUTTON])){
		if(change) temporary_state = FighterState::IDLE_ATK_FRONT;
		orientation = is_holding[LEFT_BUTTON] ? Orientation::LEFT : Orientation::RIGHT;
	}
}

void Blood::check_idle_atk_up(bool change) {
	if(pressed[ATTACK_BUTTON] and is_holding[UP_BUTTON]) {
		if(change) temporary_state = FighterState::IDLE_ATK_UP;
	}
}

void Blood::check_idle_atk_down(bool change, bool condition) {
	if((pressed[ATTACK_BUTTON] and is_holding[DOWN_BUTTON]) or condition) {
		if(change) temporary_state = FighterState::IDLE_ATK_DOWN;
	}
}

void Blood::check_pass_through_platform(bool change) {
	if(pressed[DOWN_BUTTON] and not is_holding[ATTACK_BUTTON]){
		if(crouch_timer.get() < CROUCH_COOLDOWN){
			if (change) temporary_state = FighterState::FALLING;
			pass_through = true;
		}
		crouch_timer.restart();
	}
}

void Blood::check_crouch_atk(bool change){
	if(pressed[ATTACK_BUTTON]){
		if(change) temporary_state = FighterState::CROUCH_ATK;
	}
}

void Blood::check_jump_atk_down(bool change){
	if(pressed[ATTACK_BUTTON] and is_holding[DOWN_BUTTON]){
		if(change) temporary_state = FighterState::JUMP_ATK_DOWN;
	}
}

void Blood::check_jump_atk_up(bool change) {
	if(pressed[ATTACK_BUTTON] and is_holding[UP_BUTTON]) {
		if(combo) return;
		pass_through = false;
		combo++;
		speed.y = -5;
		if(change) temporary_state = FighterState::JUMP_ATK_UP;
	}
}

void Blood::check_defense(bool change){
	if(is_holding[BLOCK_BUTTON] and on_floor){
   		if(change) temporary_state = FighterState::DEFENDING;
    }
}

void Blood::check_stunt(bool change){
	speed.x = 0;
	if(change) temporary_state = FighterState::STUNT;
}