#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include <direct.h>

int main()
{
	try
	{
		sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML_test");

		sf::Shader shader;
		if (!shader.loadFromFile("Shaders/OutputShader.frag", sf::Shader::Fragment))
		{
			std::stringstream ss;
			ss << "Shader::Error of loading or compiling GLSL shader: [" << "Shaders/OutputShader.frag]" << "!";
			throw std::exception(ss.str().c_str());
		}

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

			window.clear(sf::Color(255, 0, 0, 0));

			window.display();
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