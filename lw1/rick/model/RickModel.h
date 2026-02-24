#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class RickModel {
public:
	struct Feature {
		sf::Shape* shapes;
		sf::Color color;

		Feature(sf::Shape* shape, sf::Color color);
		Feature(const Feature&) = delete;
		Feature& operator=(const Feature&) = delete;
		Feature(Feature&& other) noexcept;
		~Feature();
	};

	RickModel();
	~RickModel();

	RickModel(const RickModel&) = delete;
	RickModel& operator=(const RickModel&) = delete;

	std::vector<Feature>& GetFeatures();
	[[nodiscard]] const std::vector<Feature>& GetFeatures() const;
	void Move(const sf::Vector2f& delta);

private:
	void CreateHair();
	void CreateFace();
	void CreateEyes();
	void CreateSmile();
	void CreateLabCoat();

	std::vector<Feature> m_features;
};