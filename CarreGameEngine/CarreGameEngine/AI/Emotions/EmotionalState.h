#ifndef EMOTIONALSTATE_H_
#define EMOTIONALSTATE_H_

#include "Emotion.h"

enum class Emotions { JOY, SADNESS, TRUST, DISGUST, FEAR, ANGER, SURPRISE, ANTICIPATION };

class EmotionalState {
public:
	EmotionalState();
	void AddEmotion(Emotions emotion, int val);
	bool isEmotion(Emotions emotion);
	
	bool isJoy();
	bool isSad();
	bool isTrust();
	bool isDisgust();
	bool isFear();
	bool isAnger();
	bool isSurprise();
	bool isAnticipation();

private:
		///Joy-Sadness
	Emotion m_js;
		///Trust-Disgust
	Emotion m_td;
		///Fear-Anger
	Emotion m_fa;
		///Surprise-Anticipation
	Emotion m_sa;

	/*
	Emotions values change
	Rules should determine what state the AI is in (read from script)
	Change state
	eg. If m_joy > 70 && m_sad < 10
	change state -> happy;

	Happy state -> AI moves around much faster ie. AIspeed * 2
	*/
};

#endif
