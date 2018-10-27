#ifndef EMOTION_H_
#define EMOTION_H_

class Emotion {
public:
	Emotion();
	void AddPositive(int val);
	void AddNegative(int val);

	bool isPositive();
	bool isNegative();

private:
	int m_value;
};

#endif