#include "EmotionalState.h"

EmotionalState::EmotionalState(){
}

void EmotionalState::AddEmotion(Emotions emotion, int val) {
	switch (emotion) {
		case Emotions::ANGER: m_fa.AddNegative(val);
			break;
		case Emotions::ANTICIPATION: m_sa.AddNegative(val);
			break;
		case Emotions::DISGUST: m_td.AddNegative(val);
			break;
		case Emotions::FEAR: m_fa.AddPositive(val);
			break;
		case Emotions::JOY: m_js.AddPositive(val);
			break;
		case Emotions::SADNESS: m_js.AddNegative(val);
			break;
		case Emotions::SURPRISE: m_sa.AddPositive(val);
			break;
		case Emotions::TRUST: m_td.AddPositive(val);
			break;
	}
}

bool EmotionalState::isEmotion(Emotions emotion) {
	switch (emotion) {
		case Emotions::ANGER: return m_fa.isNegative();
			break;
		case Emotions::ANTICIPATION: return m_sa.isNegative();
			break;
		case Emotions::DISGUST: return m_td.isNegative();
			break;
		case Emotions::FEAR: return m_fa.isPositive();
			break;
		case Emotions::JOY: return m_js.isPositive();
			break;
		case Emotions::SADNESS: return m_js.isNegative();
			break;
		case Emotions::SURPRISE: return m_sa.isPositive();
			break;
		case Emotions::TRUST: return m_td.isPositive();
			break;
	}
}

bool EmotionalState::isJoy() {
	return m_js.isPositive();
}

bool EmotionalState::isSad() {
	return m_js.isNegative();
}

bool EmotionalState::isTrust() {
	return m_td.isPositive();
}

bool EmotionalState::isDisgust() {
	return m_td.isNegative();
}

bool EmotionalState::isFear() {
	return m_fa.isPositive();
}

bool EmotionalState::isAnger() {
	return m_fa.isNegative();
}

bool EmotionalState::isSurprise() {
	return m_sa.isPositive();
}

bool EmotionalState::isAnticipation() {
	return m_sa.isNegative();
}

