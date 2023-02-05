#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include <direct.h>

int main()
{
	try
	{
		int w = 1920;
		int h = 1080;

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

		fragmentShader.setUniform("u_resolution", sf::Vector2f(w, h));

		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window.close();
				}
			}

			sf::Shader::bind(&fragmentShader);

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