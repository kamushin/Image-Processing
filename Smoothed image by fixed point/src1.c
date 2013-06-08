// draw a vector field based on horizontal and vertical flow fields
void drawmotionfield(iplimage* imgu, iplimage* imgv, iplimage* imgmotion, int xspace, int yspace, float cutoff, int multiplier, cvscalar color)
{
	int x, y;

	cvpoint p0 = cvpoint(0,0);
	cvpoint p1 = cvpoint(0,0);

	float deltax, deltay, angle, hyp;

	for(y = yspace; y < imgu->height; y+= yspace ) {

		for(x = xspace; x < imgu->width; x+= xspace ){

			p0.x = x;
			p0.y = y;

			deltax = *((float*)(imgu->imagedata + y*imgu->widthstep)+x);
			deltay = -(*((float*)(imgv->imagedata + y*imgv->widthstep)+x));

			angle = atan2(deltay, deltax);
			hyp = sqrt(deltax*deltax + deltay*deltay);

			if(hyp > cutoff){

				p1.x = p0.x + cvround(multiplier*hyp*cos(angle));
				p1.y = p0.y + cvround(multiplier*hyp*sin(angle));

				cvline( imgmotion, p0, p1, color,1, cv_aa, 0);

				p0.x = p1.x + cvround(3*cos(angle-m_pi + m_pi/4));
				p0.y = p1.y + cvround(3*sin(angle-m_pi + m_pi/4));
				cvline( imgmotion, p0, p1, color,1, cv_aa, 0);

				p0.x = p1.x + cvround(3*cos(angle-m_pi - m_pi/4));
				p0.y = p1.y + cvround(3*sin(angle-m_pi - m_pi/4));
				cvline( imgmotion, p0, p1, color,1, cv_aa, 0);
			}
		}
	}
}