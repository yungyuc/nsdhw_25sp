#include "_matrix.hpp"
#include <iostream>

void show_matrix(Matrix mat){
    std::cout << std::string(mat.ncol()*5, '=') << std::endl;
    for(size_t i=0; i < mat.nrow(); i++){
        for(size_t j=0; j<mat.ncol(); j++){
            std::cout << mat(i,j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::string(mat.ncol()*5, '=') << std::endl;
}

Matrix ones(size_t s){
    Matrix res(s,s);
    
    for(int i = 0; i< s;i++){
        for(int j=0; j<s; j++){
            if(i==j) res(i,j) = 1.0;
        }
    }

    return res;
}


int main(int argc, char ** argv)
{
    size_t size = 12;
    Matrix mat1(size, size);
    initialize(mat1);
    Matrix mat2 = ones(size);

    Matrix res(size,size);
    Matrix ref(size,size);

    res = simple_tile(mat1,mat2, 25);
    ref = multiply_mkl(mat1, mat2);

    show_matrix(res);

    if(res == ref) std::cout << "Match" << std::endl;
    else std::cout << "Mismatch" << std::endl;

    return 0;
}
