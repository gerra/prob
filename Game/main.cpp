#include <SFML/Graphics.hpp>

using namespace sf;

// window size
const int WH = 400;
const int WW = 600;

// map size
const int MH = 12;
const int MW = 40;

// player size
const int PH = 50;
const int PW = 40;

// cell size
const int CH = 32;
const int CW = 32;

String map[MH] = {
	"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
	"B                                B     B",
	"B                                B     B",
	"B                                B     B",
	"B                                B     B",
	"B         0000                BBBB     B",
	"B                                B     B",
	"BBB                              B     B",
	"B              BB                BB    B",
	"B              BB                      B",
	"B    B         BB         BB           B",
	"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
};

// for scrolling map
float offSetX, offSetY;

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
		rect = FloatRect(7 * CW, 9 * CH, PW, PH);
		prev_dx = 0.1;
		dx = dy = 0.1;
		currentFrame = 0;
	}

	void update(float time) {
		rect.left += dx * time;
		CollisionX();
		// if player isn't on ground, player is falling down
		if (!onGround)
			dy += 0.0005 * time;
		rect.top += dy * time;
		onGround = false;
		CollisionY();

		// selecting frame for player's animation
		currentFrame += 0.005 * time;
		if (currentFrame > 6)
			currentFrame -= 6;
		
		// setting selecting frame
		if (dx < 0)
			sprite.setTextureRect(IntRect(40 * int(currentFrame) + 40, 244, -PW, PH));
		if (dx > 0)
			sprite.setTextureRect(IntRect(40 * int(currentFrame), 244, PW, PH));
		if (dx == 0) {
			if (prev_dx > 0)
				sprite.setTextureRect(IntRect(0, 189, PW, PH));
			if (prev_dx < 0)
				sprite.setTextureRect(IntRect(40, 189, -PW, PH));
		}

		sprite.setPosition(rect.left - offSetX, rect.top - offSetY);

		prev_dx = dx;
		dx = 0;
	}

	void CollisionX() {
		for (int i = rect.top / CH; i < (rect.top + PH) / CH; ++i)
			for (int j = rect.left / CW; j < (rect.left + PW) / CW; ++j) {
				if (map[i][j] == 'B') {
					if (dx > 0)
						rect.left = j * CW - PW;
					if (dx < 0)
						rect.left = j * CW + CW;
				}
				if (map[i][j] == '0')
					map[i][j] = ' ';
			}
	}
	void CollisionY() {
		for (int i = rect.top / CH; i < (rect.top + PH) / CH; ++i)
			for (int j = rect.left / CW; j < (rect.left + PW) / CW; ++j) {
				if (map[i][j] == 'B') {
					if (dy > 0) {
						rect.top = i * CH - PH;
						onGround = true;
						dy = 0;
					}
					if (dy < 0) {
						rect.top = i * CH + CH;
						dy = 0;
					}
				}
				if (map[i][j] == '0')
					map[i][j] = ' ';
			}
	}
};

int main()
{
    RenderWindow window(VideoMode(WW, WH), "Prob!");

	Texture t;
	t.loadFromFile("fang.png");

	RectangleShape rectangle(Vector2f(CW, CH));

	Player player(t);
	Clock clock;

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
				player.dy = -0.35;
				player.onGround = false;
			}
		}
		player.update(time);
        window.clear(Color::White);

		if (player.rect.left - WW / 2 > 0 && player.rect.left + PW + WW / 2 < MW * CW + CW)
			offSetX = player.rect.left - 600 / 2;
		if (player.rect.top - WH / 2 > 0 && player.rect.top + PH + WH / 2 < MH * CH + CH)
			offSetY = player.rect.top - 400 / 2;

		for (int i = 0; i < MH; ++i)
			for (int j = 0; j < MW; ++j) {
				if (map[i][j] == 'B')
					rectangle.setFillColor(Color::Black);
				if (map[i][j] == '0')
					rectangle.setFillColor(Color::Green);
				if (map[i][j] == ' ')
					continue;

				rectangle.setPosition(j * CW - offSetX, i * CH - offSetY);
				window.draw(rectangle);
			}

        window.draw(player.sprite);
        window.display();
    }

    return 0;
}