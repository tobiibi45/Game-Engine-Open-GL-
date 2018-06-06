// Matrix Class    by Alan Baylis 2001

#ifndef _Matrix_H
#define _Matrix_H

#include <vector>
#include <glm/detail/type_vec3.hpp>
#include <glm/gtc/quaternion.hpp>

class MATRIX
{
    public:
        MATRIX();
        ~MATRIX();

        void LoadIdentity();
        void ODEtoOGL(const double* p, const double* R);
        void CopyMatrix(float m[16]);
        void MultMatrix(float m[16]);
        void MatrixInverse(); 
        void MatrixFromAxisAngle(glm::vec3 axis, float theta);
        void QuatToMatrix(glm::quat quat); 

        float Element[16];
};

#endif

