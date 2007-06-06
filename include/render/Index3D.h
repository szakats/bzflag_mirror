#ifndef INDEX3D_H_
#define INDEX3D_H_

class Index3D {
	
	public:
	
	int a, b, c,		// vertex indices
	    t1, t2, t3;	// texture coordinate indices
	
	Index3D() {
		a = b = c = t1 = t2 = t3 = 0;	
	}
	
	Index3D(int a, int b, int c) {
		this->a = a;
		this->b = b;
		this->c = c;
		this->t1 = a;
		this->t2 = b;
		this->t3 = c;
	}
	
	Index3D(int a, int b, int c, int t1, int t2, int t3) {
		this->a = a;
		this->b = b;
		this->c = c;
		this->t1 = t1;
		this->t2 = t2;
		this->t3 = t3;
	}
	
	virtual ~Index3D() {}
};


#endif /*INDEX3D_H_*/
