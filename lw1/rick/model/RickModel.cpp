#include "RickModel.h"

RickModel::Feature::Feature(sf::Shape* shape, sf::Color color)
	: shapes(shape)
	, color(color)
{
}

RickModel::Feature::Feature(Feature&& other) noexcept
	: shapes(other.shapes)
	, color(other.color)
{
	other.shapes = nullptr;
}

RickModel::Feature::~Feature()
{
	delete shapes;
}

RickModel::RickModel()
{
	CreateHair();
	CreateFace();
	CreateEyes();
	CreateSmile();
	CreateLabCoat();
}

RickModel::~RickModel()
{
	for (auto& feature : m_features)
	{
		delete feature.shapes;
	}
}

std::vector<RickModel::Feature>& RickModel::GetFeatures()
{
	return m_features;
}

const std::vector<RickModel::Feature>& RickModel::GetFeatures() const
{
	return m_features;
}

void RickModel::Move(const sf::Vector2f& delta)
{
	for (auto& feature : m_features)
	{
		feature.shapes->move(delta);
	}
}

void RickModel::CreateHair()
{
	auto* hair = new sf::ConvexShape(12);
	hair->setPoint(0, { 400.f, 150.f });
	hair->setPoint(1, { 390.f, 160.f });
	hair->setPoint(2, { 370.f, 180.f });
	hair->setPoint(3, { 360.f, 160.f });
	hair->setPoint(4, { 365.f, 210.f });
	hair->setPoint(5, { 370.f, 150.f });
	hair->setPoint(6, { 400.f, 250.f });
	hair->setPoint(7, { 430.f, 150.f });
	hair->setPoint(8, { 435.f, 210.f });
	hair->setPoint(9, { 440.f, 170.f });
	hair->setPoint(10, { 430.f, 180.f });
	hair->setPoint(11, { 410.f, 160.f });

	hair->setFillColor({ 180, 220, 255 });
	m_features.emplace_back(hair, sf::Color(180, 220, 255));
}

void RickModel::CreateFace()
{
	auto* face = new sf::CircleShape(40.f);
	face->setPosition({ 360.f, 190.f });
	face->setFillColor({ 255, 220, 180 });
	m_features.emplace_back(face, sf::Color(255, 220, 180));
}

void RickModel::CreateEyes()
{
	auto* leftEye = new sf::CircleShape(8.f);
	leftEye->setPosition({ 375.f, 210.f });
	leftEye->setFillColor(sf::Color::White);
	m_features.emplace_back(leftEye, sf::Color::White);

	auto* rightEye = new sf::CircleShape(8.f);
	rightEye->setPosition({ 405.f, 210.f });
	rightEye->setFillColor(sf::Color::White);
	m_features.emplace_back(rightEye, sf::Color::White);

	auto* leftPupil = new sf::CircleShape(4.f);
	leftPupil->setPosition({ 379.f, 214.f });
	leftPupil->setFillColor(sf::Color::Black);
	m_features.emplace_back(leftPupil, sf::Color::Black);

	auto* rightPupil = new sf::CircleShape(4.f);
	rightPupil->setPosition({ 409.f, 214.f });
	rightPupil->setFillColor(sf::Color::Black);
	m_features.emplace_back(rightPupil, sf::Color::Black);
}

void RickModel::CreateSmile()
{
	auto* smile = new sf::ConvexShape(4);
	smile->setPoint(0, { 380.f, 240.f });
	smile->setPoint(1, { 420.f, 240.f });
	smile->setPoint(2, { 415.f, 255.f });
	smile->setPoint(3, { 385.f, 255.f });
	smile->setFillColor({ 200, 50, 50 });
	m_features.emplace_back(smile, sf::Color(200, 50, 50));
}

void RickModel::CreateLabCoat()
{
	auto* labCoat = new sf::ConvexShape(4);
	labCoat->setPoint(0, { 350.f, 250.f });
	labCoat->setPoint(1, { 450.f, 250.f });
	labCoat->setPoint(2, { 440.f, 350.f });
	labCoat->setPoint(3, { 360.f, 350.f });
	labCoat->setFillColor(sf::Color::White);
	m_features.emplace_back(labCoat, sf::Color::White);
}