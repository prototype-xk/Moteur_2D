#include "Player.h"

#define ANIM_WIDTH   64.f
#define ANIM_HEIGHT  64.f

#define ANIM_FRAMES_IDLE 2
#define ANIM_FRAMES_WALK 9

#define ANIM_DELAY_IDLE 300
#define ANIM_DELAY_WALK 100

enum AnimRow {
	WALK = 8,
	IDLE = 22,
	JUMP = 26,
	RUN = 36,
};

Player::Player(float screenW, float screenH, ResourceManager& res)
	: x(screenW * 0.5f - RECT_WIDTH * 0.5f),
	y(screenH * 0.75f),
	rect{ x, y, RECT_WIDTH, RECT_HEIGHT },
	color(PLAYER_IDLE_COLOR),
	resources(res),
	state(CHSTATE_IDLE),
	dir(CHDIR_SOUTH),
	speed(0.0f),
	animStartTime(SDL_GetTicks()),
	updateTime(SDL_GetTicks())
{
	std::cout << "[PLAYER] Spawned at x=" << x
		<< " y=" << y
		<< " screen " << screenW << "x" << screenH << ")\n";

	// Charge ta spritesheet (mets le bon chemin de ton fichier)
	resources.loadTexture("player_sprite", "assets/character-spritesheet.png");
	anims = resources.getTexture("player_sprite");
}

void Player::respawn(float screenW, float screenH) {
	x = screenW * 0.5f - RECT_WIDTH * 0.5f;
	y = screenH * 0.75f;
	rect = { x, y, RECT_WIDTH, RECT_HEIGHT };
	std::cout << "[PLAYER] RESPAWN x=" << x << " y=" << y << "\n";
}

void Player::handleEvent(const SDL_Event& e) {
	if (e.type == SDL_EVENT_KEY_DOWN && !e.key.repeat) {
		if (e.key.key == SDLK_Q) moveLeft = true;
		if (e.key.key == SDLK_D) moveRight = true;
		if (e.key.key == SDLK_Z) moveUp = true;
		if (e.key.key == SDLK_S) moveDown = true;
	}
	if (e.type == SDL_EVENT_KEY_UP) {
		if (e.key.key == SDLK_Q) moveLeft = false;
		if (e.key.key == SDLK_D) moveRight = false;
		if (e.key.key == SDLK_Z) moveUp = false;
		if (e.key.key == SDLK_S) moveDown = false;
	}
}

void Player::update(float deltaTime) {
	State newState = state;
	Uint64 currentTime = SDL_GetTicks();

	// Machine à états : gestion des transitions
	switch (state) {
	case CHSTATE_IDLE:
		// Si une touche directionnelle est pressée, passe en marche
		if (moveLeft || moveRight) {
			newState = CHSTATE_WALKING;
			speed = moveLeft ? -300.0f : 300.0f;
			dir = moveLeft ? CHDIR_WEST : CHDIR_EAST;
		}
		break;

	case CHSTATE_WALKING:
		// Si aucune touche n'est pressée, retour à idle
		if (!moveLeft && !moveRight) {
			newState = CHSTATE_IDLE;
			speed = 0.0f;
		}
		else {
			// Met à jour la direction et vitesse si changement
			speed = moveLeft ? -300.0f : 300.0f;
			dir = moveLeft ? CHDIR_WEST : CHDIR_EAST;

			// Déplacement horizontal
			x += speed * deltaTime;
		}
		break;

	case CHSTATE_JUMPING:
		// TODO: Logique de saut à implémenter plus tard
		break;
	}

	// Déplacement vertical (temporaire, à remplacer par une vraie physique)
	if (moveUp) {
		y -= 500.0f * deltaTime;
	}
	if (moveDown) {
		y += 500.0f * deltaTime;
	}

	// Si l'état a changé, réinitialise le timer d'animation
	if (state != newState) {
		state = newState;
		animStartTime = currentTime;
	}

	// Met à jour le temps et la position du rectangle
	updateTime = currentTime;
	rect.x = x;
	rect.y = y;
}

void Player::render(SDL_Renderer* renderer) {
	// Rectangle de destination (où afficher à l'écran)
	SDL_FRect rcdAnim = { rect.x, rect.y, ANIM_WIDTH, ANIM_HEIGHT };

	// Rectangle source (quelle partie de la spritesheet découper)
	SDL_FRect rcsAnim = { 0, 0, ANIM_WIDTH, ANIM_HEIGHT };

	int row, frame;

	// Détermine quelle animation jouer selon l'état
	switch (state) {
	case CHSTATE_WALKING:
		row = AnimRow::WALK;
		frame = GetAnimFrame(ANIM_DELAY_WALK, ANIM_FRAMES_WALK);
		break;

	case CHSTATE_JUMPING:
		row = AnimRow::JUMP;
		frame = 0; // Temporaire, on verra le jump plus tard
		break;

	default: // CHSTATE_IDLE
		row = AnimRow::IDLE;
		frame = GetAnimFrame(ANIM_DELAY_IDLE, ANIM_FRAMES_IDLE);
		break;
	}

	// Calcule la position dans la spritesheet
	rcsAnim.x = ANIM_WIDTH * frame;          // Colonne (quelle frame)
	rcsAnim.y = ANIM_HEIGHT * (row + dir);   // Ligne (animation + direction)

	// Affiche la texture
	SDL_RenderTexture(renderer, anims, &rcsAnim, &rcdAnim);
}
