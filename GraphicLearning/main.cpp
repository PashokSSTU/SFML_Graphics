#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <direct.h>
#include <ctime>
#include <cmath>

#define LOG(x) std::cout << x << std::endl;

int main()
{
	try
	{
		int w = 1920;
		int h = 1080;
		double mouseX = 0;
		double mouseY = 0;
		
		sf::Vector3f cameraPosition = sf::Vector3f(-5.0, 0.0, 0.0);
		float cameraMovingSpeed = 0.1;
		bool pressedMovingButtons[] = { false, false, false, false, false, false }; //WASD space lshift
		bool mouseHidden = false;
		float mouseSensivity = 1;

		sf::RenderWindow window(sf::VideoMode(w, h), "SFML_test");
		window.setFramerateLimit(60);
		sf::RenderTexture firstTexture;
		firstTexture.create(w, h);
		sf::Sprite firstTextureSprite = sf::Sprite(firstTexture.getTexture());
		firstTextureSprite.setScale(1, -1); // textures are upside down by default
		firstTextureSprite.setPosition(0, h);

		sf::Shader fragmentShader;
		if (!fragmentShader.loadFromFile("Shaders/OutputShader.frag", sf::Shader::Fragment))
		{
			std::stringstream ss;
			ss << "Shader::Error of loading or compiling GLSL shader: [" << "Shaders/OutputShader.frag]" << "!";
			throw std::exception(ss.str().c_str());
		}

		sf::Clock clock;

		while (window.isOpen())
		{
			sf::Shader::bind(&fragmentShader);

			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window.close();
				}

				if (event.type == sf::Event::KeyPressed)
				{
					if (event.key.code == sf::Keyboard::Escape)
					{
						mouseHidden = false;
						window.setMouseCursorVisible(true);
						window.close();
					}

					if (event.key.code == sf::Keyboard::W) pressedMovingButtons[0] = true;
					else if(event.key.code == sf::Keyboard::A) pressedMovingButtons[1] = true;
					else if(event.key.code == sf::Keyboard::S) pressedMovingButtons[2] = true;
					else if(event.key.code == sf::Keyboard::D) pressedMovingButtons[3] = true;
					else if(event.key.code == sf::Keyboard::Space) pressedMovingButtons[4] = true;
					else if(event.key.code == sf::Keyboard::LShift) pressedMovingButtons[5] = true;
				}

				if (event.type == sf::Event::KeyReleased)
				{
					if (event.key.code == sf::Keyboard::W) pressedMovingButtons[0] = false;
					else if (event.key.code == sf::Keyboard::A) pressedMovingButtons[1] = false;
					else if (event.key.code == sf::Keyboard::S) pressedMovingButtons[2] = false;
					else if (event.key.code == sf::Keyboard::D) pressedMovingButtons[3] = false;
					else if (event.key.code == sf::Keyboard::Space) pressedMovingButtons[4] = false;
					else if (event.key.code == sf::Keyboard::LShift) pressedMovingButtons[5] = false;
				}

				if (event.type == sf::Event::MouseButtonPressed)
				{
					mouseHidden = true;
					window.setMouseCursorVisible(false);
				}

				if (event.type == sf::Event::MouseMoved)
				{
					mouseX += event.mouseMove.x - w / 2;
					mouseY += event.mouseMove.y - h / 2;
					sf::Mouse::setPosition(sf::Vector2i(w / 2, h / 2), window);
				}
			}

			if (mouseHidden)
			{
				float mx = (mouseX / w - 0.5) * mouseSensivity;
				float my = (mouseY / h - 0.5) * mouseSensivity;
				sf::Vector3f dir = sf::Vector3f(0.0, 0.0, 0.0);
				sf::Vector3f dirTemp;

				if (pressedMovingButtons[0]) dir = sf::Vector3f(1.0, 0.0, 0.0);
				else if (pressedMovingButtons[2]) dir = sf::Vector3f(-1.0, 0.0, 0.0);

				if (pressedMovingButtons[3]) dir += sf::Vector3f(0.0, 1.0, 0.0);
				else if (pressedMovingButtons[1]) dir += sf::Vector3f(0.0, -1.0, 0.0);

				dirTemp.z = dir.z * cos(my) - dir.x * sin(my);
				dirTemp.x = dir.z * sin(my) + dir.x * cos(my);
				dirTemp.y = dir.y;

				dir.x = dirTemp.x * cos(mx) - dirTemp.y * sin(mx);
				dir.y = dirTemp.x * sin(mx) + dirTemp.y * cos(mx);
				dir.z = dirTemp.z;

				cameraPosition += dir * cameraMovingSpeed;

				if (pressedMovingButtons[4]) cameraPosition.z += cameraMovingSpeed;
				else if (pressedMovingButtons[5]) cameraPosition.z -= cameraMovingSpeed;

				fragmentShader.setUniform("u_position", cameraPosition);
				fragmentShader.setUniform("u_mouse", sf::Vector2f(mx, my));
			}

			fragmentShader.setUniform("u_resolution", sf::Vector2f(w, h));
			window.clear();
			window.draw(firstTextureSprite);
			window.display();

			sf::Shader::bind(NULL);
		}
	}
	catch (const std::exception& ex)
	{
		_mkdir("SFMLLogs"); // return -1 if difectory is exist, 0 - if succesfully created
		std::ofstream file("SFMLLogs/Exceptions.txt", std::ios::out);
		if (file.is_open())
		{
			file << "Error::" << ex.what();
			return -1;
		}
		file.close();
	}

	return 0;
}