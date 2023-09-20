#include"graphic.h"
#include"window.h"
#include"input.h"

//���_�Ƃ�����W
static float Ox, Oy;
//1�Ƃ���傫��
static float Scl;

void mathAxis(float ox, float oy, float scl, float thickness, int order)
{
	Ox = ox;
	Oy = oy;
	Scl = scl;

	//���A����
	thickness *= scl;
	line(0, oy, getWidth(), oy, thickness, order);
	line(ox, 0, ox, getHeight(), thickness, order);

	//���_����num�̃�������`��
	float l = thickness * 5;
	int num, i;
	num = int((getWidth() - ox) / scl);
	for (i = 1; i <= num; i++) {
		float x = ox + scl * i;
		line(x, oy - l, x, oy + l, thickness, order);
	}
	num = int(ox / scl);
	for (i = 1; i <= num; i++) {
		float x = ox + scl * -i;
		line(x, oy - l, x, oy + l, thickness, order);
	}
	num = int((getHeight() - oy) / scl);
	for (i = 1; i <= num; i++) {
		float y = oy + scl * i;
		line(ox - l, y, ox + l, y, thickness, order);
	}
	num = int(oy / scl);
	for (i = 1; i <= num; i++) {
		float y = oy + scl * -i;
		line(ox - l, y, ox + l, y, thickness, order);
	}
}
void mathCircle(float x, float y, float diameter)
{
	//�X�N���[�����W�ɕϊ�
	x = Ox + Scl * x;
	y = Oy - Scl * y;
	diameter *= Scl;
	circle(x, y, diameter, 5);
}
void mathLine(float sx, float sy, float ex, float ey, float thickness)
{
	//�X�N���[�����W�ɕϊ�
	sx = Ox + Scl * sx;
	sy = Oy - Scl * sy;
	ex = Ox + Scl * ex;
	ey = Oy - Scl * ey;
	thickness *= Scl;
	line(sx, sy, ex, ey, thickness, 5);
}
void mathArrow(float sx, float sy, float ex, float ey, float thickness, float size)
{
	//�X�N���[�����W�ɕϊ�
	sx = Ox + Scl * sx;
	sy = Oy - Scl * sy;
	ex = Ox + Scl * ex;
	ey = Oy - Scl * ey;
	thickness *= Scl;
	size *= Scl;
	arrow(sx, sy, ex, ey, thickness, size, 5);
}
void mathGraph(float (*f)(float), float inc, float thickness, float diameter)
{
	float maxX = (getWidth() - Ox) / Scl + inc;
	for (float x = 0; x < maxX; x += inc) {
		mathCircle(x, f(x), diameter);
		mathLine(x, f(x), x + inc, f(x + inc), thickness);
	}
	float minX = -Ox / Scl - inc;
	for (float x = 0; x > minX; x -= inc) {
		mathCircle(x, f(x), diameter);
		mathLine(x, f(x), x - inc, f(x - inc), thickness);
	}
}
float getMathMouseX()
{
	return (getMouseX() - Ox) / Scl;
}
float getMathMouseY()
{
	return -(getMouseY() - Oy) / Scl;
}
