#ifndef TEXCOORD2D_H_
#define TEXCOORD2D_H_


class TexCoord2D {
	public:
	
	float u, v;
	
	TexCoord2D() {
		u = v = 0.0;	
	}
	
	TexCoord2D(float u, float v) {
		this->u = u; this->v = v;
	}
	
	TexCoord2D(double u, double v) {
		this->u = (float)u; this->v = (float)v;
	}
};

#endif /*TEXCOORD2D_H_*/
