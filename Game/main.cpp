#include <SFML/Graphics.hpp>

using namespace sf;

// ground's level
int ground  = 150;

class Player {
public:
	float dx, dy; // velocity
	float prev_dx; // for correct orrientation after run
	FloatRect rect; // rect of player
	bool onGround; // on ground or in jump
	Sprite sprite; // image of player
	float currentFrame; // number of player's image

	Player(Texture & image) {
		sprite.setTexture(image);
		rect = FloatRect(0, 0, 40, 50);
		prev_dx = 0.1;
		dx = dy = 0;
		currentFrame = 0;
	}

	void update(float time) {
		rect.left += dx * time;
		// if player isn't on ground, player is falling down
		if (!onGround)
			dy += 0.0005 * time;
		rect.top += dy * time;
		onGround = false;

		if (rect.top > ground) {
			onGround = true;
			dy = 0;
			rect.top = ground;
		}

		// selecting frame for player's animation
		currentFrame += 0.005 * time;
		if (currentFrame > 6)
			currentFrame -= 6;
		
		// setting selecting frame
		if (dx < 0)
			sprite.setTextureRect(IntRect(40 * int(currentFrame) + 40, 244, -40, 50));
		if (dx > 0)
			sprite.setTextureRect(IntRect(40 * int(currentFrame), 244, 40, 50));
		if (dx == 0) {
			if (prev_dx > 0)
				sprite.setTextureRect(IntRect(0, 189, 40, 50));
			if (prev_dx < 0)
				sprite.setTextureRect(IntRect(40, 189, -40, 50));
		}

		sprite.setPosition(rect.left, rect.top);

		prev_dx = dx;
		dx = 0;
	}
};

int main()
{
    RenderWindow window(VideoMode(600, 400), "Prob!");

	Texture t;
	t.loadFromFile("fang.png");

	Player player(t);
	Clock clock;
	clock.restart();
    while (window.isOpen()) {
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time /= 700;
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

		if (Keyboard::isKeyPressed(Keyboard::Left))
			player.dx = -0.1;
		if (Keyboard::isKeyPressed(Keyboard::Right))
			player.dx = +0.1;
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			if (player.onGround) {
				player.dy = -0.4;
				player.onGround = false;
			}
		}
		player.update(time);

        window.clear();
        window.draw(player.sprite);
        window.display();
    }

    return 0;
}