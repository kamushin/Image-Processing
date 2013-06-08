#ifndef FLOW_H
#define FLOW_H

void drawMotionField(IplImage* imgU, IplImage* imgV, IplImage* imgMotion, int xSpace, int ySpace, float cutoff, int multiplier, CvScalar color);
void drawLegendHSV(IplImage* imgColor, int radius, int cx, int cy);
void drawColorField(IplImage* imgU, IplImage* imgV, IplImage* imgColor);

#endif
