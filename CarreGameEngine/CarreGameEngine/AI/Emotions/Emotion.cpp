#include "Emotion.h"

Emotion::Emotion() {

}

void Emotion::AddPositive(int val) {
	m_value += val;
	if (m_value < 100) {
		m_value = 100;
	}
}

void Emotion::AddNegative(int val) {
	m_value -= val;
	if (m_value < -100) {
		m_value = -100;
	}
}

bool Emotion::isPositive() {
	return m_value > 25;
}

bool Emotion::isNegative() {
	return m_value < -25;
}