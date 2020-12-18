#include "add_id.h"

void add_id(Mat &image, int nrof_imgs){
	int h = image.rows;
	int w = image.cols;
	int min_side = h < w ? h : w;
	int radius = min_side / 2 - 20;
	circle(image, Point((int)w/2, (int)h/2), min_side, Scalar(0, 0, 0), min_side+70);

	for (int i = 0; i < NORF_LINES; i++) {
		float angle = 2 * PI * i / NORF_LINES;
		float x = (int)w/2 + sin(angle) * radius;
		float y = (int)h/2 + cos(angle) * radius;
		line(image, Point(x, y), Point(x+sin(angle)*10, y+cos(angle)*10), Scalar(255, 255, 255), 1);
	}

	double percent = (double)nrof_imgs/(double)MAX_IMGS;
	int nrof_big_lines = NORF_LINES * percent;

	for (int j = 0; j < nrof_big_lines; j++){
		float angle = 2 * PI * j / NORF_LINES;
		float x = (int)w/2 + sin(angle) * radius;
		float y = (int)h/2 + cos(angle) * radius;
		line(image, Point(x, y), Point(x+sin(angle)*15, y+cos(angle)*15), Scalar(255, 255, 255), 2);
	}
}